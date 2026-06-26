# qmamba

![logo](mamba.webp)

## Introduction

qmamba is a package manager for kdb-x, built using [libmamba](https://mamba.readthedocs.io/en/latest/index.html).

It enables users to download and install kdb-x modules from inside a q session with a simple command.  The modules can be sourced from the KX channel as prebuilt packages and qmamba will automatically fetch any dependencies (such as third party libraries) from other channels such as conda-forge.

It also supports having multiple independent environments into which packages are installed and allows the user to switch between these environments.



## Installation

It is recommended that qmamba is installed using one of the release packages.

### Requirements

- kdb+ ≥ 5.0 64-bit (currently only linux is supported but macOS and Windows are planned)

### Installing a release using a package

1. [Download a release](https://github.com/KxSystems/qmamba/releases) for your OS and system architecture.
2. Unzip the download.
3. From the extracted `qmamba/` directory, run the installation script:

   ```bash
   ## Linux/macOS
   ## By default kdb-x is installed to ~/.kx
   ./install.sh <path to kdb-x installation>
   
   ## Windows
   install.bat <path to kdb-x installation>
   ```


### Installing a release by building from source

1. Follow the instructions to [build qmamba from source](./BUILDING.md).
2. Run the installation script (same as above) from the package install directory that was created by the build.



## Quick links

:point_right: [Function reference](docs/reference.md)



## Documentation

The complete set of documentation is split over over several pages for ease of readability.  Users new to qmamba should read them in order.

* [Introduction](docs/introduction.md) to qmamba
* Basic [concepts](docs/concepts.md) such as packages, channels and environments
* [Getting started](docs/getting-started.md) with a tour of the basic functionality
* [Learning more](docs/learning-more.md)  about qmamba and how to use it
* [Function reference](docs/reference.md) including options, shortcuts and a simple example for each API
* [Troubleshooting](docs/troubleshooting.md) issues
* [Further information](docs/further-information.md) and advanced topics



## Status

The qmamba interface is provided here under an [Apache 2.0 license](./LICENSE).  It is built using [libmamba](https://github.com/mamba-org/mamba), its licenses are included in the [licenses](./licenses/) folder.

If you find issues with the interface or have feature requests, please consider [raising an issue](https://github.com/KxSystems/qmamba/issues).

If you wish to contribute to this project, please follow the [contribution guide](CONTRIBUTING.md).
