# Introduction



## Background

qmamba provides a [rich set of APIs](reference.md) to:

* search for packages
* install and manage packages
* create and manage [environments](./environments.md)

The qmamba APIs are mirrors of the mamba CLI commands so if you have experience using `micromamba` or `miniconda` then they should already be familiar.  But the basic APIs such as `search`, `install`, `list`, etc., are common to pretty much all package managers so anyone who has used `apt`, `yum`, `brew`, `vcpkg`, etc., can get quickly started.



## Loading qmamba

qmamba is loaded as normal with `use`:

```
q)qmamba:use `kx.qmamba
```



## Getting help with the APIs

Each API takes a dictionary input of options.  To see the available options for a given API, their types and a description, pass `` `help `` to the API, e.g.:

```
q)qmamba.install `help
option            | type       default    description
------------------| ---------------------------------------------------------------------------------------------------------------
ALLOW_DOWNGRADE   | int        0          "Allow downgrade when installing packages"
ALLOW_UNINSTALL   | int        1          "Allow uninstall when installing or updating packages"
...
VERIFY_ARTIFACTS  | int        0          "Run verifications on packages signatures (experimental)"
YES               | int        0          "Automatically answer yes on prompted questions"
```

Most APIs also have a shortcut which allows you to specify the most commonly used option for that operation without having to wrap it in a dictionary.  For example:

* `qmamba.install enlist "package"` is the same as ``qmamba.install (enlist `SPECS)!(enlist enlist "package")``
* `qmamba.activate "envname"` is the same as ``qmamba.activate (enlist `NAME)!(enlist "envname"))``
* shortcuts for other APIs are detailed in the [function reference](reference.md)



## Next steps

A few concepts are extensively used in the subsequent documentation.  You should get familiar with them first by reading the [Concepts](concepts.md) page.

Then continue with [Getting Started](getting-started.md) for a quick tour of the basic functionality.

