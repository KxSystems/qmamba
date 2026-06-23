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

This will build `libmamba-static` from source and build `qmamba.so` using that.  

The package is then created in the install directory  `./build/install`.


