#!/bin/bash

# Download micromamba
rm -rf bin root mamba build
curl -Ls https://micro.mamba.pm/api/micromamba/linux-64/latest | tar -xvj bin/micromamba

# Create and activate an environment containing the libmamba dependencies
mkdir root
export MAMBA_ROOT_PREFIX=$(pwd)/root 
bin/micromamba create -n mamba_static -f conda_env/environment-micromamba-static.yml -c conda-forge -c nodefaults -y
eval "$(bin/micromamba shell hook --shell bash)"
micromamba activate mamba_static

# Build libmamba-static
git clone https://github.com/mamba-org/mamba.git
cd mamba
git checkout 2.1.0
git apply ../patches/16-06-2026.diff
mkdir build
cd build
mkdir install
cmake .. -DBUILD_LIBMAMBA=ON -DBUILD_MAMBA=OFF -DBUILD_MICROMAMBA=OFF -DBUILD_SHARED=OFF -DBUILD_STATIC=ON -DCMAKE_INSTALL_PREFIX=$(pwd)/install
cmake --build . --parallel 6
cmake --build . --target install
export LIBMAMBA_INSTALL=$(pwd)/install
cd ../..

# Build qmamba
mkdir build
cd build
mkdir install 
cmake .. -DLIBMAMBA_INSTALL=$LIBMAMBA_INSTALL -DCMAKE_INSTALL_PREFIX=$(pwd)/install
cmake --build . --parallel 6
cmake --build . --target install
echo "qmamba installation packages created in: $(pwd)/install"
cp qmamba.so ..
cd ..

# Deactivate the micromamba environment
micromamba deactivate
