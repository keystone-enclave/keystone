# riscv-pk 
echo "riscv-pk: cherry-picking changes required for FireSim"
git submodule update --init -- riscv-pk
cd riscv-pk
git remote add firesim https://github.com/firesim/riscv-pk/
git fetch firesim
git cherry-pick -n e5846a2bc707eaa58dc8ab6a8d20a090c6ee8570..cad3deb357d25773a22e2c346ef464d3d66dd37c
cd ..

# linux v5.3
echo "linux: cherry-picking changes required for FireSim"
git submodule update --init -- linux
cd linux
git remote add firesim https://github.com/firesim/linux
git fetch firesim
git cherry-pick -n 4d856f72c10ecb060868ed10ff1b1453943fc6c8..6a95b016aba5ef275f3bb6541e90b5ab9042c853
cd ..
