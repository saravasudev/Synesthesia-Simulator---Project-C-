#!/bin/bash
set -e

sudo apt update
sudo apt install -y libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-gfx-dev

if [ ! -d "extern/cxxtest-4.4" ]; then
    mkdir -p extern
    wget https://github.com/CxxTest/cxxtest/releases/download/4.4/cxxtest-4.4.tar.gz
    tar -xzf cxxtest-4.4.tar.gz -C extern
    rm cxxtest-4.4.tar.gz
fi

# Patch a deprecated-enum-float-conversion warning that newer g++ treats
# as an error under -Werror, since this is vendor code we can't otherwise
# modify to satisfy our own build flags.
if ! grep -q "(double)BASE" extern/cxxtest-4.4/cxxtest/ValueTraits.cpp; then
    sed -i 's/t \/= BASE/t \/= (double)BASE/' extern/cxxtest-4.4/cxxtest/ValueTraits.cpp
    sed -i 's/(t \*= BASE)/(t *= (double)BASE)/' extern/cxxtest-4.4/cxxtest/ValueTraits.cpp
fi