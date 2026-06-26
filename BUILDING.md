# Building from source

Clone the repo:

```bash
git clone git@gitlab.com:kxdev/kdbx/qmamba.git
cd qmamba
```

From the root of repo run:

```bash
./build_linux.sh
```

This will build `libmamba-static` from source and build qmamba using that.  

The package is then created in the install directory under `$GITHUB_ROOT/build/install/qmamba` from where the install script can be run.


