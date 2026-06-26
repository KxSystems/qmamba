# Micromamba

`qmamba` offers functionality to install kdb-x modules from a q prompt. 
Other mamba clients exist such as [micromamba](https://mamba.readthedocs.io/en/latest/) that allow you to do the same using the micromamba CLI.

## Install

Install [Micromamba](https://mamba.readthedocs.io/en/latest/index.html):
```Bash
bash <(curl -L micro.mamba.pm/install.sh)
```
Create a new environment for testing, e.g. `test`:
```Bash
micromamba create --name test
micromamba activate test
```

## Download
Install the kdb-x module from the package server. 
```Bash
micromamba install -c kx -c conda-forge -c nodefaults q-kx-printf
```

:warning: Be sure to include `nodefaults` in the channels list to prevent use of the [Anaconda defaults channels](./troubleshooting.md#warning--libmamba-repoanacondacom-a-commercial-channel-hosted-by-anacondacom-is-used)

List of packages installed

```bash
micromamba list | grep kx
  q-kx-printf              0.0.1         h9bf148f_0        http://anaconda.org/kx/
```

Search for available kdb-x modules and list their dependencies.  All kdb-x modules are prefixed with `q-` so you can limit the results by searching for packages matching `q-*`:
```bash
$ micromamba search -c kx q-kx* -c conda-forge -c nodefaults --recursive
```

:warning: Be sure to include `nodefaults` in the channels list to prevent use of the [Anaconda defaults channels](./troubleshooting.md#warning--libmamba-repoanacondacom-a-commercial-channel-hosted-by-anacondacom-is-used)

### Use

Set `LD_LIBRARY_PATH` and `QPATH` and launch kdb-x.

```bash
LD_LIBRARY_PATH=$CONDA_PREFIX/lib/ QPATH=$CONDA_PREFIX/lib/q/mod:$HOME/.kx/mod/ $HOME/.kx/bin/q
```
Load the module
```
q)printf:use`kx.printf
```
