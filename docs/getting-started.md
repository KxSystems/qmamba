# Getting started



## Preparation

A few concepts are extensively used in the subsequent documentation.  If you haven't already done so you should get familiar with them first by reading the [Concepts](concepts.md) page.



## Setup

1. Start kdb-x and load the qmamba module:

   ```
   ~$ ~/qx/bin/q
   KDB-X 5.0 2026.04.01 Copyright (C) 1993-2026 Kx Systems
   l64/ 12()core 5925MB neal a-lptp-2vccmh4k 127.0.1.1 NONEXPIRE  nmcdonnell@kx.com COMMUNITY #5034242
   
   Welcome to KDB-X Community Edition!
   For Community support, please visit https://kx.com/slack
   Tutorials can be found at https://github.com/KxSystems/tutorials
   Ready to go beyond the Community Edition? Email preview@kx.com
   
   q)qmamba:use `kx.qmamba
   ```

2. On first use qmamba will create the root prefix (called [base](docs/concepts.md#baseenvironment)) inside the kdb-x installation, install some basic packages into it and tell you how to start q such that it picks these up:

   ```
   q)qmamba:use `kx.qmamba
   
   This is the first time you have used qmamba, initialising.
   
   Creating root prefix "/home/neal/qx/root"
   
   Installing libgcc, libstdcxx, openssl, libcurl, curl, libxml2 < 2.14 from conda-forge into root prefix "/home/neal/qx/root"...
   Done
   
   Please exit q then start it with (set an alias):
   
   LD_LIBRARY_PATH=/home/neal/qx/root/lib /home/neal/qx/bin/q
   ```

   Doing this maximises compatibility with other conda packages, including the kdb-x modules.

   It is recommended that you setup an alias for this q startup command but should you forget and load qmamba, it will detect this and warn you.



## Quick tour

1. Restart q as suggested, load qmamba and list the available environments.  Initially this will only show the base environment which was created:

   ```
   ~$ LD_LIBRARY_PATH=/home/neal/qx/root/lib /home/neal/qx/bin/q
   KDB-X 5.0 2026.04.01 Copyright (C) 1993-2026 Kx Systems
   l64/ 12()core 5924MB neal a-lptp-2vccmh4k 127.0.1.1 NONEXPIRE  nmcdonnell@kx.com COMMUNITY #5034242
   
   Welcome to KDB-X Community Edition!
   For Community support, please visit https://kx.com/slack
   Tutorials can be found at https://github.com/KxSystems/tutorials
   Ready to go beyond the Community Edition? Email preview@kx.com
   
   q)qmamba:use `kx.qmamba
   q)qmamba.env.list[]
     Name  Active  Path
   ────────────────────────────────────────────────────────────────
     base  *       /home/neal/qx/root
   ```

   Note: if you have used other mamba or conda tools before, any environments created by them may also be listed.

2. Create a new environment called `test`.  This will be created under the root environment:

   ```
   q)qmamba.create "test"
   Empty environment created at prefix: /home/neal/qx/root/envs/test
   q)qmamba.env.list[]
     Name  Active  Path
   ──────────────────────────────────────────────
     base  *       /home/neal/qx/root
     test          /home/neal/qx/root/envs/test
   ```

3. Activate the `test` environment.  This means that any subsequent qmamba operations are applied by default to this environment.  It also sets the kdb-x [search path](https://code.kx.com/kdb-x/modules/module-framework/quickstart.html#search-path) (`.Q.m.SP`) to look in this environment when loading a module.

   ```
   q)qmamba.activate "test"
   Activated environment at prefix: /home/neal/qx/root/envs/test
   q)qmamba.env.list[]
     Name  Active  Path
   ──────────────────────────────────────────────
     base          /home/neal/qx/root
     test  *       /home/neal/qx/root/envs/test
   q).Q.m.SP
   "/home/neal/qx/root/envs/test/q/mod"
   "/home/neal/qx/mod"
   ```

4. Say you are looking for a protobuf integration.  All kdb-x modules are prefixed with `q-` so you can limit the results by searching for packages matching `q-*protobuf*` in the `kx` channel:

   ```
   q)qmamba.search (`SPECS`CHANNEL`PRETTY)!(enlist "q-*protobuf*";enlist "kx";1)
   Getting repodata from channels...
   
        q-kx-protobuf 2.0.0 hb0f4dca_0
   ────────────────────────────────────────
   
    Name            q-kx-protobuf
    Version         2.0.0
    Build           hb0f4dca_0
    Size            2822 kB
    License
    Subdir          linux-64
    File Name       q-kx-protobuf-2.0.0-hb0f4dca_0.conda
    URL             https://conda.anaconda.org/kx/linux-64/q-kx-protobuf-2.0.0-hb0f4dca_0.conda
    MD5             087c83e6a36eef3c8b6532c21c4f2133
    SHA256          d4200c7015c1fafd8fd1ed9ea706ae1e261a2a910f5b53f45949412342e7037a
   
    Dependencies:
     - libgcc >=15
     - libprotobuf 3.*
     - libstdcxx >=15
     - libprotobuf >=3.21.12,<3.22.0a0
   ```

6. Install `q-kx-protobuf`.  Since it has third party dependencies such as `libprotobuf`, we add `conda-forge` to the channel list and qmamba will use that to fetch these dependencies, with `q-kx-protobuf` coming from the `kx`  channel.

   ```
   q)qmamba.install (`SPECS`CHANNEL)!(enlist "q-kx-protobuf";("kx";"conda-forge"))
   conda-forge/noarch                                  26.8MB @   3.3MB/s  8.1s
   conda-forge/linux-64                                54.9MB @   5.1MB/s 10.7s
   
   
   Transaction
   
     Prefix: /home/neal/qx/root/envs/test
   
     Updating specs:
   
      - q-kx-protobuf
   
   
     Package          Version  Build        Channel           Size
   ─────────────────────────────────────────────────────────────────
     Install:
   ─────────────────────────────────────────────────────────────────
   
     + _openmp_mutex      4.5  20_gnu       conda-forge     Cached
     + libgcc          15.2.0  he0feb66_19  conda-forge     Cached
     + libgcc-ng       15.2.0  h69a702a_19  conda-forge     Cached
     + libgomp         15.2.0  he0feb66_19  conda-forge     Cached
     + libprotobuf    3.21.12  hfc55251_2   conda-forge     Cached
     + libstdcxx       15.2.0  h934c35e_19  conda-forge     Cached
     + libstdcxx-ng    15.2.0  hdf11a46_19  conda-forge     Cached
     + libzlib          1.3.2  h25fd6f3_2   conda-forge     Cached
     + q-kx-protobuf    2.0.0  hb0f4dca_0   kx              Cached
   
     Summary:
   
     Install: 9 packages
   
     Total download: 0 B
   
   ─────────────────────────────────────────────────────────────────
   
   
   Confirm changes: [Y/n]
   Y
   
   Transaction starting
   Linking libzlib-1.3.2-h25fd6f3_2
   Linking libgomp-15.2.0-he0feb66_19
   Linking _openmp_mutex-4.5-20_gnu
   Linking libgcc-15.2.0-he0feb66_19
   Linking libgcc-ng-15.2.0-h69a702a_19
   Linking libstdcxx-15.2.0-h934c35e_19
   Linking libstdcxx-ng-15.2.0-hdf11a46_19
   Linking libprotobuf-3.21.12-hfc55251_2
   Linking q-kx-protobuf-2.0.0-hb0f4dca_0
   
   Transaction finished
   ```

7. List the packages installed in the environment, which now contains `q-kx-protobuf` and its dependencies:

   ```
   q)qmamba.list[]
   List of packages in environment: "/home/neal/qx/root/envs/test"
   
     Name           Version  Build        Channel
   ────────────────────────────────────────────────────
     _openmp_mutex  4.5      20_gnu       conda-forge
     libgcc         15.2.0   he0feb66_19  conda-forge
     libgcc-ng      15.2.0   h69a702a_19  conda-forge
     libgomp        15.2.0   he0feb66_19  conda-forge
     libprotobuf    3.21.12  hfc55251_2   conda-forge
     libstdcxx      15.2.0   h934c35e_19  conda-forge
     libstdcxx-ng   15.2.0   hdf11a46_19  conda-forge
     libzlib        1.3.2    h25fd6f3_2   conda-forge
     q-kx-protobuf  2.0.0    hb0f4dca_0   kx
   ```

8. Load `q-kx-protobuf` and start using it:

   ```
   q)proto:use `kx.protobuf
   q)proto.versionStr[]
   "libprotobuf v3.21.12"
   q)result:proto.serializeArrayFromList[`ScalarExample; (1i;2.2f;"abc")]
   q)result
   "\010\001\021\232\231\231\231\231\231\001@\032\003abc"
   q)proto.parseArrayToList[`ScalarExample; result]
   1i
   2.2
   "abc"
   ```



## Next steps

Now that you have seen a demonstration of qmamba, [learn more](learning-more.md) about how to use it.