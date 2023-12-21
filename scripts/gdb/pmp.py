import gdb

NUM_PMP_REGISTERS = 16

def cfg_regs(bits):
        if bits == 32:
                return [f'pmpcfg{i}' for i in range(0, int(NUM_PMP_REGISTERS / 4))]
        elif bits == 64:
                return [f'pmpcfg{2 * i}' for i in range(0, int(NUM_PMP_REGISTERS / 8))]
        else:
                return None

class PMPClear(gdb.Command):
        def __init__(self, bits):
                super(PMPClear, self).__init__("pmp-clear", gdb.COMMAND_NONE)
                self.cfg_regs = cfg_regs(bits)

        def invoke(self, arg, from_tty):
                # Clear addresses
                for i in range(NUM_PMP_REGISTERS):
                        gdb.execute(f'set $pmpaddr{i} = 0')

                # Clear configuratoins
                for reg in self.cfg_regs:
                        gdb.execute(f'set ${reg} = 0')

class PMPDump(gdb.Command):
        def __init__(self, bits):
                super(PMPDump, self).__init__("pmp-dump", gdb.COMMAND_NONE)
                self.bits = bits
                self.cfg_regs = cfg_regs(bits)
                if bits == 32:
                        self.pmp_per_group = 4
                elif bits == 64:
                        self.pmp_per_group = 8
                else:
                        self.pmp_per_group = None

        def invoke(self, arg, from_tty):
                f = gdb.selected_frame()

                # Keep track of previous addresses for TOR regions
                prev_addr = None

                # Parse configurations first
                for i, reg in enumerate(self.cfg_regs):
                        cfg_val = f.read_register(f'{reg}')

                        # Now parse addresses
                        for j in range(self.pmp_per_group):
                                reg_cfg = (cfg_val >> (8 * j)) & 0xFF
                                r = (reg_cfg & 0b00000001) != 0
                                w = (reg_cfg & 0b00000010) != 0
                                x = (reg_cfg & 0b00000100) != 0
                                a = (reg_cfg & 0b00011000) >> 3
                                l = (reg_cfg & 0b10000000) != 0

                                reg_addr = f.read_register(f'pmpaddr{self.pmp_per_group * i + j}')
                                if a == 0:
                                        # Disabled region, continue
                                        type = "disabled"
                                        prev_addr = reg_addr << 2
                                        continue
                                elif a == 1:
                                        # TOR region
                                        type = "TOR"
                                        range_bottom = (0 if prev_addr is None else prev_addr)
                                        range_top = reg_addr << 2
                                elif a == 2:
                                        # NA4 region
                                        type = "NA4"
                                        range_bottom = reg_addr
                                        range_top = reg_addr + 4
                                elif a == 3:
                                        # NAPOT region. First, find the leftmost 1
                                        type = "NAPOT"
                                        for k in range(self.bits):
                                                if (reg_addr >> k) & 0b1 == 0:
                                                        break

                                        size = 2 ** (k + 3)
                                        range_bottom = (reg_addr & ~((1 << k) - 1)) << 2
                                        range_top = range_bottom + size
                                else:
                                        print('error: unknown type!')
                                        continue

                                print(f'PMP reg {self.pmp_per_group * i + j} {type}')
                                print(f'\tcfg\t{hex(reg_cfg)} {"R" if r else ""}{"W" if w else ""}{"X" if x else ""}{"L" if l else ""}')
                                print(f'\taddr\t{hex(reg_addr)} = {hex(range_bottom)} -> {hex(range_top)}')

                                prev_addr = reg_addr

# Figure out whether we are in 32-bit or 64-bit mode
misa = int(gdb.selected_frame().read_register('misa'))
print(hex(misa))
if misa & (0b11 << 30) == 1 << 30:
        PMPClear(32)
        PMPDump(32)
elif misa & (0b11 << 62) == 2 << 62:
        PMPClear(64)
        PMPDump(64)
else:
        print('Unsupported MISA size')
