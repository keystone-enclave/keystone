# Keystone Eyrie Modular Runtime

Eyrie only builds as part of the Keystone [sdk](https://github.com/keystone-enclave/keystone-sdk).

We strongly encourage using the top-level [Keystone](https://github.com/keystone-enclave/keystone) build process.

# Compatibility

| Name         | Version        |
|--------------|----------------|
| Keystone SDK | v1.0 or higher |
| Keystone SM  | v1.0 or higher |

# Building

## Building the Eyrie Runtime

Make sure you've properly set the environment variable `KEYSTONE_SDK_DIR` to point to the Keystone SDK installation path.

Then, run `./build.sh [features]`.

## Running the tests

Make sure you checked out all submodules with `git submodule update --init`.

Then, run `make test`.

If a test fails and you'd like more detail, enter into `obj/test` and run the binary for the failed test. e.g. if `test_string` fails, run `obj/test/test_string`.

## Build options

See the sdk Makefile for feature selection.

# Contributing

The Eyrie Runtime is licensed under the 3-clause BSD license. See LICENSE for more details.

Before submitting a pull request to GitHub, make sure you format your code first.

```sh
make format
```
