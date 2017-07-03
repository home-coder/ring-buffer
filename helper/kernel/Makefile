#ARCH=armh
#ARCH=armc
ARCH=x86c
ifeq ($(ARCH), armh)
    export ARCH=arm
    export CROSS_COMPILE=/home/java/basic/cross_compile/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/bin/arm-fsl-linux-gnueabi-
    LINUX_PATH :=/home/java/opt/hhzn/kernel_imx
endif

ifeq ($(ARCH), armc)
    export ARCH=arm64
    export CROSS_COMPILE=/opt/toolchain/mstar/linaro-aarch64_linux-2014.09_843419-patched/bin/aarch64-linux-gnu-
    LINUX_PATH :=/home/jiangxiujie/mstar-828-tv/Mstar-828/vendor/mstar/kernel/3.10.40
endif

ifeq ($(ARCH), x86c)
    LINUX_PATH :=/usr/src/linux-headers-4.4.0-75-generic
endif
obj-m += pages.o

all:
	make -C $(LINUX_PATH) M=`pwd` modules
clean:
	make -C $(LINUX_PATH) M=`pwd` modules clean
