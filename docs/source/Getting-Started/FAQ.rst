FAQ
===

 Q: What can I run Keystone on?
    - A: Keystone can be run on real RISC-V hardware, on FPGA, and in
      simulation. See `Getting Started <#>`_

 Q: What is the difference between the SM and runtime?
    - A: The SM is part of the Keystone TCB, and is trusted by all
      components. The runtime is a (technically) optional part of the
      enclave itself. While the enclave app trusts it, it is not part
      of the trusted boot process and is not part of the Keystone TCB.

 Q: Why are enclaves/SM/etc written in C? Why not Rust (or another modern language)?
    - A: Rust RV64 support was unavailable when Keystone was
      started. Few options for the security monitor besides C were
      available. We are keeping a close eye on Rust support as it
      matures for RV64, and expect to support it for enclaves at a
      minimum.
