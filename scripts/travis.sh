# Build everything
./fast-build.sh


# Launch QEMU test
screen -dmS qemu ./scripts/run-qemu.sh
sleep 10
./scripts/test-qemu.sh

diff output.log tests/test-qemu.expected.log
if [ $? -eq 1 ]
then
  echo "[FAIL] output.log does not match with the expected output"
  return 1
else
  echo "[PASS] output.log matches with the expected output"
  return 0
fi
