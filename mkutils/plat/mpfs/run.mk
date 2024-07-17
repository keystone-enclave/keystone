
###########################
## Flash and run targets ##
###########################

export SC_INSTALL_DIR ?= /opt/microchip/SoftConsole-v2022.2-RISC-V-747

OPENOCD_FLAGS := -c "gdb_port 3333" -c "telnet_port 4444" -c "tcl_port 6666" \
                    -c "set DEVICE MPFS" -c "set JTAG_KHZ 11000"

ifneq ($(MPFS_COREID),)
	OPENOCD_FLAGS += -c "set COREID $(MPFS_COREID)"
endif

OPENOCD_FLAGS += --file board/microsemi-riscv.cfg

run:
	$(call log,info,Starting OpenOCD)
	$(SC_INSTALL_DIR)/openocd/bin/openocd $(OPENOCD_FLAGS)

debug-connect:
	$(call log,info,Connecting to OpenOCD)
	PYTHONPATH=$(BUILDROOT_BUILDDIR)/build/host-gcc-final-11.4.0/libstdc++-v3/python \
		$(BUILDROOT_BUILDDIR)/host/bin/riscv64-buildroot-linux-gnu-gdb \
		-iex "set KEYSTONE=$(KEYSTONE)" \
		-x $(KEYSTONE)/scripts/gdb/mpfs.cfg
