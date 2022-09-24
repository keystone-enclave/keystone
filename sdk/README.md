## Build SDK and Install

Replace `<install directory>` with any path that you want SDK to be installed.
If not specified, the install directory will be the same as the build directory.
`<install directory>` MUST be different from the source path.

```bash
mkdir build
cd build
export KEYSTONE_SDK_DIR=<install_directory>
cmake ..
make
make install
```

To uninstall,
```bash
make uninstall
```

Ideally, you may need to add the following to your shell rcfile (e.g., .bashrc)

```bash
export KEYSTONE_SDK_DIR=<install directory>
```

## Build Examples

In the CMake build directory, run the following:

```bash
make examples
```

This will compile example enclaves in `examples` directory.
All the example enclave packages have suffix `.ke`.

## Format Checking

All code should be formatted correctly before they are merged.

```
make format
```

We use `clang-format` and `cpplint` for checking format.
`clang-format` auto-formats the code in `src`, `include`, `tests`, and `examples` directories.
`cpplint` will check C++ format (based on Google C++ coding style) and print out warnings if there are ill-formatted files.

## Run Unit Tests

The Keystone SDK uses GoogleTest as a framework to run various tests. The test framework source code is located under `tests`.

Before you can run the tests, you must install the GoogleTest libaries.
There is a setup script under `tests/scripts/setup_test.sh` which installs the GoogleTest source code and builds the library.

The unit tests will be running on your host machine (not RISC-V QEMU).

In order to run the unit tests, do the following:

```
make check
```
