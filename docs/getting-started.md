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



## A quick tour

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

4. The KX channel is currently hosted on gitlab at https://kxdev.gitlab.io/kdbx/conda-channel/channel.  Since this is private it is necessary to provide your Personal Access Token for authentication (this only needs to be done once, the details are saved in `~/.mamba/auth/`):

   ```
   q)qmamba.auth.login (`HOST`BEARER)!("kxdev.gitlab.io";getenv `GITLAB_TOKEN)
   Successfully stored login information
   ```

   [TODO this won't be necessary with the KX Anaconda channel because it is public]

5. Let's say we are looking for a protobuf integration.  We can search for packages matching `*protobuf*` in the KX channel:

   ```
   q)qmamba.search (`SPECS`CHANNEL)!(enlist "*protobuf*";enlist "https://kxdev.gitlab.io/kdbx/conda-channel/channel")
   Getting repodata from channels...
   
       q-kx-protobuf 1.1.0 hb0f4dca_0
   ────────────────────────────────────────
   
    Name            q-kx-protobuf
    Version         1.1.0
    Build           hb0f4dca_0
    Size            2821 kB
    License
    Subdir          linux-64
    File Name       q-kx-protobuf-1.1.0-hb0f4dca_0.conda
    URL             https://kxdev.gitlab.io/kdbx/conda-channel/channel/linux-64/q-kx-protobuf-1.1.0-hb0f4dca_0.conda
    MD5             c15a0715e13bb429810862ff8d06af27
    SHA256          c68bbd4cfd859596fb992f293ef8b1de3346c068b4f832850b1106760555c72e
   
    Dependencies:
     - libgcc >=15
     - libstdcxx >=15
     - libprotobuf 3.*
     - libprotobuf >=3.21.12,<3.22.0a0
   ```

6. Install `q-kx-protobuf`.  Since it has third party dependencies such as `libprotobuf`, we add `conda-forge` to the channel list and qmamba will use that to fetch these dependencies, with `q-kx-protobuf` coming from the KX channel.

   ```
   q)qmamba.install (`SPECS`CHANNEL)!(enlist "q-kx-protobuf";("https://kxdev.gitlab.io/kdbx/conda-channel/channel";"conda-forge"))
   conda-forge/linux-64                                        Using cache
   conda-forge/noarch                                          Using cache
   
   
   Transaction
   
     Prefix: /home/neal/qx/root/envs/test
   
     Updating specs:
   
      - q-kx-protobuf
   
   
     Package          Version  Build        Channel                                          Size
   ────────────────────────────────────────────────────────────────────────────────────────────────
     Install:
   ────────────────────────────────────────────────────────────────────────────────────────────────
   
     + _openmp_mutex      4.5  20_gnu       conda-forge                                    Cached
     + q-kx-protobuf    1.1.0  hb0f4dca_0   kxdev.gitlab.io/kdbx/conda-channel/channel        3MB
     + libgcc          15.2.0  he0feb66_19  conda-forge                                    Cached
     + libgcc-ng       15.2.0  h69a702a_19  conda-forge                                    Cached
     + libgomp         15.2.0  he0feb66_19  conda-forge                                    Cached
     + libprotobuf    3.21.12  hfc55251_2   conda-forge                                       2MB
     + libstdcxx       15.2.0  h934c35e_19  conda-forge                                    Cached
     + libstdcxx-ng    15.2.0  hdf11a46_19  conda-forge                                    Cached
     + libzlib          1.3.2  h25fd6f3_2   conda-forge                                    Cached
   
     Summary:
   
     Install: 9 packages
   
     Total download: 0 B
   
   ────────────────────────────────────────────────────────────────────────────────────────────────
   
   
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
   Linking q-kx-protobuf-1.1.0-hb0f4dca_0
   
   Transaction finished
   ```

7. List the packages installed in the environment, which now contains `q-kx-protobuf` and its dependencies:

   ```
   q)qmamba.list[]
   List of packages in environment: "/home/neal/qx/root/envs/test"
   
     Name           Version  Build        Channel
   ───────────────────────────────────────────────────────────────────────────────────────────
     _openmp_mutex  4.5      20_gnu       conda-forge
     q-kx-protobuf  1.1.0    hb0f4dca_0   https://kxdev.gitlab.io/kdbx/conda-channel/channel
     libgcc         15.2.0   he0feb66_19  conda-forge
     libgcc-ng      15.2.0   h69a702a_19  conda-forge
     libgomp        15.2.0   he0feb66_19  conda-forge
     libprotobuf    3.21.12  hfc55251_2   conda-forge
     libstdcxx      15.2.0   h934c35e_19  conda-forge
     libstdcxx-ng   15.2.0   hdf11a46_19  conda-forge
     libzlib        1.3.2    h25fd6f3_2   conda-forge
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