#!/bin/bash

echo "Compile: 32/64-bit [86/64] "
read bit

if [$bit != "32" | $bit != "64"] ; then
	echo "Invalid architecture"
	exit
fi

path=/home/joshua/Desktop/Protheus
SDL2=$path/SDL2
SDLN=$path/SDL2_net
SDLI=$path/SDL2_image
lib=$path/lib
cstd=c99
cppstd=c++11

# SDL lib
gcc -c $SDL2/src/SDL.c -I $SDL2/include -o $lib/SDL2$bit.o -std=$cstd -m$bit
ar rcs $lib/libSDL2$bit.a $lib/SDL2$bit.o

# SDL net lib

gcc -c $SDLN/SDLnet.c -o $lib/SDL2_net$bit.o -I $SDL2/include -std=$cstd -m$bit
ar rcs $lib/libSDL2_net$bit.a $lib/SDL2_net$bit.o

# SDL image lib

gcc -c $SDLI/SDL_image.h -I $SDLI/external/include -I $SDL2/include -o $lib/SDL2_image$bit.o -std=$cstd -m$bit
ar rcs $lib/libSDL2_image$bit.a $lib/SDL2_image$bit.o

# Protheus
g++ $path/scr/Engine/Main.cpp -std=$cppstd -L$lib -lSDL2_image$bit -lSDL2$bit -lSDL2_net$bit -o $path/Protheus -m$bit
