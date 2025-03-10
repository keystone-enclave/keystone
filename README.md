# Keystone: An Open-Source Secure Enclave Framework for RISC-V Processors

![Documentation Status](https://readthedocs.org/projects/keystone-enclave/badge/)
[![Build Status](https://travis-ci.org/keystone-enclave/keystone.svg?branch=master)](https://travis-ci.org/keystone-enclave/keystone/)
[![OpenSSF Best Practices](https://www.bestpractices.dev/projects/8916/badge)](https://www.bestpractices.dev/projects/8916)

> Visit [Project Website](https://keystone-enclave.org) for more information.

## Introduction

Keystone is an open-source project that builds trusted execution environments (TEEs) for RISC-V systems. Its hardware-enforced and software-defined memory isolation enables trusted computing (a.k.a. confidential computing) with various threat models and functionalities. The implementation is platform-agnostic, making Keystone portable across different RISC-V platforms with minimal engineering efforts.


## Goals

Keystone is a free and open framework for architecting and deploying TEEs on RISC-V hardware platforms. The project's goals are:

* **Enable TEE on (almost) all RISC-V processors**: Keystone aims to support as many RISC-V processor cores that follow RISC-V standard ISA and sub-ISAs as possible. This will help hardware designers and manufacturers to enable TEE with minimal efforts.

* **Make TEE easy to customize depending on needs**: while providing simple TEE features, Keystone also aims to allow various customization that depends on platform-specific features or non-standard sub-ISAs. We borrow the concept from software-defined network, where hardware platform provides *primitives* and the software leverages the primitives to implement specific functionalities or meet security requirements.

* **Reduce the cost of building TEE**: Keystone aims to reduce the cost of building TEE or TEE-based systems. We achieve this by reusing the implementation across multiple different platforms, reducing hardware integration cost, reducing verification cost, and integrating with existing software tools. We hope that anyone can simply extend Keystone to build their own novel TEE design with very low cost.


## Status

Keystone started as an academic project that helps researchers to build and test their ideas. 
Now, Keystone is an **Incubation Stage** open-source project of the Confidential Computing Consortium (CCC) under the Linux Foundation. 

Keystone has helped many researchers focus on their creative ideas instead of building TEE by themselves from scratch.
This resulted in many innovative research projects and publications, which have been pushing the technical advancement of TEEs.

We are currently trying to make Keystone production-ready. You can find the latest general roadmap of Keystone [here](https://docs.google.com/document/d/1AxT0w6NCtfvZcFE1wbZAkAODftqRYhpHaj63mvnQQqA/edit?usp=sharing)

Here are some ongoing and/or planned efforts towards the goal:

* **Technical Improvements**: Make Keystone more usable and on par with existing industry solutions, including memory isolation improvement, better application and hardware support, and additional features.

* **Parity with Industry Standards**: Make Keystone follow the industry standard. This includes standard cryptography, measured boot, and remote attestation protocols. 

* **Hardware Integration**: Partner with RISC-V hardware designer/vendor to fully integrate with the hardware. This includes integration with hardware root-of-trust, memory encryption engine, and crypto accelerators.

## Documentation

See [docs](http://docs.keystone-enclave.org) for getting started.

## Hardware Support

Keystone requires a standard RISC-V platform with a *hardware root of trust* --- including secure key storage and measured boot. Currently, no hardware root of trust has been designed or manufactured specifically for Keystone. If you have a open-source root-of-trust we'd love to integrate with it!

As this project focuses more on the software stack and the toolchain, you can still run the full Keystone software stack on top of a few RISC-V platforms without a real root-of-trust. See https://github.com/keystone-enclave/keystone/tree/master/sm/plat for the supported platforms. In general, `generic` should work with most of the standard RISC-V cores as long as they support:

- RV64 with SV39 addressing mode (or RV32 with SV32)
- M/S/U privilege modes
- More than 4 PMP registers

For full security, platform architect needs to provide the followings

- Entropy source (and ideally a platform specific random number generator)
- Measured boot
- Secure on-chip key storage

Keystone doesn’t provide high-performance hardware-based memory encryption, as it requires a proprietary memory controller. Instead, it provides an example software-based encryption, which uses scratchpad SRAM (if any) to encrypt physical pages.

## Team

Contributors

- Gregor Haas
- Evgeny Pobachienko
- Jakob Sorensen
- David Kholbrenner
- Alex Thomas
- Cathy Lu
- Gui Andrade
- Kevin Chen
- Stephan Kaminsky
- Dayeol Lee (Maintainer)

Advisors

- David Kohlbrenner @ UW
- Shweta Shinde @ ETH Zurich
- Krste Asanovic @ UCB
- Dawn Song @ UCB

## License

Keystone is under BSD-3.

## Contributing

See CONTRIBUTING.md

## Citation

If you want to cite the project, please use the following bibtex:

```
@inproceedings{lee2019keystone,
    title={Keystone: An Open Framework for Architecting Trusted Execution Environments},
    author={Dayeol Lee and David Kohlbrenner and Shweta Shinde and Krste Asanovic and Dawn Song},
    year={2020},
    booktitle = {Proceedings of the Fifteenth European Conference on Computer Systems},
    series = {EuroSys’20}
}
```
