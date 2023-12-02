git submodule update --init --recursive --depth 1
echo "please read this file for further instructions"

### INSTRUCTIONS
# make -j 12 # change 12 to desired parallelism
### find port in command from make, 9821 at the time of writing
# make run
### switch terminals
# scp -i build-generic64/overlay/root/.ssh/id-rsa -P <PORT_NUMBER> build-generic64/buildroot.build/build/keystone-*/*.ko root@localhost:.
# scp -i build-generic64/overlay/root/.ssh/id-rsa -P <PORT_NUMBER> build-generic64/buildroot.build/build/keystone-examples-*/*/*.ke root@localhost:.
