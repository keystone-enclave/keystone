How to Contribute
=================

Keystone is, and will be, fully open source. We hope to see many
projects both under the Keystone name, and built on-top of it.

We welcome contributions at all levels, from platform specific
bootloader modifications, to improved compilers and toolchains, to
support for novel use-cases for end users.

Platform Specific Builds/Changes
--------------------------------

Many SoCs and boards will require some amount of custom support for
Keystone to function fully. If you are interested in ensuring Keystone
operates correctly on your target there are several requirements:

 - PMP support
 - RISCV priv-1.10 ISA support
 - Entropy sources
 - >=3-run level support (U/S/M)

Where possible, changes that allow Keystone to operate on another
platform should be cross-platform themselves, to avoid fragmentation.

See also the list of known areas to work on, we need better platform
specific builds!

General Contributions
---------------------

Submit pull-requests, with a good explanation, and following the
coding guidelines.
