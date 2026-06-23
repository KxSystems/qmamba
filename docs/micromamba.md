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
micromamba install -c kx q-kx-printf
```

List of packages installed
```bash
micromamba list | grep kx
  q-kx-printf              0.0.1         h9bf148f_0        http://anaconda.org/kx/
```

Search for available packages and list their dependencies
```bash
$ micromamba search -c kx kx* --recursive
```

### Use

Set `QPATH` and launch kdb-x.

```bash
LD_LIBRARY_PATH=$CONDA_PREFIX/lib/ QPATH=$CONDA_PREFIX/q/mod:$HOME/.kx/mod/ $HOME/.kx/bin/q
```
Load the module
```
q)printf:use`kx.printf
```
