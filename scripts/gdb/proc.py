import gdb

import glob
import os

builddir = os.environ['BUILDROOT_BUILDDIR'] + '/build'
imagedir = os.environ['BUILDROOT_BUILDDIR'] + '/images'

class ProcLoadSymbols(gdb.Command):
	def __init__(self):
		super(ProcLoadSymbols, self).__init__("proc-load-symbols", gdb.COMMAND_NONE)

	def invoke(self, arg, from_tty):
		found_files = {}

		with open('/tmp/proc.txt') as f:
			for line in f:
				line_spl = line.split()
				if len(line_spl) != 6:
					continue

				name = line_spl[5]
				if name in ['[heap]', '[vvar]', '[vdso]', '[stack]']:
					continue

				addr_low = int('0x' + line_spl[0].split('-')[0], 16)
				if name not in found_files:
					found_files[name] = addr_low
				else:
					if addr_low < found_files[name]:
						found_files[name] = addr_low

		for name, addr in found_files.items():
			fname = os.path.basename(name)
			print('searching for', fname)
			files = glob.glob(builddir + '/**/' + fname, recursive=True)

			if len(files) != 1:
				print(f'Ambiguous symbol file {fname} {files}')
				continue

			gdb.execute(f'add-symbol-file -o {addr} {files[0]}')

ProcLoadSymbols()
