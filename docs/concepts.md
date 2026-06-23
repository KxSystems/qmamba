# Concepts

Adapted from the [libmamba documentation](https://mamba.readthedocs.io/en/latest/user_guide/concepts.html).



## Package

A *package* is basically an tarball containing:

* source files (including q scripts)
* binaries such as shared objects
* metadata, including versioning information and a list of the package's dependencies



## Channel

A *channel* is a provider of packages.

Typically a *channel* refers to a webserver which is hosting a repository of packages.  But it can equally refer to a simple directory on your local machine.



## Prefix

In Unix-like platforms, installing a piece of software consists of placing files in subdirectories of an “installation prefix”:

```
$PREFIX
├── bin
├── include
├── lib
└── share
```

(examples of these are the `/usr/` and `/usr/local/` directories)

A *prefix* is a fully self-contained and portable installation.  Mamba allows you have multiple prefixes which are independent of each other.

To disambiguate with [root prefix](#root-prefix), *prefix* is often referred to as *target prefix*. Without an explicit *target* or *root* prefix, you can assume it refers to a *target prefix*.



## Environment

An *environment* and *target prefix* are the same thing.

However, an *environment* is typically referred to by a *name* whereas a *target prefix* is referred to by a *path*.



## Root prefix

The *root prefix* provides a convenient location to store *environments* (`$ROOT_PREFIX/envs/`), although you are free to create an *environment* elsewhere.

The *root prefix* also holds a *cache* which is used to speed up future operations and that *cache* is shared by all *environments* or *target prefixes* based under the same *root prefix*.



## Base environment

The *base* environment is the environment located at the *root prefix*.

qmamba installs some basic packages into the *base* environment so that q can load them at startup.



## Next steps

Continue with [Getting Started](getting-started.md) for a quick tour of the basic functionality.