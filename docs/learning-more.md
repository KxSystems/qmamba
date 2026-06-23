# Learning more



## The qmamba APIs

All the qmamba APIs are described in the [function reference](reference.md), with each one detailing:

* the calling syntax, including the shortcut
* frequently used options for that operation
* a basic example demonstrating of its use

To start with you can focus on the most frequently used operations such are:

| **Name**                                  | **Description**                         |
| ----------------------------------------- | --------------------------------------- |
| [`search`](./reference.md#search)         | Find packages on channels               |
| [`create`](./reference.md#create)         | Create new environment                  |
| [`activate`](./reference.md#activate)     | Activate an environment                 |
| [`install`](./reference.md#install)       | Install packages in active environment  |
| [`list`](./reference.md#list)             | List packages in active environment     |
| [`remove`](./reference.md#remove)         | Remove packages from active environment |
| [`update`](./reference.md#update)         | Update packages in active environment   |
| [`active`](./reference.md#active)         | Get active prefix                       |
| [`deactivate`](./reference.md#deactivate) | Deactivate environment                  |
| [`env.list`](./reference.md#envlist)      | List known environments                 |
| [`env.remove`](./reference.md#envremove)  | Remove an environment                   |



## Guidelines for the safe use of shared objects

When you `use` a module containing a shared object, that shared object is loaded into kdb.  But typically a shared object will have dependencies on other libraries so those shared objects are also loaded.  This continues recursively with dependencies of dependencies being loaded and so on.  A large module can have therefore have a significant tree of dependencies (you can use [repoquery.depends](./reference.md#repoquerydepends) with the `` `RECURSIVE `` option to view these).

When multiple such modules are loaded it is possible that parts of the dependency tree from one module will begin to intersect with the dependency tree of another module where they share common (sub) dependencies.  This can lead to problems if the two modules each require different versions of this common dependency.

It is recommended that only one version of a given library be loaded into the same process (i.e. q session).  Indeed the libmamba dependency resolution detects this and won't install a package into an environment if it requires a different version of a library than that already present in the environment - it will either require you to uninstall the conflicting package or it will fail the installation of the new package.

qmamba takes a similar approach by imposing guardrails around actions which are potentially unsafe:

1. If you have already used a module from one environment then try to activate a different environment, qmamba will warn you not to use any modules from the new environment and prompt you whether to continue.  This is in case the modules being loaded from the new environment conflict with the already loaded modules from the old environment.  The correct way to handle this scenario is to install all the required modules into the *same* environment (such that libmamba can prevent versioning conflicts) then load all the modules from that environment.
2. If you have already used a module from an environment then try to make changes to that same environment qmamba will warn you.  This is case your changes impact (e.g. remove) any of the dependencies required by the already loaded modules.  The correct way to address this scenario is to have all the packages in your environment present and stable before loading anything so you don't subsequently have to change the environment.

But these guardrails cannot protect against all potential misuse.  One such example is an arbitrary module being loaded from an location which is not managed by qmamba.  Where a module is loaded from outside the active environment then qmamba/libmamba will have no knowledge of it (what dependencies it requires, where those dependencies are coming from, what versions of those dependencies are required, etc.) so has no information to act on.

If you do encounter an unexpected issue then you should consult the [troubleshooting guide](./troubleshooting.md) for suggestions on how to identify the cause and possible remedies.



## Other mamba clients

Other mamba clients such as the [micromamba CLI](micromamba.md) can be used to download and install kdb-x modules from the command line, prior to starting q.

These are fully interoperable with qmamba, any such client can be used to manage the environments created by another.



## Next steps

That's the main topics covered.

If you're looking for more fine grained control over a qmamba operation remember to use the [help facility](introduction.md#getting-help-with-the-apis) for that API to check if there is an option which could be applicable.

Similarly the [function reference](reference.md) covers all operations not just the frequently used ones.

If you want to learn more about the underlying design of mamba then continue to the [further information](further-information.md) page.