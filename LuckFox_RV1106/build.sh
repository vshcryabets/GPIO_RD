TOOLCHAIN_BIN=/sdk/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin
SYSROOT=/sdk/sysdrv/source/buildroot/buildroot-2023.02.6/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot
CC=$TOOLCHAIN_BIN/arm-rockchip830-linux-uclibcgnueabihf-g++

SRC=./testGpio/src/testLibGpiodv2.cpp
OUT=testGpio_static

echo "Link path = $SYSROOT/lib"

$CC $SRC -o $OUT \
  --sysroot=$SYSROOT \
  -I./testGpio/include \
  -I$SYSROOT/usr/include \
  -L$SYSROOT/usr/lib \
  -L$SYSROOT/lib \
  -lgpiod