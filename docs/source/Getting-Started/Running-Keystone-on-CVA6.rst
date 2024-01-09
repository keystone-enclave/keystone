Running Keystone on CVA6
===========================

`CVA6 <https://github.com/openhwgroup/cva6/>`_. is a 6-stage, single-issue, in-order CPU which implements the 64-bit RISC-V instruction set. 

Requirement 
-----------
Keystone has been ported so it can run on CVA6 Platform on Genesys 2 Board. It is tested on version `797f0a9 <https://github.com/openhwgroup/cva6/commit/f301d6967517336a21a58c9b8a00ea3186906c01/>`_ for both 64 bit and 32 bit. You need to synthesize cva6 and flash the it Genesys2 Board. 

Root of Trust
#############
To enable the Root of Trust for attestation report, you need to modify the CVA6 `bootrom <https://github.com/openhwgroup/cva6/tree/master/corev_apu/fpga/src/bootrom/src/>`_ code to perform TCB measurement and SM key generation. This modification is not archived in CVA6 repository. You can imitate keystone QEMU bootrom to implement the CVA6 RoT


Floating Point Support
######################

CVA6 32 bit only support F extension for floating point. Additionally, up to glibc version 2.38, F extension is not yet supported. because of that, you may not be able to use floating point or run application that requires floating point.

Building Keystone
-----------------

To build the keystone, you need to run this command

.. code-block:: bash

    KEYSTONE_PLATFORM=cva6 make

or, if you want to build the 32 bit version of keystone to run on CV32A6

.. code-block:: bash 
  
    KEYSTONE_PLATFORM=cva6 KEYSTONE_BITS=32 make


Setting up the CVA6
-------------------

Flash SD Card
################

To flash the SD card, you need to run this command

.. code-block:: bash

    sudo KEYSTONE_PLATFORM=cva6 SD_DEVICE=/dev/sdb make flash

or, for the 32 bit version

.. code-block:: bash
   
    sudo KEYSTONE_PLATFORM=cva6 KEYSTONE_BITS=32 SD_DEVICE=/dev/sdb make flash


make sure that the SD_DEVICE is set to the correct SD Card device name on your machine


Run binaries
############

Insert the Keystone driver, and run whatever test binaries you wish.

Example

.. code-block:: bash 

    insmod keystone-driver.ko
    ./tests.ke

Known Issues
------------

32 Bit Error
############

On 32 bit target, during startup, the linux log display this error

.. code-block:: bash

    Starting network: [   38.002974] lowrisc-eth 30000000.lowrisc-eth eth0: Current addr  0[   38.011773] lowrisc-eth 30000000.lowrisc-eth eth0: Expected add0  [   38.020196] ------------[ cut here ]------------
  
    [   38.026528] netdevice: eth0: Incorrect netdev->dev_addr
    [   38.034447] WARNING: CPU: 0 PID: 99 at net/core/dev_addr_lists.
    [   38.041518] Modules linked in:
    [   38.045439] CPU: 0 PID: 99 Comm: ip Not tainted 6.1.32 #2
    [   38.051668] Hardware name: eth,ariane-bare (DT)
    [   38.056825] epc : dev_addr_check+0xa0/0xf4
    [   38.061851]  ra : dev_addr_check+0xa0/0xf4
    [   38.066842] epc : c03709bc ra : c03709bc sp : c23c3d80
    [   38.072723]  gp : c1c90a60 tp : c22f2f40 t0 : 00000000
    [   38.078577]  t1 : ffffffff t2 : 00000000 s0 : c23c3da0
    [   38.084431]  s1 : c215c800 a0 : 0000002b a1 : 3fffefff
    [   38.090270]  a2 : 00000000 a3 : fffffffe a4 : 00000000
    [   38.096093]  a5 : 00000000 a6 : c1c62f88 a7 : 00000001
    [   38.101924]  s2 : c215c800 s3 : c22aedf4 s4 : 00000000
    [   38.107763]  s5 : 00000000 s6 : c215c97c s7 : 00000000
    [   38.113592]  s8 : c21bac00 s9 : 00000000 s10: 9d430e68
    [   38.119428]  s11: 00000008 t3 : 00000000 t4 : 00000000
    [   38.125249]  t5 : 00000001 t6 : 00000000
    [   38.136288] [<c03709bc>] dev_addr_check+0xa0/0xf4use: 00000003
    [   38.141970] [<c036dc3a>] __dev_open+0x2c/0x12c
    [   38.147345] [<c036e06c>] __dev_change_flags+0x146/0x1b0
    [   38.153558] [<c036e0f2>] dev_change_flags+0x1c/0x4c
    [   38.159369] [<c03f2934>] devinet_ioctl+0x1bc/0x4dc
    [   38.165230] [<c03f4174>] inet_ioctl+0x104/0x136
    [   38.170637] [<c034a648>] sock_ioctl+0x22a/0x26e
    [   38.176201] [<c00df3de>] sys_ioctl+0x6e/0x7c
    [   38.181487] [<c0003182>] ret_from_syscall+0x0/0x2
    [   38.187090] ---[ end trace 0000000000000000 ]---
    [   38.210424] Open device, request interrupt 3
    udhcpc: started, v1.36.0

And when you run the enclave application, such as hello-native, you will see the error log as below

.. code-block:: bash

  # /usr/share/keystone/examples/hello-native.ke
  /dev/console
  Verifying archive integrity... MD5 checksums are OK. All good.
  Uncompressing Keystone Enclave Package
  [ 2334.409841] rcu: INFO: rcu_sched self-detected stall on CPU
  [ 2334.416433] rcu:     0-...!: (2100 ticks this GP) idle=a6e4/1/0x402
  [ 2334.423348]    (t=2100 jiffies g=9217 q=24 ncpus=1)
  [ 2334.428853] rcu: rcu_sched kthread timer wakeup didn't happen f 
  [ 2334.435566] rcu:  Possible timer handling issue on cpu=0 timer-
  [ 2334.442017] rcu: rcu_sched kthread starved for 2100 jiffies! g9T
  [ 2334.448709] rcu:     Unless rcu_sched kthread gets sufficient CPU e
  [ 2334.455251] rcu: RCU grace-period kthread stack dump:
  [ 2334.460899] task:rcu_sched       state:I stack:0     pid:12    0
  [ 2334.467886] Call Trace:
  [ 2334.470930] [<c04bbb1e>] __schedule+0x1ca/0x46c
  [ 2334.476485] [<c04bbdfa>] schedule+0x3a/0xba
  [ 2334.481575] [<c04c0418>] schedule_timeout+0x62/0xc4
  [ 2334.487504] [<c00589a0>] rcu_gp_fqs_loop+0x238/0x2a0
  [ 2334.493394] [<c005a32a>] rcu_gp_kthread+0x5a/0xd4
  [ 2334.498992] [<c0024c80>] kthread+0x8c/0x98
  [ 2334.504146] [<c0003190>] ret_from_exception+0x0/0x16
  [ 2334.510048] rcu: Stack dump where RCU GP kthread last ran:
  [ 2334.516218] CPU: 0 PID: 190 Comm: hello-native-ru Tainted: G   3
  [ 2334.522896] Hardware name: eth,ariane-bare (DT)
  [ 2334.528052] epc : __do_softirq+0xb8/0x25a
  [ 2334.533035]  ra : irq_exit+0x8c/0xbc
  [ 2334.537500] epc : c04c1660 ra : c000f266 sp : c23b7b70
  [ 2334.543396]  gp : c1c90a60 tp : c0810000 t0 : 00046000
  [ 2334.549285]  t1 : 00002382 t2 : c0000000 s0 : c23b7bd0
  [ 2334.555163]  s1 : c1cbdd58 a0 : 00000000 a1 : c104fcc0
  [ 2334.561036]  a2 : 00030f7d a3 : 00030f7e a4 : c7afacc0
  [ 2334.566906]  a5 : c104fcc0 a6 : 70a61351 a7 : 2c010000
  [ 2334.572787]  s2 : c1cbdd58 s3 : c23b7c00 s4 : 00000000
  [ 2334.578657]  s5 : c1c05080 s6 : c104e1e0 s7 : c104e170
  [ 2334.584537]  s8 : 95709030 s9 : 00000000 s10: c1050000
  [ 2334.590404]  s11: 00000008 t3 : 000000a1 t4 : 00000014
  [ 2334.596258]  t5 : a402303c t6 : a4023044
  [ 2334.607380] [<c04c1660>] __do_softirq+0xb8/0x25aause: 80000005
  [ 2334.613023] [<c000f266>] irq_exit+0x8c/0xbc
  [ 2334.618117] [<c04bafb2>] generic_handle_arch_irq+0x64/0x74
  [ 2334.624537] [<c0003190>] ret_from_exception+0x0/0x16
  [ 2334.630814] CPU: 0 PID: 190 Comm: hello-native-ru Tainted: G   2
  [ 2334.637579] Hardware name: eth,ariane-bare (DT)
  [ 2334.642798] epc : __do_softirq+0xb8/0x25a
  [ 2334.647783]  ra : irq_exit+0x8c/0xbc
  [ 2334.652209] epc : c04c1660 ra : c000f266 sp : c23b7b70
  [ 2334.658114]  gp : c1c90a60 tp : c0810000 t0 : 00046000
  [ 2334.664003]  t1 : 00002382 t2 : c0000000 s0 : c23b7bd0
  [ 2334.669881]  s1 : c1cbdd58 a0 : 00000000 a1 : c104fcc0
  [ 2334.675752]  a2 : 00030f7d a3 : 00030f7e a4 : c7afacc0
  [ 2334.681625]  a5 : c104fcc0 a6 : 70a61351 a7 : 2c010000
  [ 2334.687493]  s2 : c1cbdd58 s3 : c23b7c00 s4 : 00000000
  [ 2334.693356]  s5 : c1c05080 s6 : c104e1e0 s7 : c104e170
  [ 2334.699233]  s8 : 95709030 s9 : 00000000 s10: c1050000
  [ 2334.705097]  s11: 00000008 t3 : 000000a1 t4 : 00000014
  [ 2334.710953]  t5 : a402303c t6 : a4023044
  [ 2334.722077] [<c04c1660>] __do_softirq+0xb8/0x25aause: 80000005
  [ 2334.727704] [<c000f266>] irq_exit+0x8c/0xbc
  [ 2334.732791] [<c04bafb2>] generic_handle_arch_irq+0x64/0x74
  [ 2334.739199] [<c0003190>] ret_from_exception+0x0/0x16
  Enclave said: "Hello World"

It's not known yet the root cause of the issue but one possible cause is some application using floating point trigger the error.


Network Issue
#############

During startup, the DHCP client more often than not is unable to get IP address from DHCP server. You need to run the DHCP client manually after boot and it will be able to get the IP address and connect to the network

.. code-block:: bash

  # udhcpc
  udhcpc: started, v1.36.0
  udhcpc: broadcasting discover
  udhcpc: broadcasting select for 192.168.1.4, server 192.168.1.1
  udhcpc: lease of 192.168.1.4 obtained from 192.168.1.1, lease time 86400
  deleting routers
  adding dns 192.168.1.1

