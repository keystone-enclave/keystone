# Keystone: An Open-Source Secure Enclave Framework for RISC-V Processors

![Documentation Status](https://readthedocs.org/projects/keystone-enclave/badge/)
[![Build Status](https://travis-ci.org/keystone-enclave/keystone.svg?branch=master)](https://travis-ci.org/keystone-enclave/keystone/)

> Visit [Project Website](https://keystone-enclave.org) for more information.

## Introduction

Keystone is an open-source project that builds trusted execution environments (TEEs) based on RISC-V. Its hardware-enforced and software-defined memory isolation enables trusted computing (a.k.a. confidential computing) with various threat models and functionalities. The implementation is platform-agnostic, making Keystone portable across different RISC-V platforms with minimal engineering efforts.

Keystone started as an academic project that helps researchers to build and test their ideas. Now, Keystone is a part of Confidential Computing Consortium (CCC) under Linux Foundation. We are currently trying to make Keystone industry-ready.

You can find the latest general roadmap of Keystone [here](https://docs.google.com/document/d/1E-982564GvOcWzdCqM7TXCJV_7uWy2F8NiwglWorjFA/edit#heading=h.xa3pe84ubay4)

## Documentation

See [docs](http://docs.keystone-enclave.org) for getting started.

## Hardware Support

Keystone requires standard RISC-V platform with *hardware root of trust*, which includes a secure key storage and measured boot. Currently, no hardware root of trust has been designed or manufactured specifically for Keystone. We hope to integrate Keystone with a standard root-of-trust when it becomes available.

As this project focuses more on the software stack and the toolchain, you can still run the full Keystone software stack on top of a few RISC-V platforms. See https://github.com/keystone-enclave/keystone/tree/master/sm/plat for the supported platforms. In general, `generic` should work with most of the standard RISC-V cores as long as they support:

- RV64 with SV39 addressing mode (or RV32 with SV32)
- M/S/U privilege modes
- More than 4 PMP registers

For full security, platform architect needs to provide the followings

- Entropy source (random number generator)
- Measured boot
- Secure on-chip key storage

Keystone doesn’t provide high-performance hardware-based memory encryption, as it requires a proprietary memory controller. Instead, it provides software-based encryption, which uses scratchpad SRAM (if any) to encrypt physical pages.

## Team

Active Contributors

- Gregor Haas
- Evgeny Pobachienko
- Jakob Sorensen

Past Contributors

- Dayeol Lee
- David Kholbrenner
- Alex Thomas
- Cathy Lu
- Gui Andrade
- Kevin Chen
- Stephan Kaminsky

Maintainers

- Dayeol Lee
- Gregor Haas

Active Academic Advisors

- David Kohlbrenner
- Shweta Shinde
- Krste Asanovic
- Dawn Song

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
