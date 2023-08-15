#!/bin/bash

modprobe keystone-driver
sysctl -w vm.max_map_count=6553000
./vta-runner vta-enclave eyrie-rt