#!/bin/bash

source /ledgerdev/virtualenv/bin/activate

export APP_DIR="/ledger-app-hive/"
export BOLOS_SDK="/ledgerdev/nanos-secure-sdk"
export BOLOS_ENV="/ledgerdev/bolosenv/"
export GCCPATH=/opt/bolos/gcc-arm-none-eabi-5_3-2016q1
export CLANGPATH=/opt/bolos/clang-arm-fropi

build() {
	cd $APP_DIR
	make clean && make
}

cd $BOLOS_SDK
case $1 in
	"1.4")
		git checkout nanos-1423 && build
		;;
	"1.5")
		git checkout nanos-1553 && build
		;;
	"1.6")
		git checkout nanos-og-1601 && build
		;;
	*)
		echo "usage: bash build-hive.sh version - [1.4, 1.5, 1.6]"
		;;
esac
