# Further Information



## Benefits of conda

Using conda packages as the distribution mechanism for kdb-x modules has a number of advantages which fall into two categories

1. The package structure
   * It is language agnostic - the file structure is flexible allowing q files to be including
   * The binaries are prebuilt - it is not necessary to build them from source which would be much slower
   * Dependencies are specified as metadata so do not need to be included in the package itself.  During installation those dependent packages are automatically pulled from their providers

2. The conda ecosystem is much richer compared to other package managers
   * it offers a wealth of third party packages with multiple versions for each right up to the latest
   * it supports different OSes (linux, macOS, Windows)
   * it supports differently architectures (e.g. x86/64, arm, m1)



## How kdb-x modules are structured as conda packages

The basic package structure is unchanged, it contains the same files and metadata as found in any conda package.

Rather the layout of the payload (the part that is installed to the *target prefix*) has been extended to include a `q/` directory under `$PREFIX`.  This directory contains the q files and any shared objects for each module (although the dependencies of such shared objects are installed to `$PREFIX/lib` as normal):

```
$PREFIX
├── q
│   └── mod
│       └── kx
│           └── foo
│               ├── clib.li64.so
│               └── init.q
│           └── bar
│               └── init.q
├── lib
└── <other standard directories>
```



## Advanced topics

Please refer to the Mamba documentation for more advanced topics such as:

* [Channels](https://mamba.readthedocs.io/en/latest/advanced_usage/more_concepts.html#channel) 
* [Linking to the package cache](https://mamba.readthedocs.io/en/latest/advanced_usage/more_concepts.html#linking)
* [Package install process](https://mamba.readthedocs.io/en/latest/advanced_usage/detailed_operations.html#install)
* [Artifacts verification](https://mamba.readthedocs.io/en/latest/advanced_usage/artifacts_verification.html)
* [Package resolution](https://mamba.readthedocs.io/en/latest/advanced_usage/package_resolution.html)

