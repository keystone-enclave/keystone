# Static Compilation of Python

## Background

The most widely used Python Interpreter is CPython, written in C and compiled. Python libraries dealing with a lot of data generally have a backend written in C and the Python part of the library simply interfaces with that code. This C code is generally a shared object.

To create a static program, we need the following compiled to RISC-V binary:
1. A Python interpreter.
    - Also the static (`.a` not `.so`) versions generic libraries it uses -- i.e. pthread, libc.
2. Python libraries
    - Static versions of their backends.
    - Static versions of the dependencies of their backends -- i.e. some installations of Numpy may use math libraries.

These are all the pieces of static binary that we need, but we also need to combine them. For this, we need a Python packaging or "freezing" tool.

## Python packaging tools

Most are purpose-built for specific platforms, and will not work for us since RISC-V is currently obscure. We need a less end-to-end tool that gets the Python interpreter embedded in some language, which we can then use a compiler on to get to RISC-V.

These tools generally just copy paste the entire interpreter and libraries and run Python code normally.

### [Cython --embed](https://cython.readthedocs.io/en/latest/src/tutorial/embedding.html) (not to be confused with the CPython interpreter).

- Cython is not primarily a packaging tool. Does not have great support for properly packaging and Python-importing all libraries used by the Python code being packaged.
- Written in Python, meaning its native functionality is equivalent on RISC-V and anything else!
- Embeds into the C language.

### [PyOxidizer](https://pyoxidizer.readthedocs.io/en/stable/index.html)

- Actively developed for this purpose, supports packaging Python libraries, has documentation for how to add libraries not automatically detected.
- Written in Rust, so it needs
- Embeds into Rust.
    - Technically, you can cross-compile Rust but cross-compilation is very bad and should be avoided. Also, Rust std lib may not be fully implemented for some targets.
- Supposed to use a custom build of the CPython interpreter.

## Challenges

This project has to overcome 2 unpopular tasks: compiling to RISC-V, and *statically* packaging Python. This means that it is impossible to find end-to-end tools, or even already-compiled libraries. Build systems on Arm/ x86 can be already surprisingly complicated and unstable, and more-so for static compilation and for RISC-V targets, if available at all. Also, there's the issue of getting static versions of system libraries.

An uninvestigated challenge is incompatibility of something like libc between build process and the Enclave target, as well as possible syscall issues.

## Approach 1: Native compilation in Fedora on RISC-V QEMU

### Accomplishes

Dynamically-linked RISC-V executable, with copies of the needed shared object libraries. This runs in the linux (not enclave) of the Enclave QEMU.

Staticically-compiled CPython interpreter.

### Tips

These commands assume using a linux docker image (I have disk space to waste and find it nice to effectively checkpoint environment changes).

This is different from the RISC-V linux built for the Enclave because this is a full Fedora distribution with all the accompanying packages, and a package manager.

Note: using `vim`, `nano`, etc. inside QEMU inside docker inside WSL2 on Windows broke the number & size of lines displayed and made the viewing area small.

Lastly, I used root inside QEMU but now I think non-root is better.

### Copying files

With full linux QEMU:
```bash
scp -P 3333 riscv@localhost:<qemu file location> <host file location>
```
Port 3333 is because that's what the QEMU launch script uses. Password is '`fedora_rocks!`'.

With Enclave QEMU:
```bash
cd /keystone/build
cp -r /keystone/<your folder>/* ./overlay/root/
make image
```

### Useful commands

```bash
file <you file name> # see file type (RISC-V or x86/ etc, static or dynamic, etc.)
ldd <your executable> # see what libraries it links
```

### `pip install` change

I needed to tell `pip install` to trust the websites hosting python libraries. If facing the same issue, replace each use of `pip install` with `pip install --trusted-host pypi.org --trusted-host pypi.python.org --trusted-host files.pythonhosted.org`.

### [QEMU Setup](https://embeddedinn.xyz/articles/tutorial/Linux-Python-on-RISCV-using-QEMU-from-scratch/)

```bash
mkdir -p /keystone/risclinux
cd /keystone/risclinux

apt update
apt install libguestfs-tools

mkdir -p ~/.config/virt-builder/repos.d/

cat <<EOF > ~/.config/virt-builder/repos.d/fedora-riscv.conf
[fedora-riscv]
uri=https://dl.fedoraproject.org/pub/alt/risc-v/repo/virt-builder-images/images/index
EOF

wget https://dl.fedoraproject.org/pub/alt/risc-v/repo/virt-builder-images/images/Fedora-Developer-Rawhide-20200108.n.0-sda.raw.xz
unxz -k Fedora-Developer-Rawhide-20200108.n.0-sda.raw.xz

wget https://dl.fedoraproject.org/pub/alt/risc-v/repo/virt-builder-images/images/Fedora-Developer-Rawhide-20200108.n.0-fw\_payload-uboot-qemu-virt-smode.elf

# any RISC-V QEMU should be fine
cat <<"EOF" > ./qemu_start.sh
export RISC_LINUX_VER=20200108.n.0
/keystone/qemu/riscv64-softmmu/qemu-system-riscv64 -machine virt \
                    -nographic \
                    -smp 4 \
                    -m 8G \
                    -kernel Fedora-Developer-Rawhide-${RISC_LINUX_VER}-fw_payload-uboot-qemu-virt-smode.elf \
                    -object rng-random,filename=/dev/urandom,id=rng0 \
                    -device virtio-rng-device,rng=rng0 \
                    -device virtio-blk-device,drive=hd0 \
                    -drive file=Fedora-Developer-Rawhide-${RISC_LINUX_VER}-sda.raw,format=raw,id=hd0 \
                    -device virtio-net-device,netdev=usernet \
                    -netdev user,id=usernet,hostfwd=tcp::3333-:22
EOF

chmod +x ./qemu_start.sh
```

### Fix DNS in QEMU

```bash
rm /etc/resolv.conf
cat <<EOF > /etc/resolv.conf
# File from command copied from STATIC_PYTHON.md, because QEMU doesn't set up DNS correctly 
nameserver 1.1.1.1
EOF
```

### Cython setup

Ideally, we would do this:

```bash
pip install --trusted-host pypi.org --trusted-host pypi.python.org --trusted-host files.pythonhosted.org cython
dnf install python-devel # package manager fails to do this
```

Instead, we need to compile the CPython interpreter (nothign to do with Cython) to get the C headers needed for embedding the Python interpreter.
```bash
pip install --trusted-host pypi.org --trusted-host pypi.python.org --trusted-host files.pythonhosted.org cython

mkdir -p /keypy/cpython_interpreter
cd /keypy/cpython_interpreter
git clone --single-branch -b 3.8 https://github.com/python/cpython.git
```

We could compile this normally and package static versions of libraries later.
```bash
./configure
make
make test
sudo make install
```

Instead, I did this (although this may not be the best choice):
[Compile statically](https://stackoverflow.com/questions/1150373/compile-the-python-interpreter-statically). It proved to have some issues, and the interpreter should be missing some features, but I got a **static** python interpreter out of it. However, when I tried doing the next section, static linking failed, but dynamic linking ran. See the `./Modules/Setup` file I used in [Appendix > Setup file](#setup-file-for-cpython-static-compilation).

### Compile with Cython

Note that this is general usage of Cython, and you can attempt to use cross-compilation here (not recommended). You would need to get static RISC-V builds of all of the dependencies (Python interpreter, Python libraries, system libraries, etc.).

```bash
mkdir -p /keypy/cyth
cd /keypy/cyth
echo "print('Hello World')" > hello.py
cython -3 --embed -o hello.c hello.py

# dynamic build, works
gcc -Os `python3-config --cflags --ldflags` hello.c -o hello -lpython3.8
## python3-config --cflags --ldflags expanded in my environment:
#-I/usr/local/include/python3.8 -I/usr/local/include/python3.8  -Wno-unused-result -Wsign-compare  -DNDEBUG -g -fwrapv -O3 -Wall
#-L/usr/local/lib/python3.8/config-3.8-riscv64-linux-gnu -L/usr/local/lib  -lcrypt -lpthread -ldl  -lutil -lm -lm
## some sources suggest additionally doing: -lz -lexpat -lc

# static build, doesn't work due to missing static versions of system libraries
gcc -static -Os `python3-config --cflags --ldflags` hello.c -o hello -lpython3.8

# this looks useless, don't remember where I got this from
gcc -I/usr/include/python3.8 -Wno-unused-result -Wsign-compare  -O2 -g -pipe -Wall -Werror=format-security -Wp,-D_FORTIFY_SOURCE=2 -Wp,-D_GLIBCXX_ASSERTIONS -fexceptions -fstack-protector-strong -grecord-gcc-switches   -fasynchronous-unwind-tables -fstack-clash-protection -D_GNU_SOURCE -fPIC -fwrapv  -DNDEBUG  -O2 -g -pipe -Wall -Werror=format-security -Wp,-D_FORTIFY_SOURCE=2 -Wp,-D_GLIBCXX_ASSERTIONS -fexceptions -fstack-protector-strong -grecord-gcc-switches   -fasynchronous-unwind-tables -fstack-clash-protection -D_GNU_SOURCE -fPIC -fwrapv -L/usr/lib64  -lcrypt -lpthread -ldl  -lutil -lm hello.c -o hello -lpython3.8
```

### Bonus: easy dynamic-linked executable with [PyInstaller](https://pyinstaller.org)

Note that this tool does not support static compilation.

In full linux QEMU:

```bash
pip install --trusted-host pypi.org --trusted-host pypi.python.org --trusted-host files.pythonhosted.org pyinstaller

mkdir -p /keypy/pyinst
cd /keypy/pyinst
echo "print('Hello World')" > hello.py
pyinstaller hello.py
./dist/hello/hello # sanity check the executable
```

In Enclave QEMU:
```bash
cd ~/dist/hello
LD_LIBRARY_PATH=. ./hello
```

## Approach 2: Cross-compiling with PyOxidizer

Note: you can alternatively try to get PyOxidizer working on RISC-V, which may or may not make things easier.

```bash
apt update
apt install llvm-9
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh

rustup toolchain install nightly
rustup +nightly component add rust-src
rustup +nightly target add riscv64gc-unknown-none-elf
cargo +nightly install cargo-xbuild

# python3.8 is officially supported
apt install python3.8
apt install python3-pip
python3.8 -m pip install --upgrade pip setuptools wheel
python3.8 -m pip install pyoxidizer

mkdir -p /keystone/keypy/pyoxi/hello_app
cd /keystone/keypy/pyoxi
echo "print('hello world')" >> hello_app/hello.py
pyoxidizer init-config-file hello_app
pyoxidizer init-rust-project hello_app
cd hello_app

# todo get cpython interpreter risc-v binary
# todo add cpython binary to a target to pyoxidizer cfg file

# todo fix -- one of the std modules has a python build script that thinks it's compiling for build machine
cargo +nightly build -Zbuild-std --target riscv64gc-unknown-none-elf
rustc build.rs +nightly --target riscv64gc-unknown-none-elf
```

## Appendix

### Additional Links

- https://wiki.python.org/moin/BuildStatically
- https://stackoverflow.com/questions/5105482/compile-main-python-program-using-cython/22040484#22040484
- https://stackoverflow.com/questions/39913847/is-there-a-way-to-compile-a-python-application-into-static-binary

### Setup file for CPython static compilation

```
*static*
# -*- makefile -*-
# The file Setup is used by the makesetup script to construct the files
# Makefile and config.c, from Makefile.pre and config.c.in,
# respectively.  Note that Makefile.pre is created from Makefile.pre.in
# by the toplevel configure script.

# (VPATH notes: Setup and Makefile.pre are in the build directory, as
# are Makefile and config.c; the *.in files are in the source directory.)

# Each line in this file describes one or more optional modules.
# Modules configured here will not be compiled by the setup.py script,
# so the file can be used to override setup.py's behavior.
# Tag lines containing just the word "*static*", "*shared*" or "*disabled*"
# (without the quotes but with the stars) are used to tag the following module
# descriptions. Tag lines may alternate throughout this file.  Modules are
# built statically when they are preceded by a "*static*" tag line or when
# there is no tag line between the start of the file and the module
# description.  Modules are built as a shared library when they are preceded by
# a "*shared*" tag line.  Modules are not built at all, not by the Makefile,
# nor by the setup.py script, when they are preceded by a "*disabled*" tag
# line.

# Lines have the following structure:
#
# <module> ... [<sourcefile> ...] [<cpparg> ...] [<library> ...]
#
# <sourcefile> is anything ending in .c (.C, .cc, .c++ are C++ files)
# <cpparg> is anything starting with -I, -D, -U or -C
# <library> is anything ending in .a or beginning with -l or -L
# <module> is anything else but should be a valid Python
# identifier (letters, digits, underscores, beginning with non-digit)
#
# (As the makesetup script changes, it may recognize some other
# arguments as well, e.g. *.so and *.sl as libraries.  See the big
# case statement in the makesetup script.)
#
# Lines can also have the form
#
# <name> = <value>
#
# which defines a Make variable definition inserted into Makefile.in
#
# The build process works like this:
#
# 1. Build all modules that are declared as static in Modules/Setup,
#    combine them into libpythonxy.a, combine that into python.
# 2. Build all modules that are listed as shared in Modules/Setup.
# 3. Invoke setup.py. That builds all modules that
#    a) are not builtin, and
#    b) are not listed in Modules/Setup, and
#    c) can be build on the target
#
# Therefore, modules declared to be shared will not be
# included in the config.c file, nor in the list of objects to be
# added to the library archive, and their linker options won't be
# added to the linker options. Rules to create their .o files and
# their shared libraries will still be added to the Makefile, and
# their names will be collected in the Make variable SHAREDMODS.  This
# is used to build modules as shared libraries.  (They can be
# installed using "make sharedinstall", which is implied by the
# toplevel "make install" target.)  (For compatibility,
# *noconfig* has the same effect as *shared*.)
#
# NOTE: As a standard policy, as many modules as can be supported by a
# platform should be present.  The distribution comes with all modules
# enabled that are supported by most platforms and don't require you
# to ftp sources from elsewhere.


# Some special rules to define PYTHONPATH.
# Edit the definitions below to indicate which options you are using.
# Don't add any whitespace or comments!

# Directories where library files get installed.
# DESTLIB is for Python modules; MACHDESTLIB for shared libraries.
DESTLIB=$(LIBDEST)
MACHDESTLIB=$(BINLIBDEST)

# NOTE: all the paths are now relative to the prefix that is computed
# at run time!

# Standard path -- don't edit.
# No leading colon since this is the first entry.
# Empty since this is now just the runtime prefix.
DESTPATH=

# Site specific path components -- should begin with : if non-empty
SITEPATH=

# Standard path components for test modules
TESTPATH=

COREPYTHONPATH=$(DESTPATH)$(SITEPATH)$(TESTPATH)
PYTHONPATH=$(COREPYTHONPATH)


# The modules listed here can't be built as shared libraries for
# various reasons; therefore they are listed here instead of in the
# normal order.

# This only contains the minimal set of modules required to run the
# setup.py script in the root of the Python source tree.

posix -DPy_BUILD_CORE_BUILTIN -I$(srcdir)/Include/internal posixmodule.c # posix (UNIX) system calls
errno errnomodule.c                     # posix (UNIX) errno values
pwd pwdmodule.c                         # this is needed to find out the user's home dir
                                        # if $HOME is not set
_sre _sre.c                             # Fredrik Lundh's new regular expressions
_codecs _codecsmodule.c                 # access to the builtin codecs and codec registry
_weakref _weakref.c                     # weak references
_functools -DPy_BUILD_CORE_BUILTIN -I$(srcdir)/Include/internal _functoolsmodule.c   # Tools for working with functions and callable objects
_operator _operator.c                   # operator.add() and similar goodies
_collections _collectionsmodule.c       # Container types
_abc _abc.c                             # Abstract base classes
itertools itertoolsmodule.c             # Functions creating iterators for efficient looping
atexit atexitmodule.c                   # Register functions to be run at interpreter-shutdown
_signal -DPy_BUILD_CORE_BUILTIN -I$(srcdir)/Include/internal signalmodule.c
_stat _stat.c                           # stat.h interface
time -DPy_BUILD_CORE_BUILTIN -I$(srcdir)/Include/internal timemodule.c  # -lm # time operations and variables
_thread -DPy_BUILD_CORE_BUILTIN -I$(srcdir)/Include/internal _threadmodule.c    # low-level threading interface

# access to ISO C locale support
_locale -DPy_BUILD_CORE_BUILTIN _localemodule.c  # -lintl

# Standard I/O baseline
_io -DPy_BUILD_CORE_BUILTIN -I$(srcdir)/Include/internal -I$(srcdir)/Modules/_io _io/_iomodule.c _io/iobase.c _io/fileio.c _io/bytesio.c _io/bufferedio.c _io/textio.c _io/stringio.c

# faulthandler module
faulthandler faulthandler.c

# debug tool to trace memory blocks allocated by Python
#
# bpo-35053: The module must be builtin since _Py_NewReference()
# can call _PyTraceMalloc_NewReference().
_tracemalloc _tracemalloc.c hashtable.c

# The rest of the modules listed in this file are all commented out by
# default.  Usually they can be detected and built as dynamically
# loaded modules by the new setup.py script added in Python 2.1.  If
# you're on a platform that doesn't support dynamic loading, want to
# compile modules statically into the Python binary, or need to
# specify some odd set of compiler switches, you can uncomment the
# appropriate lines below.

# ======================================================================

# The Python symtable module depends on .h files that setup.py doesn't track
_symtable symtablemodule.c

# Uncommenting the following line tells makesetup that all following
# modules are to be built as shared libraries (see above for more
# detail; also note that *static* or *disabled* cancels this effect):

#*shared*

# GNU readline.  Unlike previous Python incarnations, GNU readline is
# now incorporated in an optional module, configured in the Setup file
# instead of by a configure script switch.  You may have to insert a
# -L option pointing to the directory where libreadline.* lives,
# and you may have to change -ltermcap to -ltermlib or perhaps remove
# it, depending on your system -- see the GNU readline instructions.
# It's okay for this to be a shared library, too.

#readline readline.c -lreadline -ltermcap


# Modules that should always be present (non UNIX dependent):

array arraymodule.c     # array objects
cmath cmathmodule.c _math.c # -lm # complex math library functions
math mathmodule.c _math.c # -lm # math library functions, e.g. sin()
_contextvars _contextvarsmodule.c  # Context Variables
_struct _struct.c       # binary structure packing/unpacking
_weakref _weakref.c     # basic weak reference support
#_testcapi _testcapimodule.c    # Python C API test module
_testinternalcapi _testinternalcapi.c -I$(srcdir)/Include/internal -DPy_BUILD_CORE_MODULE  # Python internal C API test module
_random _randommodule.c # Random number generator
_elementtree -I$(srcdir)/Modules/expat -DHAVE_EXPAT_CONFIG_H -DUSE_PYEXPAT_CAPI _elementtree.c  # elementtree accelerator
_pickle _pickle.c       # pickle accelerator
_datetime _datetimemodule.c     # datetime accelerator
_bisect _bisectmodule.c # Bisection algorithms
_heapq _heapqmodule.c   # Heap queue algorithm
_asyncio _asynciomodule.c  # Fast asyncio Future
_json -I$(srcdir)/Include/internal -DPy_BUILD_CORE_BUILTIN _json.c      # _json speedups
_statistics _statisticsmodule.c # statistics accelerator

unicodedata unicodedata.c    # static Unicode character database


# Modules with some UNIX dependencies -- on by default:
# (If you have a really backward UNIX, select and socket may not be
# supported...)

fcntl fcntlmodule.c     # fcntl(2) and ioctl(2)
spwd spwdmodule.c               # spwd(3)
grp grpmodule.c         # grp(3)
select selectmodule.c   # select(2); not on ancient System V

# Memory-mapped files (also works on Win32).
mmap mmapmodule.c

# CSV file helper
_csv _csv.c

# Socket module helper for socket(2)
#_socket socketmodule.c

# Socket module helper for SSL support; you must comment out the other
# socket line above, and possibly edit the SSL variable:
#SSL=/usr/local/ssl
#_ssl _ssl.c \
#       -DUSE_SSL -I$(SSL)/include -I$(SSL)/include/openssl \
#       -L$(SSL)/lib -lssl -lcrypto

# The crypt module is now disabled by default because it breaks builds
# on many systems (where -lcrypt is needed), e.g. Linux (I believe).

_crypt _cryptmodule.c # -lcrypt # crypt(3); needs -lcrypt on some systems


# Some more UNIX dependent modules -- off by default, since these
# are not supported by all UNIX systems:

#nis nismodule.c -lnsl  # Sun yellow pages -- not everywhere
#termios termios.c      # Steen Lumholt's termios module
#resource resource.c    # Jeremy Hylton's rlimit interface

#_posixsubprocess _posixsubprocess.c  # POSIX subprocess module helper

# Multimedia modules -- off by default.
# These don't work for 64-bit platforms!!!
# #993173 says audioop works on 64-bit platforms, though.
# These represent audio samples or images as strings:

#audioop audioop.c      # Operations on audio samples


# Note that the _md5 and _sha modules are normally only built if the
# system does not have the OpenSSL libs containing an optimized version.

# The _md5 module implements the RSA Data Security, Inc. MD5
# Message-Digest Algorithm, described in RFC 1321.

#_md5 md5module.c


# The _sha module implements the SHA checksum algorithms.
# (NIST's Secure Hash Algorithms.)
#_sha1 sha1module.c
#_sha256 sha256module.c
#_sha512 sha512module.c
#_sha3 _sha3/sha3module.c

# _blake module
#_blake2 _blake2/blake2module.c _blake2/blake2b_impl.c _blake2/blake2s_impl.c

# The _tkinter module.
#
# The command for _tkinter is long and site specific.  Please
# uncomment and/or edit those parts as indicated.  If you don't have a
# specific extension (e.g. Tix or BLT), leave the corresponding line
# commented out.  (Leave the trailing backslashes in!  If you
# experience strange errors, you may want to join all uncommented
# lines and remove the backslashes -- the backslash interpretation is
# done by the shell's "read" command and it may not be implemented on
# every system.

# *** Always uncomment this (leave the leading underscore in!):
# _tkinter _tkinter.c tkappinit.c -DWITH_APPINIT \
# *** Uncomment and edit to reflect where your Tcl/Tk libraries are:
#       -L/usr/local/lib \
# *** Uncomment and edit to reflect where your Tcl/Tk headers are:
#       -I/usr/local/include \
# *** Uncomment and edit to reflect where your X11 header files are:
#       -I/usr/X11R6/include \
# *** Or uncomment this for Solaris:
#       -I/usr/openwin/include \
# *** Uncomment and edit for Tix extension only:
#       -DWITH_TIX -ltix8.1.8.2 \
# *** Uncomment and edit for BLT extension only:
#       -DWITH_BLT -I/usr/local/blt/blt8.0-unoff/include -lBLT8.0 \
# *** Uncomment and edit for PIL (TkImaging) extension only:
#     (See http://www.pythonware.com/products/pil/ for more info)
#       -DWITH_PIL -I../Extensions/Imaging/libImaging  tkImaging.c \
# *** Uncomment and edit for TOGL extension only:
#       -DWITH_TOGL togl.c \
# *** Uncomment and edit to reflect your Tcl/Tk versions:
#       -ltk8.2 -ltcl8.2 \
# *** Uncomment and edit to reflect where your X11 libraries are:
#       -L/usr/X11R6/lib \
# *** Or uncomment this for Solaris:
#       -L/usr/openwin/lib \
# *** Uncomment these for TOGL extension only:
#       -lGL -lGLU -lXext -lXmu \
# *** Uncomment for AIX:
#       -lld \
# *** Always uncomment this; X11 libraries to link with:
#       -lX11

# Lance Ellinghaus's syslog module
#syslog syslogmodule.c          # syslog daemon interface


# Curses support, requiring the System V version of curses, often
# provided by the ncurses library.  e.g. on Linux, link with -lncurses
# instead of -lcurses).

#_curses _cursesmodule.c -lcurses -ltermcap
# Wrapper for the panel library that's part of ncurses and SYSV curses.
#_curses_panel _curses_panel.c -lpanel -lncurses


# Modules that provide persistent dictionary-like semantics.  You will
# probably want to arrange for at least one of them to be available on
# your machine, though none are defined by default because of library
# dependencies.  The Python module dbm/__init__.py provides an
# implementation independent wrapper for these; dbm/dumb.py provides
# similar functionality (but slower of course) implemented in Python.

#_dbm _dbmmodule.c      # dbm(3) may require -lndbm or similar

# Anthony Baxter's gdbm module.  GNU dbm(3) will require -lgdbm:

#_gdbm _gdbmmodule.c -I/usr/local/include -L/usr/local/lib -lgdbm


# Helper module for various ascii-encoders
#binascii binascii.c

# Fred Drake's interface to the Python parser
#parser parsermodule.c


# Andrew Kuchling's zlib module.
# This require zlib 1.1.3 (or later).
# See http://www.gzip.org/zlib/
#zlib zlibmodule.c -I$(prefix)/include -L$(exec_prefix)/lib -lz

# Interface to the Expat XML parser
# More information on Expat can be found at www.libexpat.org.
#
#pyexpat expat/xmlparse.c expat/xmlrole.c expat/xmltok.c pyexpat.c -I$(srcdir)/Modules/expat -DHAVE_EXPAT_CONFIG_H -DXML_POOR_ENTROPY -DUSE_PYEXPAT_CAPI

# Hye-Shik Chang's CJKCodecs

# multibytecodec is required for all the other CJK codec modules
#_multibytecodec cjkcodecs/multibytecodec.c

#_codecs_cn cjkcodecs/_codecs_cn.c
#_codecs_hk cjkcodecs/_codecs_hk.c
#_codecs_iso2022 cjkcodecs/_codecs_iso2022.c
#_codecs_jp cjkcodecs/_codecs_jp.c
#_codecs_kr cjkcodecs/_codecs_kr.c
#_codecs_tw cjkcodecs/_codecs_tw.c

# Example -- included for reference only:
# xx xxmodule.c

# Another example -- the 'xxsubtype' module shows C-level subtyping in action
xxsubtype xxsubtype.c

# Uncommenting the following line tells makesetup that all following modules
# are not built (see above for more detail).
#
#*disabled*
#
#_sqlite3 _tkinter _curses pyexpat
#_codecs_jp _codecs_kr _codecs_tw unicodedata
```
