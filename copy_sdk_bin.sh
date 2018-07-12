echo "Copying binary $1"
mkdir -p /tmp/busybear
sudo mount busybear-linux/busybear.bin /tmp/busybear
sudo cp sdk/$1 /tmp/busybear/root
sudo umount /tmp/busybear
rmdir /tmp/busybear
