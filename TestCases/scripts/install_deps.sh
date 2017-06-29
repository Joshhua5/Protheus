#! /bin/bash

echo "Installing dependencies, G++-4.9, OpenAL and OpenGL/glew"
#Required for G++ 4.9
#sudo add-apt-repository ppa:ubuntu-toolchain-r/test

sudo apt-get update

sudo apt-get install build-essential libglew-dev libopenal-dev valgrind
