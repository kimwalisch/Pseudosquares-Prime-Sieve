#!/bin/bash

# Usage: scripts/build_mingw64_x64.sh
# Builds a Pseudosquares Prime Sieve release binary that is statically
# linked and ready for distribution.

# === Prerequisites x64 ===
# 1) Install MSYS2 x64
# 2) pacman -Syu (exit then run it again)
# 3) pacman -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake zip unzip git wget
# 4) git clone https://github.com/kimwalisch/Pseudosquares-Prime-Sieve.git
# 5) scripts/build_mingw64_x64.sh

# Exit if any error occurs
set -e

####################################################################

handle_error() {
    echo ""
    echo "Error: $1"
    exit 1
}

rm -rf build build-release

FULL_DATE=$(date +'%B %d, %Y')
YEAR=$(date +'%Y')

# The program version is printed by version() in src/main.cpp.
VERSION=$(sed -n 's/.*pseudosquares_prime_sieve \([0-9][0-9.]*\)".*/\1/p' src/main.cpp | head -n 1)
[ -n "$VERSION" ] || handle_error "failed detecting version in src/main.cpp"

TEMPLATE_VERSION=1.0
TEMPLATE_ZIP=Pseudosquares-Prime-Sieve-$TEMPLATE_VERSION-win-x64.zip
RELEASE_DIR=Pseudosquares-Prime-Sieve-$VERSION-win-x64
TMP_DIR=$RELEASE_DIR-tmp

####################################################################

# Fetch hurchalla_util. The direct g++ commands below include headers
# from build/_deps/hurchalla_util-src/include, which CMake's
# FetchContent step populates while configuring modular_arithmetic.
cmake -S . -B build
[ -d build/_deps/hurchalla_util-src/include ] || handle_error "failed fetching hurchalla_util"

# Build release binary ##############################################

mkdir build-release
cd build-release

g++ -std=gnu++17 -static -O3 -mpopcnt -flto -DNDEBUG -D_WIN32_WINNT=0x0A00 -Wall -Wextra -pedantic -DENABLE_MULTIARCH_AVX512_BW -DENABLE_MULTIARCH_AVX512_VBMI2 \
    -I../src \
    -I../lib/primesieve/include \
    -I../lib/primesieve/src \
    -I../lib/modular_arithmetic/modular_arithmetic/include \
    -I../lib/modular_arithmetic/montgomery_arithmetic/include \
    -I../build/_deps/hurchalla_util-src/include \
    ../lib/primesieve/src/*.cpp \
    ../lib/primesieve/src/arch/x86/*.cpp \
    ../src/pseudosquares_prime_sieve.cpp \
    ../src/CmdOptions.cpp \
    ../src/main.cpp \
    -o pseudosquares_prime_sieve.exe

strip pseudosquares_prime_sieve.exe

# Build and run release tests #######################################

g++ -std=gnu++17 -static -O3 -mpopcnt -flto -DNDEBUG -D_WIN32_WINNT=0x0A00 -Wall -Wextra -pedantic -DENABLE_MULTIARCH_AVX512_BW -DENABLE_MULTIARCH_AVX512_VBMI2 \
    -I../src \
    -I../lib/primesieve/include \
    -I../lib/primesieve/src \
    -I../lib/modular_arithmetic/modular_arithmetic/include \
    -I../lib/modular_arithmetic/montgomery_arithmetic/include \
    -I../build/_deps/hurchalla_util-src/include \
    ../lib/primesieve/src/*.cpp \
    ../lib/primesieve/src/arch/x86/*.cpp \
    ../src/pseudosquares_prime_sieve.cpp \
    ../src/tests.cpp \
    -o tests.exe

./tests.exe

echo ""
echo ""

# Create a release zip archive ######################################

wget https://github.com/kimwalisch/Pseudosquares-Prime-Sieve/releases/download/v$TEMPLATE_VERSION/$TEMPLATE_ZIP
unzip "$TEMPLATE_ZIP" -d "$TMP_DIR"
rm "$TEMPLATE_ZIP"

[ -f "$TMP_DIR/pseudosquares_prime_sieve.exe" ] || handle_error "release template is missing pseudosquares_prime_sieve.exe"
[ -f "$TMP_DIR/README.txt" ] || handle_error "release template is missing README.txt"
[ -f "$TMP_DIR/LICENSE" ] || handle_error "release template is missing LICENSE"

echo ""
echo ""
echo "Old file size: $(ls -l --block-size=K "$TMP_DIR/pseudosquares_prime_sieve.exe")"
echo "New file size: $(ls -l --block-size=K pseudosquares_prime_sieve.exe)"
echo ""
echo ""

mv -f pseudosquares_prime_sieve.exe "$TMP_DIR"
cd "$TMP_DIR"

# README.txt line 1 contains the title and version, while line 2
# contains the release date. Preserve the existing title text.
README_TITLE=$(sed -n '1p' README.txt)
[[ "$README_TITLE" == *"$TEMPLATE_VERSION" ]] || handle_error "unexpected README.txt version line"
README_TITLE="${README_TITLE%$TEMPLATE_VERSION}$VERSION"
sed -i "1 s/.*/$README_TITLE/" README.txt
sed -i "2 s/.*/$FULL_DATE/" README.txt

if [ "$YEAR" = "2025" ]; then
    COPYRIGHT="Copyright (c) 2025, Kim Walisch."
else
    COPYRIGHT="Copyright (c) 2025 - $YEAR, Kim Walisch."
fi
sed -i "3 s/.*/$COPYRIGHT/" LICENSE

# Verify sed has worked correctly
[ "$(sed -n '1p' README.txt)" = "$README_TITLE" ] || handle_error "failed updating README.txt version"
[ "$(sed -n '2p' README.txt)" = "$FULL_DATE" ] || handle_error "failed updating README.txt date"
[ "$(sed -n '3p' LICENSE)" = "$COPYRIGHT" ] || handle_error "failed updating LICENSE"

./pseudosquares_prime_sieve.exe -v

echo ""
echo ""

# Build release zip archive #########################################

zip "$RELEASE_DIR.zip" pseudosquares_prime_sieve.exe README.txt LICENSE
mv "$RELEASE_DIR.zip" ..
cd ..
mv "$TMP_DIR" "$RELEASE_DIR"

####################################################################

echo "Release binary built successfully!"
cd ..
