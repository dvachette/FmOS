#!/bin/bash
set -e

# Defining variables
PREFIX="$(pwd)/opt/cross"
TARGET="x86_64-elf"
PATH="$PREFIX/bin:$PATH"

# Étape 1 : Downloading sources
wget https://ftp.gnu.org/gnu/binutils/binutils-2.41.tar.gz
wget https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.gz

tar -xvf binutils-2.41.tar.gz
tar -xvf gcc-13.2.0.tar.gz

# Étape 2 : Compiling Binutils
mkdir build-binutils
cd build-binutils
../binutils-2.41/configure --target=$TARGET --prefix=$PREFIX --with-sysroot --disable-nls --disable-werror
make -j$(nproc)
make install
cd ..

# Étape 3 : Compiling GCC (C and C++)
mkdir build-gcc
cd build-gcc
../gcc-13.2.0/configure --target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc -j$(nproc)
make all-target-libgcc -j$(nproc)
make install-gcc
make install-target-libgcc
cd ..

# Cleanup
rm -rf binutils-2.41 gcc-13.2.0 build-binutils build-gcc
rm binutils-2.41.tar.gz gcc-13.2.0.tar.gz