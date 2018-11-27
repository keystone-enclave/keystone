Future Objectives and Features for Keystone
===========================================

This is a list of larger features or changes that Keystone may
need. These are not bugs.

Entropy
-------

Secure cryptography requires a dependable entropy source.  All
platforms that have such a hardware source should integrate it as a
platform-specific SM feature.

Other platforms will need to use more intrusive entropy gathering
strategies. See the Linux jitter entropy source, or the Welcome to the
Entropics_ paper.


Multi-threading models
----------------------

Formal Verification
-------------------

Task/Message Queueing for the Keystone Runtime
----------------------------------------------

Edge compiler and DSL/Toolchain
-------------------------------

Similar to SGX, we need tools to help generate code for edgecalls
between the enclave and host. Currently all such code is manually
generated.


Misc
----

 - Better ELF entry point detection and handling (more configurable)
 - Better/more complex ELF loading for eapps
 - Scheduling interfaces for closer runtime/os collaboration
 - New shared-memory usage models

.. _Entropics: https://cseweb.ucsd.edu/~dkohlbre/papers/earlyentropy.pdf
