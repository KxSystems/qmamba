# Troubleshooting



## libssl.so: version `xyz' not found

If you see an error similar to:

```
q)arrow:use `kx.arrow
'no kexport: /lib/x86_64-linux-gnu/libssl.so: version `OPENSSL_3.2.0' not found (required by /root/qx/root/envs/arrow/q/mod/kx/arrow/../../../../lib/././libcurl.so.4)
  [17] (.Q.m.ld@)

  [10] /root/qx/root/envs/arrow/q/mod/kx/arrow/init.q:2: clib:$[5<=.z.K;use`.clib;(`arrowkdb 2:(`kexport;1))[]]
```

Are you setting `LD_LIBRARY_PATH` as requested when starting q?  Run `qmamba.showStartup[]` to display the message again.

If the problem persists you may need to [reinitialise the base environment](reference.md#initialiseroot).  By using the latest conda versions of these base packages, it maximises their compatibility with other conda packages, including the kdb-x modules.



## You have already used modules from the old prefix...

If you see a warning similar to:

```
You have already used modules from the old prefix "/home/neal/qx/root/envs/test":

p                                                             s| m
---------------------------------------------------------------| ---------------------
"/home/neal/qx/root/envs/test/q/mod/kx/protobuf/clib.li64.so"  | .m.kx.0protobuf.0clib
"/home/neal/qx/root/envs/test/q/mod/kx/protobuf"               | .m.kx.0protobuf

For a given q session, all modules with shared object dependencies should be loaded from the same environment.
If you intend to load modules from the new prefix "/home/neal/qx/root", it is recommended that you restart q before doing so.

Otherwise unpredictable results can occur if binary incompatibilities exist between any dependencies present in both environments.
Are you sure you wish to continue (y/N)?
```

This warning is one of the guardrails around [potentially unsafe actions](./learning-more.md#guidelines-for-the-safe-use-of-shared-objects).  If you don't believe there will be a problem then you can answer `Y`.  To suppress this prompt set the `GUARD_IN_USE` option to `"warn"` or  `"permit"` when calling the API.



## You are attempting to make changes to the prefix...

If you see a warning similar to:

```
You are attempting to make changes to the prefix "/home/neal/qx/root/envs/test" from which you have already loaded the modules:

p                                                             s| m
---------------------------------------------------------------| ---------------------
"/home/neal/qx/root/envs/test/q/mod/kx/protobuf/clib.li64.so"  | .m.kx.0protobuf.0clib
"/home/neal/qx/root/envs/test/q/mod/kx/protobuf"               | .m.kx.0protobuf

This may cause unpredictable results if the changes affect those modules or their dependencies.
```

This warning is one of the guardrails around [potentially unsafe actions](./learning-more.md#guidelines-for-the-safe-use-of-shared-objects).  If you don't believe there will be a problem then it can be ignored.  To suppress this warning set the `GUARD_IN_USE` option to `"permit"` when calling the API.



## foobar.so: version `xyz' not found

## foobar.so: undefined symbol: xyz

## Something unexpected occurs when I try to use a module or exit from q

These are all permutations of the same root cause.

It indicates a problem with loading the dependencies of the module as described by [potentially unsafe actions](./learning-more.md#guidelines-for-the-safe-use-of-shared-objects).  Some suggestions:

* Did you ignore any or the guardrail prompts/warnings?
* Did you load a module from a directory not controlled by qmamba (i.e. not from an environment)?
* If you restart q and load just this module does the problem still occur?

Otherwise there could be a problem with this environment.  Try cloning it by:

* exporting the environment specification to a file using [env.export](./reference.md#envexport) with the `TO_FILE` option
* create a new environment
* install the environment specification into the new environment using [install](./reference.md#install) with the `FILE` option

Then try to recreate the issue in the new environment



## When I install a new package it tries to remove an existing one

Likely there is a conflict in the dependency resolution - mamba can't find a suitable version of a common library which meets the dependency requirements of both packages.

Rerun the install and add the options `` (`ALLOW_DOWNGRADE`ALLOW_UNINSTALL)!(1 0) ``.  This will prevent mamba removing the existing package.  Consequently it is likely that mamba will refuse to install the new package but it will display which library is causing the conflict.



## The operation failed but I don't understand why

Try setting the `VERBOSE` option to 1, 2 or 3 (with increasing verbosity).  The extra logging will show what libmamba is doing in more detail and includes the logging from other libraries which libmamba uses such as curl.  This should make it easier to track down the source of the failure.



## Using the Anaconda `defaults` channels

Not recommended as the libmamba docs [explain](https://mamba.readthedocs.io/en/latest/user_guide/troubleshooting.html#using-the-defaults-channels).



## Mixing the Anaconda `defaults` and `conda-forge` channels

Not recommended as the libmamba docs [explain](https://mamba.readthedocs.io/en/latest/user_guide/troubleshooting.html#mixing-the-defaults-and-conda-forge-channels).

