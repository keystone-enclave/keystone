.. 

Running Keystone in RV32
--------------------------------------

Currently we support running Keystone using all three priviledges, which are Machine, Supervisor, and User mode (M/S/U). Currently, we are working on a Machine and User mode \
implementation on Keystone to better support embedded systems. 

Setup RV32 Environment
#############################

First, run the ``rv32-setup.sh`` script (located at the top-level directory) to install the RV32 toolchain (which uses the GC extensions). 

::

  ./rv32-setup.sh
  
This script will unzip and install the RV32 toolchain and store it in ``./riscv32``. The script will also set the ``$RISCV`` environment variable to point to the ``riscv32`` \
directory. We strongly recommend adding the following to your ``.bashrc``.

::

  export RISCV=$(pwd)/riscv32
  export PATH=$RISCV/bin:$PATH

Build SDK in RV32
#############################

We will now have to build the SDK in RV32. Go to your SDK directory and create a build directory. Remember to set your SDK path

::

  mkdir <build directory>
  cd <build directory>
  export KEYSTONE_SDK_DIR=<install_directory>
  cmake .. -DRISCV32=y 
  make
  make install
  
This will build the SDK in RV32. 

Build Keystone in RV32
#############################

Similar to the SDK, we may now build the entire Keystone framework in RV32. First, create a build directory and specify the RV32 flag.

::

  mkdir <build directory>
  cd <build directory>
  cmake .. -DRISCV32=y 
  make

This will begin building Keystone in RV32. 
