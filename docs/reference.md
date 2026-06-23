# Function reference

The list of qmamba APIs is:

| **Name**                                   | **Description**                                         |
| ------------------------------------------ | ------------------------------------------------------- |
| [`search`](#search)                        | Find packages on channels                               |
| [`create`](#create)                        | Create new environment                                  |
| [`activate`](#activate)                    | Activate an environment                                 |
| [`install`](#install)                      | Install packages in active environment                  |
| [`list`](#list)                            | List packages in active environment                     |
| [`remove`](#remove)                        | Remove packages from active environment                 |
| [`update`](#update)                        | Update packages in active environment                   |
| [`active`](#active)                        | Get active prefix                                       |
| [`deactivate`](#deactivate)                | Deactivate environment                                  |
| [`env.list`](#envlist)                     | List known environments                                 |
| [`env.remove`](#envremove)                 | Remove an environment                                   |
| [`env.export`](#envexport)                 | Export environment specification                        |
| [`env.update`](#envupdate)                 | Update an environment using file specifications         |
| [`repoquery.search`](#repoquerysearch)     | Search for packages matching a given query              |
| [`repoquery.depends`](#repoquerydepends)   | List dependencies of a given query                      |
| [`repoquery.whoneeds`](#repoquerywhoneeds) | List packages that need the given query as a dependency |
| [`auth.login`](#authlogin)                 | Store login information for a specific host             |
| [`auth.logout`](#authlogout)               | Delete login information for a specific host            |
| [`clean`](#clean)                          | Clean package cache                                     |
| [`info`](#info)                            | Information about qmamba                                |
| [initialiseRoot](#initialiseroot)          | Initialise the root prefix                              |



## `search`

*Find packages on channels*

```
qmamba.search[options]
```

where `options` is one of:

* the package specifications (string list) to search for, shortcut for ``qmamba.search (enlist `SPECS)!(enlist specs)``
* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:

| option  | type       | default | description                                         |
| ------- | ---------- | ------- | --------------------------------------------------- |
| CHANNEL | stringlist | n/a     | Define the list of channels                         |
| LOCAL   | int        | 0       | Search local prefix instead of remote repositories  |
| PRETTY  | int        | 0       | Pretty print result                                 |
| SPECS   | stringlist | n/a     | Specs to query                                      |
| VERBOSE | int        | 0       | Set verbosity (0, 1 or 2 with increasing verbosity) |

Alias for [repoquery.search](#repoquerysearch).

```
q)qmamba.search (`SPECS`CHANNEL)!(enlist "q-*"; enlist ("https://kxdev.gitlab.io/kdbx/conda-channel/channel"))
Getting repodata from channels...

 Name          Version Build        Channel         Subdir
─────────────────────────────────────────────────────────────
 q-kx-arrow    1.5.0   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-avro     1.1.0   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-expat    1.2.1   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-expat    1.1.0   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-fusionx  1.2.1   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-fusionx  1.1.0   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-kafka    0.0.1   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-log      1.0.0   h4616a5c_0   kxdev.gitlab.io noarch
 q-kx-openblas 1.2.1   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-openblas 1.1.0   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-pcre2    1.2.1   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-pcre2    1.1.0   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-printf   1.0.1   h4616a5c_0   kxdev.gitlab.io noarch
 q-kx-protobuf 1.1.0   hb0f4dca_0   kxdev.gitlab.io linux-64
```



## `create`

*Create a new environment*

```
qmamba.create[options]
```

where `options` is one of:

* the name (string) of the environment to create, shortcut for ``qmamba.create (enlist `NAME)!(enlist name))``
* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:
option            | type     |  default  |  description
------------------| --------------------------------------------------------------------------------------------------------------- |------------------|------------------
FILE | stringlist | n/a | File(s) providing package specifications (yaml or json) 
NAME              | string |    n/a |        Name of the target prefix
PREFIX            | string |    n/a |        Path to the target prefix
ROOT_PREFIX       | string | "$QHOME/root" |        Path to the root prefix

Once created, the environment should be activated in order to load packages from it.

```
q)qmamba.env.list[]
  Name  Active  Path
──────────────────────────────────────────────────────────────
  base  *       /home/neal/qx/root
q)qmamba.create "test"
Empty environment created at prefix: /home/neal/qx/root/envs/test
q)qmamba.activate "test"
Activated environment at prefix: /home/neal/qx/root/envs/test
q)qmamba.env.list[]
  Name  Active  Path
──────────────────────────────────────────────────────────────
  base          /home/neal/qx/root
  test  *       /home/neal/qx/root/envs/test
```



## `activate`

*Activate an environment*

```
qmamba.create[options]
```

where `options` is one of:

* the name (string) of the environment to create, shortcut for ``qmamba.activate (enlist `NAME)!(enlist name))``
* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:

| option       | type   | default       | description                                                  |
| ------------ | ------ | ------------- | ------------------------------------------------------------ |
| NAME         | string | n/a           | Name of the target prefix                                    |
| PREFIX       | string | n/a           | Path to the target prefix                                    |
| ROOT_PREFIX  | string | "$QHOME/root" | Path to the root prefix                                      |
| GUARD_IN_USE | string | "prompt"      | Guard against modules being in use from the old prefix ('permit', 'warn', 'prompt' or 'deny') |

After activating, any subsequent qmamba operations are applied by default to this environment.  It also sets the kdb-x [search path](https://code.kx.com/kdb-x/modules/module-framework/quickstart.html#search-path) (`.Q.m.SP`) to look in this environment when loading a module.

```
q)qmamba.env.list[]
  Name  Active  Path
──────────────────────────────────────────────────────────────
  base  *       /home/neal/qx/root
  test          /home/neal/qx/root/envs/test
q)qmamba.active[]
"/home/neal/qx/root"
q).Q.m.SP
"/home/neal/qx/root/q/mod"
"/home/neal/qx/mod"
q)qmamba.activate "test"
Activated environment at prefix: /home/neal/qx/root/envs/test
q)qmamba.active[]
"/home/neal/qx/root/envs/test"
q).Q.m.SP
"/home/neal/qx/root/envs/test/q/mod"
"/home/neal/qx/mod"
```



## `install`

*Install packages in active environment*

```
qmamba.install[options]
```

where `options` is one of:

* the package specifications (string list) to install, shortcut for ``qmamba.install (enlist `SPECS)!(enlist specs)``
* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:

| option       | type   | default       | description                                                  |
| ------------ | ------ | ------------- | ------------------------------------------------------------ |
| ALLOW_DOWNGRADE   | int        | 0    | Allow downgrade when installing packages |
| ALLOW_UNINSTALL   | int        | 1    | Allow uninstall when installing packages |
| CHANNEL           | stringlist | n/a  | Define the list of channels |
| FILE              | stringlist | n/a  | File(s) providing package specifications (yaml or json) |
| FREEZE_INSTALLED  | int        | 0    | Freeze already installed dependencies |
| GUARD_IN_USE      | string     | "warn" | Guard against making changes to an in use prefix ('permit', 'warn', 'prompt' or 'deny') |
| SPECS             | stringlist | n/a  | Specs to install into the environment |
| VERBOSE           | int        | 0      | Set verbosity (0, 1 or 2 with increasing verbosity) |

Where there already are packages in the environment and libmamba detects conflicts in the dependency resolution between them and the new package, it can:

* Uninstall the conflicting existing packages (option `ALLOW_UNINSTALL`)
* Attempt to downgrade the conflicting existing packages (option `ALLOW_DOWNGRADE`)
* Not change the conflicting existing packages, in which case the installation will fail (options `FREEZE_INSTALLED`)

```
q)qmamba.list[]
List of packages in environment: "/home/neal/qx/root/envs/test"
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
  + q-kx-protobuf    1.1.0  hb0f4dca_0   kxdev.gitlab.io/kdbx/conda-channel/channel     Cached
  + libgcc          15.2.0  he0feb66_19  conda-forge                                    Cached
  + libgcc-ng       15.2.0  h69a702a_19  conda-forge                                    Cached
  + libgomp         15.2.0  he0feb66_19  conda-forge                                    Cached
  + libprotobuf    3.21.12  hfc55251_2   conda-forge                                    Cached
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




## `list`

*List packages in active environment*

```
qmamba.list[options]
```

where `options` is one of:

* generic null (::) to list all packages in current environment
* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:

| option           | type       | default | description                                                  |
| ---------------- | ---------- | ------- | ------------------------------------------------------------ |
|CANONICAL       | int        |0|       Output canonical names of packages only|
|EXPLICIT        | int        |0 |      List explicitly all installed packages with URL|
|FULL_NAME       | int        |0  |     Only search for full names, i.e., `^<regex>$`|
|REGEX           | string     |""  |    List only packages matching a regular expression|
|REVISIONS       | int        |0     |  List the revision history|

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



## `remove`

*Remove packages from active environment*

```
qmamba.remove[options]
```

where `options` is one of:

* the package specifications (string list) to remove, shortcut for ``qmamba.remove (enlist `SPECS)!(enlist specs)``
* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:

| option       | type       | default | description                                                  |
| ------------ | ---------- | ------- | ------------------------------------------------------------ |
| ALL          | int        | 0       | Remove all packages in the environment                       |
| GUARD_IN_USE | string     | "warn"  | Guard against making changes to an in use prefix ('permit', 'warn', 'prompt' or 'deny') |
| SPECS        | stringlist | n/a     | Specs to remove from the environment                         |
| VERBOSE      | int        | 0       | Set verbosity (0, 1 or 2 with increasing verbosity)          |

```
q)qmamba.list[]
List of packages in environment: "/home/neal/qx/root/envs/test"

  Name           Version  Build        Channel
───────────────────────────────────────────────────────────────────────────────────────────
  _openmp_mutex  4.5      20_gnu       conda-forge
  q-kx-printf    1.0.1    h4616a5c_0   https://kxdev.gitlab.io/kdbx/conda-channel/channel
  q-kx-protobuf  1.1.0    hb0f4dca_0   https://kxdev.gitlab.io/kdbx/conda-channel/channel
  libgcc         15.2.0   he0feb66_19  conda-forge
  libgcc-ng      15.2.0   h69a702a_19  conda-forge
  libgomp        15.2.0   he0feb66_19  conda-forge
  libprotobuf    3.21.12  hfc55251_2   conda-forge
  libstdcxx      15.2.0   h934c35e_19  conda-forge
  libstdcxx-ng   15.2.0   hdf11a46_19  conda-forge
  libzlib        1.3.2    h25fd6f3_2   conda-forge
q)qmamba.remove enlist "q-kx-printf"
Transaction

  Prefix: /home/neal/qx/root/envs/test

  Removing specs:

   - q-kx-printf


  Package       Version  Build       Channel                                          Size
──────────────────────────────────────────────────────────────────────────────────────────
  Remove:
──────────────────────────────────────────────────────────────────────────────────────────

  - q-kx-printf 1.0.1    h4616a5c_0  kxdev.gitlab.io/kdbx/conda-channel/channel     Cached

  Summary:

  Remove: 1 packages

  Total download: 0 B

──────────────────────────────────────────────────────────────────────────────────────────


Confirm changes: [Y/n]
Y

Transaction starting
Unlinking q-kx-printf-1.0.1-h4616a5c_0

Transaction finished

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



## `update`

*Update packages in active environment*

```
qmamba.update[options]
```

where `options` is one of:

* the package specifications (string list) to update, shortcut for ``qmamba.update (enlist `SPECS)!(enlist specs)``
* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:

| option          | type       | default | description                                                  |
| --------------- | ---------- | ------- | ------------------------------------------------------------ |
| ALL             | int        | 0       | Update all packages in the environment                       |
| ALLOW_DOWNGRADE | int        | 0       | Allow downgrade when installing packages                     |
| ALLOW_UNINSTALL | int        | 1       | Allow uninstall when installing packages                     |
| GUARD_IN_USE    | string     | "warn"  | Guard against making changes to an in use prefix ('permit', 'warn', 'prompt' or 'deny') |
| SPECS           | stringlist | n/a     | Specs to update in the environment                           |
| VERBOSE         | int        | 0       | Set verbosity (0, 1 or 2 with increasing verbosity)          |

Where libmamba detects conflicts in the dependency resolution between the updated package and other packages, it can:

* Uninstall the existing conflicting packages (option `ALLOW_UNINSTALL`)
* Attempt to downgrade the existing conflicting packages (option `ALLOW_DOWNGRADE`)

```
q)qmamba.list[]
List of packages in environment: "/home/neal/qx/root/envs/lzma"

  Name           Version  Build        Channel
────────────────────────────────────────────────────
  _openmp_mutex  4.5      20_gnu       conda-forge
  libgcc         15.2.0   he0feb66_19  conda-forge
  libgomp        15.2.0   he0feb66_19  conda-forge
  liblzma        5.8.2    hb03c661_0   conda-forge
q)qmamba.update enlist "liblzma"
conda-forge/linux-64                                        Using cache
conda-forge/noarch                                          Using cache

Transaction

  Prefix: /home/neal/qx/root/envs/lzma

  Updating specs:

   - liblzma
  Removing specs:

   - liblzma


  Package    Version  Build       Channel           Size
──────────────────────────────────────────────────────────
  Upgrade:
──────────────────────────────────────────────────────────

  - liblzma    5.8.2  hb03c661_0  conda-forge     Cached
  + liblzma    5.8.3  hb03c661_0  conda-forge     Cached

  Summary:

  Upgrade: 1 packages

  Total download: 0 B

──────────────────────────────────────────────────────────


Confirm changes: [Y/n]
Y

Transaction starting
Unlinking liblzma-5.8.2-hb03c661_0
Linking liblzma-5.8.3-hb03c661_0

Transaction finished

q)qmamba.list[]
List of packages in environment: "/home/neal/qx/root/envs/lzma"

  Name           Version  Build        Channel
────────────────────────────────────────────────────
  _openmp_mutex  4.5      20_gnu       conda-forge
  libgcc         15.2.0   he0feb66_19  conda-forge
  libgomp        15.2.0   he0feb66_19  conda-forge
  liblzma        5.8.3    hb03c661_0   conda-forge
```



## `active`

*Get active prefix*

```
qmamba.active[]
```

Returns the prefix path to the active environment.

```
q)qmamba.active[]
"/home/neal/qx/root/envs/test"
```



## `deactivate`

*Deactivate environment*

```
qmamba.deactivate[]
```

When an environment is deactivated, the base environment becomes active instead.

```
q)qmamba.active[]
"/home/neal/qx/root/envs/test"
q)qmamba.deactivate[]
Activated environment at prefix: /home/neal/qx/root
q)qmamba.active[]
"/home/neal/qx/root"
```



## `env.list`

*List known environments*

```
qmamba.env.list[options]
```

where `options` is one of:

* generic null (::) to list environments
* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:

| option      | type   | default         | description             |
| ----------- | ------ | --------------- | ----------------------- |
| ROOT_PREFIX | string | "${QHOME}/root" | Path to the root prefix |

```
q)qmamba.env.list[]
  Name   Active  Path
───────────────────────────────────────────────────────────────
  base           /home/neal/qx/root
  test   *       /home/neal/qx/root/envs/test
```



## `env.remove`

*Remove an environment*

```
qmamba.env.remove[options]
```

where `options` is one of:

* the name (string) of the environment to remove, shortcut for ``qmamba.env.remove (enlist `NAME)!(enlist name))``
* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:

| option       | type   | default       | description                                                  |
| ------------ | ------ | ------------- | ------------------------------------------------------------ |
| NAME         | string | n/a           | Name of the target prefix                                    |
| PREFIX       | string | n/a           | Path to the target prefix                                    |
| ROOT_PREFIX  | string | "$QHOME/root" | Path to the root prefix                                      |
| GUARD_IN_USE | string | "prompt"      | Guard against making changes to an in use prefix ('permit', 'warn', 'prompt' or 'deny') |

```
q)qmamba.env.list[]
  Name   Active  Path
───────────────────────────────────────────────────────────────
  base   *       /home/neal/qx/root
  test           /home/neal/qx/root/envs/test
q)qmamba.env.remove "test"
Transaction

  Prefix: /home/neal/qx/root/envs/test

  Removing specs:

   - q-kx-printf


  Package     Version  Build       Channel                                          Size
──────────────────────────────────────────────────────────────────────────────────────────
  Remove:
──────────────────────────────────────────────────────────────────────────────────────────

  - q-kx-printf 1.0.0  h4616a5c_0  kxdev.gitlab.io/kdbx/conda-channel/channel     Cached

  Summary:

  Remove: 1 packages

  Total download: 0 B

──────────────────────────────────────────────────────────────────────────────────────────


Confirm changes: [Y/n]
Y

Transaction starting
Unlinking q-kx-printf-1.0.0-h4616a5c_0

Transaction finished

Environment removed at prefix: /home/neal/qx/root/envs/test
q)qmamba.env.list[]
  Name   Active  Path
───────────────────────────────────────────────────────────────
  base   *       /home/neal/qx/root
```



## `env.export`

*Export environment specification*

```
qmamba.env.export[options]
```

where `options` is one of:

* generic null (::) to export the active environment
* the name (string) of the environment to export, shortcut for ``qmamba.env.export (enlist `NAME)!(enlist name))``
* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

It returns a kdb string with the yaml specification.  When written to a file (e.g. with the `TO_FILE` option), this yaml specification can be used to recreate this environment by using the `FILE` option with [create](#create), [install](#install) or [env.update](#envupdate).

```
q)-1 qmamba.env.export[];
name: proto
channels:
  - conda-forge
  - https://kxdev.gitlab.io/kdbx/conda-channel/channel
dependencies:
  - _openmp_mutex=4.5=20_gnu
  - q-kx-protobuf=1.1.0=hb0f4dca_0
  - libgcc=15.2.0=he0feb66_19
  - libgcc-ng=15.2.0=h69a702a_19
  - libgomp=15.2.0=he0feb66_19
  - libprotobuf=3.21.12=hfc55251_2
  - libstdcxx=15.2.0=h934c35e_19
  - libstdcxx-ng=15.2.0=hdf11a46_19

prefix: "/home/neal/qx/root/envs/proto"
```

Frequently used options:

| option       | type   | default       | description                                                  |
| ------------ | ------ | ------------- | ------------------------------------------------------------ |
| EXPLICIT        | int       | 0       | Use explicit format |
| FROM_HISTORY    | int       | 0       | Build environment spec from explicit specs in history |
| NAME         | string | n/a           | Name of the target prefix                                    |
| PREFIX       | string | n/a           | Path to the target prefix                                    |
| ROOT_PREFIX  | string | "$QHOME/root" | Path to the root prefix                                      |
| TO_FILE         | string | n/a |      Write output to the specified file |



## `env.update`

*Update an environment using file specifications*

```
qmamba.env.update[options]
```

where `options` is one of:

* the specification files (string list) to update with, shortcut for ``qmamba.env.update (enlist `FILE)!(enlist file)``
* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:

| option   | type | default | description         |
| -------- | ---- | ------- | ------------------- |
| FILE            | stringlist | n/a | File(s) providing package specifications (yaml or json) |
| GUARD_IN_USE    | string     | "warn" |      Guard against making changes to an in use prefix ('permit', 'warn', 'prompt' or 'deny') |
| NAME         | string | n/a           | Name of the target prefix                                    |
| PREFIX       | string | n/a           | Path to the target prefix                                    |
| PRUNE           | int        | 0   |  Remove installed packages not in specifications |
| ROOT_PREFIX  | string | "$QHOME/root" | Path to the root prefix                                      |
| VERBOSE         | int        | 0   |    Set verbosity (0, 1 or 2 with increasing verbosity) |

Default where no `NAME` or `PREFIX` is specified is to update the active environment.
```
q)qmamba.env.export (enlist `TO_FILE)!(enlist "proto_spec.yaml")
q)qmamba.create "new_proto"
Empty environment created at prefix: /home/neal/qx/root/envs/new_proto
q)qmamba.activate "new_proto"
Activated environment at prefix: /home/neal/qx/root/envs/new_proto
q)qmamba.env.update enlist "proto_spec.yaml"
conda-forge/linux-64                                        Using cache
conda-forge/noarch                                          Using cache

Transaction

  Prefix: /home/neal/qx/root/envs/new_proto

  Updating specs:

   - _openmp_mutex=4.5=20_gnu
   - q-kx-protobuf=1.1.0=hb0f4dca_0
   - libgcc=15.2.0=he0feb66_19
   - libgcc-ng=15.2.0=h69a702a_19
   - libgomp=15.2.0=he0feb66_19
   - libprotobuf=3.21.12=hfc55251_2
   - libstdcxx=15.2.0=h934c35e_19
   - libstdcxx-ng=15.2.0=hdf11a46_19


  Package          Version  Build        Channel                                          Size
────────────────────────────────────────────────────────────────────────────────────────────────
  Install:
────────────────────────────────────────────────────────────────────────────────────────────────

  + _openmp_mutex      4.5  20_gnu       conda-forge                                    Cached
  + libgcc          15.2.0  he0feb66_19  conda-forge                                    Cached
  + libgcc-ng       15.2.0  h69a702a_19  conda-forge                                    Cached
  + libgomp         15.2.0  he0feb66_19  conda-forge                                    Cached
  + libprotobuf    3.21.12  hfc55251_2   conda-forge                                    Cached
  + libstdcxx       15.2.0  h934c35e_19  conda-forge                                    Cached
  + libstdcxx-ng    15.2.0  hdf11a46_19  conda-forge                                    Cached
  + libzlib          1.3.2  h25fd6f3_2   conda-forge                                    Cached
  + q-kx-protobuf    1.1.0  hb0f4dca_0   kxdev.gitlab.io/kdbx/conda-channel/channel        3MB

  Summary:

  Install: 9 packages

  Total download: 3MB

────────────────────────────────────────────────────────────────────────────────────────────────


Confirm changes: [Y/n]
Y

Transaction starting
Linking libgomp-15.2.0-he0feb66_19
Linking libzlib-1.3.2-h25fd6f3_2
Linking _openmp_mutex-4.5-20_gnu
Linking libgcc-15.2.0-he0feb66_19
Linking libstdcxx-15.2.0-h934c35e_19
Linking libgcc-ng-15.2.0-h69a702a_19
Linking libstdcxx-ng-15.2.0-hdf11a46_19
Linking libprotobuf-3.21.12-hfc55251_2
Linking q-kx-protobuf-1.1.0-hb0f4dca_0

Transaction finished
```



## `repoquery.search`

*Search for packages matching a given query*

```
qmamba.repoquery.search[options]
```

where `options` is one of:

* the package specifications (string list) to query, shortcut for ``qmamba.repoquery.search (enlist `SPECS)!(enlist specs)``
* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:

| option  | type       | default | description                                           |
| ------- | ---------- | ------- | ----------------------------------------------------- |
| CHANNEL | stringlist | n/a     | Define the list of channels                           |
| LOCAL   | int        | 0       | Search local prefix instead of remote repositories    |
| PRETTY  | int        | 0       | Pretty print result with more detail for each package |
| SPECS   | stringlist | n/a     | Specs to query                                        |
| VERBOSE | int        | 0       | Set verbosity (0, 1 or 2 with increasing verbosity)   |

```
q)qmamba.search (`SPECS`CHANNEL)!(enlist "q-*"; enlist ("https://kxdev.gitlab.io/kdbx/conda-channel/channel"))
Getting repodata from channels...

 Name          Version Build        Channel         Subdir
─────────────────────────────────────────────────────────────
 q-kx-arrow    1.5.0   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-avro     1.1.0   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-expat    1.2.1   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-expat    1.1.0   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-fusionx  1.2.1   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-fusionx  1.1.0   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-kafka    0.0.1   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-log      1.0.0   h4616a5c_0   kxdev.gitlab.io noarch
 q-kx-openblas 1.2.1   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-openblas 1.1.0   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-pcre2    1.2.1   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-pcre2    1.1.0   hb0f4dca_0   kxdev.gitlab.io linux-64
 q-kx-printf   1.0.1   h4616a5c_0   kxdev.gitlab.io noarch
 q-kx-protobuf 1.1.0   hb0f4dca_0   kxdev.gitlab.io linux-64
```



## `repoquery.depends`

*List dependencies of a given query*

```
qmamba.repoquery.depends[options]
```

where `options` is one of:

* the package specifications (string list) to query, shortcut for ``qmamba.repoquery.depends (enlist `SPECS)!(enlist specs)``
* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:

| option    | type       | default | description                                         |
| --------- | ---------- | ------- | --------------------------------------------------- |
| CHANNEL   | stringlist | n/a     | Define the list of channels                         |
| RECURSIVE | int        | 0       | Show dependencies recursively                       |
| REMOTE    | int        | 0       | Query remote repositories instead of local prefix   |
| SPECS     | stringlist | n/a     | Specs to query                                      |
| TREE      | int        | 0       | Show results as a tree                              |
| VERBOSE   | int        | 0       | Set verbosity (0, 1 or 2 with increasing verbosity) |

```
q)qmamba.repoquery.depends (`SPECS`CHANNEL)!(enlist "q-kx-protobuf"; ("https://kxdev.gitlab.io/kdbx/conda-channel/channel";"conda-forge"))
Getting repodata from channels...

conda-forge/linux-64                                        Using cache
conda-forge/noarch                                          Using cache
 Name          Version  Build        Channel         Subdir
─────────────────────────────────────────────────────────────
 q-kx-protobuf   1.1.0  hb0f4dca_0   kxdev.gitlab.io linux-64
 libgcc         15.2.0  h767d61c_5   conda-forge     linux-64
 libprotobuf   3.21.12  h3eb15da_0   conda-forge     linux-64
 libstdcxx      15.2.0  h8f9b012_5   conda-forge     linux-64
```



## `repoquery.whoneeds`

*List packages that need the given query as a dependency*

```
qmamba.repoquery.whoneeds[options]
```

where `options` is one of:

* the package specifications (string list) to query, shortcut for ``qmamba.repoquery.whoneeds (enlist `SPECS)!(enlist specs)``
* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:

| option  | type       | default | description                                         |
| ------- | ---------- | ------- | --------------------------------------------------- |
| CHANNEL | stringlist | n/a     | Define the list of channels                         |
| REMOTE  | int        | 0       | Query remote repositories instead of local prefix   |
| SPECS   | stringlist | n/a     | Specs to query                                      |
| VERBOSE | int        | 0       | Set verbosity (0, 1 or 2 with increasing verbosity) |

Only a single query is supported.

```
q)qmamba.repoquery.whoneeds (`SPECS`CHANNEL)!(enlist "q-kx-printf"; ("https://kxdev.gitlab.io/kdbx/conda-channel/channel";"conda-forge"))
Getting repodata from channels...

conda-forge/linux-64                                        Using cache
conda-forge/noarch                                          Using cache
 Name     Version Build        Depends     Channel         Subdir
──────────────────────────────────────────────────────────────────
 q-kx-log 1.0.0   h4616a5c_0   q-kx-printf kxdev.gitlab.io noarch
```



## `auth.login`

*Store login information for a specific host*

```
qmamba.auth.login[options]
```

where `options` is one of:

* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:

| option  | type       | default | description                                         |
| ------- | ---------- | ------- | --------------------------------------------------- |
| BEARER  | string | n/a | Bearer token for the account |
| HOST    | string | n/a | Host for the account |
| PASSWORD| string | n/a | Password for the account |
| TOKEN   | string | n/a | Token for the account |
| USERNAME| string | n/a | User name for the account |

```
q)qmamba.auth.login (`HOST`BEARER)!("kxdev.gitlab.io"; getenv `GITLAB_TOKEN)
Successfully stored login information
```



## `auth.logout`

*Delete login information for a specific host*

```
qmamba.auth.logout[options]
```

where `options` is one of:

* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:

| option | type   | default | description            |
| ------ | ------ | ------- | ---------------------- |
| ALL    | int    | 0       | Log out from all hosts |
| HOST   | string | n/a     | Host for the account   |

```
q)qmamba.auth.logout (enlist `HOST)!(enlist "kxdev.gitlab.io")
Logged out from kxdev.gitlab.io
```



## `clean`

*Clean package cache*

```
qmamba.clean[options]
```

where `options` is one of:

* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:

| option | type   | default | description            |
| ------ | ------ | ------- | ---------------------- |
|ALL             | int |       0       | Remove index cache, lock files, unused cache packages, and tarballs |
|INDEX_CACHE     | int |       0       | Remove index cache |
|LOCKS           | int |       0       | Remove lock files from caches |
|PACKAGES        | int |       0       | Remove unused packages from writable package caches |
|TARBALLS        | int |       0       | Remove cached package tarballs |

```
q)qmamba.clean (enlist `ALL)!(enlist 1)
Collect information..
Cleaning index cache..
Cleaning lock files..
  Package file                       Size
───────────────────────────────────────────
  /home/neal/qx/root/pkgs
───────────────────────────────────────────

  q-kx-printf-1.0.0-h4616a5c_0.conda  4kB

  /home/neal/.mamba/pkgs
───────────────────────────────────────────



───────────────────────────────────────────

  Total size:                         4kB
Cleaning tarballs..

Remove tarballs: [Y/n]
Y
Cleaning packages..
q)
```




## `info`

*Information about qmamba*

```
qmamba.info[options]
```

where `options` is one of:

* generic null (::) to display basic information
* a dictionary of options.  Dictionary key must be a symbol list, values can be an int, string or mixed list.
* `` `help `` to see the available options, their types and a description of each

Frequently used options:

| option   | type | default | description    |
| -------- | ---- | ------- | -------------- |
| LICENSES | int  | 0       | Print licenses |

```
q)qmamba.info[]

       libmamba version : 2.1.0
           curl version : libcurl/8.20.0 OpenSSL/3.6.3 zlib/1.3.2 zstd/1.5.7 libssh2/1.11.1 nghttp2/1.68.1 mit-krb5/1.22.2
     libarchive version : libarchive 3.8.7 zlib/1.3.2 bz2lib/1.0.8 libzstd/1.5.7 libb2/bundled
       envs directories : /home/neal/qx/root/envs
          package cache : /home/neal/qx/root/pkgs
                          /home/neal/.mamba/pkgs
            environment : test (active)
           env location : /home/neal/qx/root/envs/test
      user config files : /home/neal/.mambarc
 populated config files :
       virtual packages : __unix=0=0
                          __linux=6.18.33=0
                          __glibc=2.31=0
                          __archspec=1=x86_64_v3
               channels : https://conda.anaconda.org/conda-forge/linux-64
                          https://conda.anaconda.org/conda-forge/noarch
       base environment : /home/neal/qx/root
               platform : linux-64
```



## `initialiseRoot`

*Initialise the root prefix*

```
q)qmamba.initialiseRoot[]
```

Installs the latest versions of the basic packages into the root prefix.

```
q)qmamba.initialiseRoot[]

Installing libgcc, libstdcxx, openssl, libcurl, curl, libxml2 < 2.14 from conda-forge into root prefix "/home/neal/qx/root"...
Done

Please exit q then start it with (set an alias):

LD_LIBRARY_PATH=/home/neal/qx/root/lib /home/neal/qx/bin/q
```

