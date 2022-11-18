#!/usr/bin/env bash
# this script is automatically run from CMakeLists.txt

BUILD_ROOT=$PWD
BUILD_INTERACTIVE=$1

if [ -f /.dockerenv ]; then
	BUILD_CONTAINER="YES"
else
	BUILD_CONTAINER="NO"
fi


echo "[Pre-build]  dependency installer script running..."
echo "[Pre-build]  build root directory: $BUILD_ROOT"
echo "[Pre-build]  build interactive:    $BUILD_INTERACTIVE"
echo "[Pre-build]  build container:      $BUILD_CONTAINER"
echo " "


# detect build architecture
ARCH=$(uname -i)

# break on errors
#set -e


# docker doesn't use sudo
if [ $BUILD_CONTAINER = "YES" ]; then
	SUDO=""
else
	SUDO="sudo"
fi
	
	
# install packages
$SUDO apt-get update
$SUDO apt-get install -y dialog
$SUDO apt-get install -y libglew-dev glew-utils libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libglib2.0-dev
$SUDO apt-get install -y qtbase5-dev

if [ $ARCH != "x86_64" ]; then
	# on x86, these are already installed by conda and installing them again creates conflicts
	$SUDO apt-get install -y libpython3-dev python3-numpy
fi

$SUDO apt-get update


# download/install models and PyTorch
if [ $BUILD_CONTAINER = "NO" ]; then
	./download-models.sh $BUILD_INTERACTIVE
	./install-pytorch.sh $BUILD_INTERACTIVE
else
	# in container, the models are mounted and PyTorch is already installed
	echo "Running in Docker container => skipping model downloads";
fi


echo "[Pre-build]  Finished CMakePreBuild script"
