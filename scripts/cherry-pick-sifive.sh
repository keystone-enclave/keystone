# riscv-pk 
#echo "[riscv-pk] cherry-picking changes required for FireSim"
#git submodule update --init -- riscv-pk
#cd riscv-pk
#if ! git remote | grep firesim_cherry_pick > /dev/null; then
#	echo "adding remote ..."
#	git remote add firesim_cherry_pick https://github.com/firesim/riscv-pk/
#fi
#git fetch firesim_cherry_pick
#git cherry-pick -n e5846a2bc707eaa58dc8ab6a8d20a090c6ee8570..cad3deb357d25773a22e2c346ef464d3d66dd37c
#echo "done"
#cd ..

# linux v5.3
echo "[linux] cherry-picking changes required for FireSim"
git submodule update --init -- linux
cd linux
if ! git remote | grep firesim_cherry_pick > /dev/null; then
	echo "adding remote ..."
	git remote add firesim_cherry_pick https://github.com/firesim/linux
fi
git fetch firesim_cherry_pick
git cherry-pick -n 4d856f72c10ecb060868ed10ff1b1453943fc6c8..6a95b016aba5ef275f3bb6541e90b5ab9042c853
echo "done"
cd ..
