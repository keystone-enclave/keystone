FAQ
===

 Q: What is the difference between the SM and runtime?
    - A: The SM is part of the Keystone TCB, and is trusted by all
      components. The runtime is a (technically) optional part of the
      enclave itself. While the enclave app trusts it, it is not part
      of the trusted boot process and is not part of the Keystone TCB.
