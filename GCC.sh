#!/bin/bash

echo "Compile: 32/64-bit [86/64] "
read bit

if [$bit != "32"] ; then
	if [$bit != "64"] ; then
		echo "Invalid architecture"
		exit
	fi
fi

echo "Compile for this processor type: [Y/N]"
read optimise

user_flags=""
cpp_flags=""
c_flags=""

if [$optimise == Y]
	$user_flags += "-march=native"
fi

$user_flags += " -m" + $bit

path=/home/joshua/Desktop/Protheus
SDL2=$path/SDL2
SDLN=$path/SDL2_net
SDLI=$path/SDL2_image
lib=$path/lib 

#define the standards used
$c_flags += " -std=c99"
$cpp_flags += " -std=c++11" 
 
# SDL lib
gcc -c $SDL2/src/SDL.c -I $SDL2/include -o $lib/SDL2$bit.o $c_flags $user_flags
ar rcs $lib/libSDL2$bit.a $lib/SDL2$bit.o

# SDL net lib

gcc -c $SDLN/SDLnet.c -o $lib/SDL2_net$bit.o -I $SDL2/include $c_flags $user_flags
ar rcs $lib/libSDL2_net$bit.a $lib/SDL2_net$bit.o

# SDL image lib

gcc -c $SDLI/SDL_image.h -I $SDLI/external/include -I $SDL2/include -o $lib/SDL2_image$bit.o $c_flags $user_flags
ar rcs $lib/libSDL2_image$bit.a $lib/SDL2_image$bit.o

# Protheus
$Protheus_lib= "-L$lib -lSDL2_image$bit -lSDL2$bit -lSDL2_net$bit"
$Protheus_out= "-o $path/Protheus"
$Protheus_main= "$path/scr/Engine/Main.cpp"
g++ $Protheus_main $Protheus_out $Protheus_lib $cpp_flags $user_flags
