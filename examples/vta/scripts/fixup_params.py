import struct
import sys

kTVMNDArrayMagic = 0xDD5E40F096B4A13F
kTVMNDArrayListMagic = 0xF7E58D4F05049CB7


def fixup(file, ofile):
    out_data = b''

    with open(file, 'rb') as f:
        header = struct.unpack('<Q', f.read(8))[0]
        assert header == kTVMNDArrayListMagic
        out_data += struct.pack('<Q', header)

        reserved = struct.unpack('<Q', f.read(8))[0]
        assert reserved == 0
        out_data += struct.pack('<Q', reserved)

        names_count = struct.unpack('<Q', f.read(8))[0]
        out_data += struct.pack('<Q', names_count)

        print(f'Found {names_count} names')
        names = []

        for i in range(names_count):
            name_length = struct.unpack('<Q', f.read(8))[0]
            out_data += struct.pack('<Q', name_length)

            name = f.read(name_length)
            names.append(name.decode('utf-8'))
            out_data += name

        sz = struct.unpack('<Q', f.read(8))[0]
        assert sz == names_count
        out_data += struct.pack('<Q', sz)

        for i in range(sz):
            array_header = struct.unpack('<Q', f.read(8))[0]
            assert array_header == kTVMNDArrayMagic
            out_data += struct.pack('<Q', array_header)

            array_reserved = struct.unpack('<Q', f.read(8))[0]
            assert array_reserved == 0
            out_data += struct.pack('<Q', array_reserved)

            print(f'Parsing array {names[i]}')

            array_device_type = struct.unpack('<I', f.read(4))[0]
            print(f'\tdevice_type {array_device_type}')

            # We patch this to be 12 instead
            out_data += struct.pack('<I', 12)

            array_device_num = struct.unpack('<I', f.read(4))[0]
            print(f'\tdevice_num {array_device_num}')
            out_data += struct.pack('<I', array_device_num)

            array_ndim = struct.unpack('<I', f.read(4))[0]
            print(f'\tndim {array_ndim}')
            out_data += struct.pack('<I', array_ndim)

            array_dtype_code = struct.unpack('<B', f.read(1))[0]
            print(f'\tdtype_code {array_dtype_code}')
            out_data += struct.pack('<B', array_dtype_code)

            array_dtype_bits = struct.unpack('<B', f.read(1))[0]
            print(f'\tdtype_bits {array_dtype_bits}')
            out_data += struct.pack('<B', array_dtype_bits)

            array_dtype_lanes = struct.unpack('<H', f.read(2))[0]
            print(f'\tdtype_lanes {array_dtype_lanes}')
            out_data += struct.pack('<H', array_dtype_lanes)

            array_elem_bytes = int((array_dtype_bits + 7) / 8)
            array_num_elems = 1
            print(f'\tdim ', end='')
            for i in range(array_ndim):
                dim = struct.unpack('<Q', f.read(8))[0]
                print(f'{dim} ', end='')
                out_data += struct.pack('<Q', dim)
                array_num_elems *= dim

            print(f'')

            array_data_size = struct.unpack('<Q', f.read(8))[0]
            print(f'\tdata_size {array_data_size}')
            assert array_data_size == array_elem_bytes * array_num_elems
            out_data += struct.pack('<Q', array_data_size)

            out_data += f.read(array_data_size)

    print('done parsing')
    with open(ofile, 'wb+') as f:
        f.write(out_data)


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("usage: fixup_params.py <params.bin> <params_patched.bin>")
        exit(-1)

    fixup(sys.argv[1], sys.argv[2])
