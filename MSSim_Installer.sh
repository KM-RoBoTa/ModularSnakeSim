#!/bin/sh

clear 
echo "Modular Snake Sim install script"
echo "-----------------------------"
echo " "
echo "Press [Enter] key to continue."
read enter

# prerequisites
sudo apt-get update
sudo apt-get install build-essential libode-dev libopenscenegraph-dev openscenegraph freeglut3 freeglut3-dev libjpeg-dev libpng-dev libqt4-dev

# download source
# it is suppose that you already clone the "ModularSnakeSim" reposritory to the desired installing path

# build
qmake-qt4 -Wnone src/Melosimula.pro -r -spec linux-g++
make -w
make clean -w
rm Makefile

clear 
echo "Modular Snake Sim install script"
echo "-----------------------------"
echo " "
echo "Modular Snake Sim has been succesfully installed"
echo " "
echo "Press [Enter] key to exit."
read enter

exit 0
