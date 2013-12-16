#!/bin/bash
make distclean
# Directory "extras" must be available in top kernel directory. Sign scripts must also be available at ~/.gnome2/nautilus-scripts/SignScripts/
# Edit CROSS_COMPILE to mirror local path. Edit "version" to any desired name or number but it cannot have spaces. 
pwd=`readlink -f .`
export CROSS_COMPILE=/home/jdsingle76/S3Kernel/kernel-extras/arm-eabi-4.6/bin/arm-eabi-
export ARCH=arm
export version=AGAT_JB

# Determines the number of available logical processors and sets the work thread accordingly
export JOBS="(expr 4 + $(grep processor /proc/cpuinfo | wc -l))"

loc=~/.gnome2/nautilus-scripts/SignScripts/
date=$(date +%Y%m%d-%H:%M:%S)

# Check for a log directory in ~/ and create if its not there
[ -d ~/logs ] || mkdir -p ~/logs

# Setting up environment
rm out -R
mkdir -p out
cp -r ../kernel-extras/mkboot $pwd
cp -r ../kernel-extras/zip $pwd

# Build entire kernel and create build log
make m2_spr_defconfig
make headers_install
# make modules
time make -j8 CC="ccache home/jdsingle76/S3Kernel/kernel-extras/arm-eabi-4.6/bin/arm-eabi-" 2>&1 | tee ~/logs/$version.txt

echo "making boot image"
cp arch/arm/boot/zImage mkboot/
cd mkboot
./img-l710-jb-test.sh
cd ..

echo "making signed zip"
rm -rf zip/$version
mkdir -p zip/$version
mkdir -p zip/$version/system/lib/modules

# Find all modules that were just built and copy them into the working directory
find -name '*.ko' -exec cp -av {} $pwd/zip/$version/system/lib/modules/ \;
mv mkboot/boot.img zip/$version
cp -R zip/META-INF-l710-jb zip/$version
cd zip/$version
mv META-INF-l710-jb META-INF
zip -r ../tmp.zip ./
cd ..
java -classpath "$loc"testsign.jar testsign "tmp.zip" "$version"-"$date"-signed.zip
rm tmp.zip
mv *.zip ../out
echo "Popped kernel available in the out directory"
echo "Build log is avalable in ~/logs"
echo "Cleaning kernel directory"
# Clean up kernel tree
cd $pwd
rm -rf mkboot 
rm -rf zip
echo "Done"

geany ~/logs/$version.txt || exit 1
