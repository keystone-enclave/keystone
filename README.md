# Keystone SDK

Build with `make`. Currently does not support any options.

## Runtimes

Runtimes are independent git repos in the ``rts/`` directory. They are
_not_ submodules of the sdk, and should be versioned and managed
independently.

## Testing the SDK 

The Keystone SDK uses GoogleTest as a framework to run various tests. The test framework source code is located under sdk/tests. 

Before you can run the tests, you must install the GoogleTest libaries. There is a setup script under `tests/scripts/setup_test.sh` which installs the GoogleTest source code and builds the library. 

In order to run the test framework, do the following:
1) Run `make` in the tests directory in the SDK. This will generate a `test-runner` executable to run the test framework.
2) Run `./test-runner`. This will execute all tests. 

## Build SDK

```bash
mkdir build
cmake .. -DOUTPUT_DIR=$(pwd)/../lib
make
make install
```

