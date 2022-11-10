How to Contribute
================================

Keystone is, and will be, fully open source. We hope to see many
projects both under the Keystone umbrella, and built on-top of it.

We welcome contributions at all levels, from platform specific
bootloader modifications, to make compilers and toolchains for programming enclaves, to
support for novel use-cases for end users.


Known Issues and Ongoing Projects
---------------------------------

Known issues are updated in `github issues <https://github.com/keystone-enclave/keystone/issues>`_.
We also post ongoing projects on `github projects <https://github.com/keystone-enclave/keystone/projects>`_.
If you have any issues or project ideas, please post it to the github issue page.

Platform Specific Builds/Changes
--------------------------------

Many SoCs and boards will require some amount of custom support for
Keystone to fully function. If you are interested in ensuring Keystone
operates correctly on your target there are several requirements:

 - RISC-V priv-1.10 ISA support (including PMP)
 - Entropy sources
 - 3 privilege level support (U/S/M)

Where possible, changes that allow Keystone to operate on another
platform should be cross-platform themselves, to avoid fragmentation.

See also the list of known areas to work on, we need better platform
specific builds!

General Contributions
--------------------------------

General workflow is as follows:

 - Fork ``keystone`` repo
 - Create a branch (ex: ``dev-cool-feature``)
 - PR your branch to ``keystone`` on the ``dev`` branch
 - Make sure all CI tests pass
 - Wait for reviews

If you are modifying a submodule:

 - Fork ``$submodule`` repo
 - Create a branch (ex: ``keystone-another-fix``)
 - PR your branch to ``$submodule`` on ``keystone`` branch
 - Wait for reviews
