clib:use`.clib

// Options helpers
root_prefix:clib.qbase[], "/root"
defaults:(enlist `ROOT_PREFIX)!(enlist root_prefix)
makeOptions:{$[99h = type x; defaults, x; 101h = type x; defaults; `help ~ x; x; '"type"]}
makeOptionsSpecs:{specs:$[0 = type x; (enlist `SPECS)!enlist x; x]; makeOptions[specs]}
makeOptionsName:{name:$[10 = type x; (enlist `NAME)!enlist x; x]; makeOptions[name]}
makeOptionsFile:{file:$[0 = type x; (enlist `FILE)!enlist x; x]; makeOptions[file]}

// Guard rails for already loaded modules
// GUARD_IN_USE can be one of "permit", "warn", "prompt" or "deny"
guard_change_default:(enlist `GUARD_IN_USE)!(enlist "warn")
guard_activate_default:(enlist `GUARD_IN_USE)!(enlist "prompt")
getLoaded:{[prefix]select from .Q.m.M where p like (prefix, "/q/mod/*")}
prompt:{[guard]
    result:$[
        "deny" ~ guard; 0; 
        "warn" ~ guard; 1;
        [-1 "Are you sure you wish to continue (y/N)?"; input:read0 0; (upper input) ~ enlist "Y"]
        ];
    if [not result; -1 "Aborting";];
    result
    }
checkInUse:{[options]
    if [options ~ `help; :1];

    prefix:clib.get_prefix[`PREFIX`ROOT_PREFIX`NAME # options];
    $[
        (0 < count loaded:getLoaded[prefix]) and
        (not "permit" ~ guard:raze string (guard_change_default, options)[`GUARD_IN_USE]) and
        (not .z.o like "w*");
        [
            -1 "\nWarning: You are attempting to make changes to the prefix \"", prefix, "\"";
            -1 "from which you have already loaded the modules:\n";
            show loaded;
            -1 "\nThis may cause unpredictable results if the changes affect those modules or their dependencies.\n";
            prompt[guard]
        ];
        1
        ]
    }

// Top level commands
info:{clib.info[makeOptions[x]]}
list:{clib.list[makeOptions[x]]}
install:{options:makeOptionsSpecs[x]; if [checkInUse[options]; :clib.install[options]]}
create:{options:makeOptionsName[x]; if [checkInUse[options]; :clib.create[options]]}
remove:{options:makeOptionsSpecs[x]; if [checkInUse[options]; :clib.remove[options]]}
.z.m.update:{options:makeOptionsSpecs[x]; if [checkInUse[options]; :clib.update[options]]}
clean:clib.clean

// Auth subcommands
auth.login:clib.auth_login
auth.logout:clib.auth_logout

// Repoquery subcommands
repoquery.search:search:{clib.repoquery[`search; makeOptionsSpecs[x]]}
repoquery.whoneeds:{clib.repoquery[`whoneeds; makeOptionsSpecs[x]]}
repoquery.depends:{clib.repoquery[`depends; makeOptionsSpecs[x]]}

// Env subcommands
env.list:{clib.env_list[makeOptions[x]]}
env.export:{clib.env_export[makeOptionsName[x]]}
env.remove:{options:makeOptionsName[x]; if [checkInUse[options]; result:clib.env_remove[options]; $[-1h = type result; if [result ~ 1b; activate (`NAME`GUARD_IN_USE)!("base"; "permit")]; :result]]}
env.update:{options:makeOptionsFile[x]; if [checkInUse[options]; :clib.env_update[options]]}

// Active environments and SPs
active:{clib.active[][`PREFIX]}
searchPaths:{[prefix]$[count prefix; enlist prefix, "/lib/q/mod"; ()]}
activate:{
    if [x ~ `help; :clib.activate[x]];

    options:makeOptionsName[x];
    old_prefix:active[];
    activate_prefix:clib.get_prefix[`PREFIX`ROOT_PREFIX`NAME # options];
    continue:$[
        (not old_prefix ~ activate_prefix) and
        (0 < count loaded:getLoaded[old_prefix]) and
        (not "permit" ~ guard:raze string (guard_activate_default, options)[`GUARD_IN_USE]) and
        (not .z.o like "w*") and
        count old_prefix;
        [
            -1 "\nWarning: You have already used modules from the old prefix \"", old_prefix, "\":\n";
            show loaded;
            -1 "\nFor a given q session, all modules with shared object dependencies should be loaded from the same environment.";
            -1 "If you intend to load modules from the new prefix \"", activate_prefix, "\", it is recommended that you restart q before doing so.";
            -1 "\nOtherwise unpredictable results can occur if binary incompatibilities exist between any dependencies present in both environments.\n";
            prompt[guard]
        ];
        1
        ];
    if [continue;
        old_sp:searchPaths[old_prefix];
        res:clib.activate[options];
        new_prefix:active[];
        if [count old_sp; -1 "Activated environment at prefix: ", new_prefix;];
        .Q.m.SP:.Q.m.SP except old_sp;
        if [count new_sp:searchPaths[new_prefix]; .Q.m.SP:new_sp, .Q.m.SP];
        `CONDA_PREFIX setenv new_prefix;
        // On Windows may need to add "{new_prefix}\lib" to the PATH since it doesn't have a RUNPATH
        :res
        ]
    }
deactivate:{activate["base"]} // Activate/deactivate could use a stack?

// Initialisation
common_packages:("libgcc"; "libstdcxx")                 // used by a lot of things
compatibility_packages:("openssl"; "libcurl"; "curl")   // kdb loads libssl on startup, curl is tied to the libssl version
module_packages:enlist "libxml2 < 2.14"                 // used by kurl and objstor
root_packages:common_packages, compatibility_packages, module_packages
root_lib_path:root_prefix, "/lib"
lib_path_env:$[.z.o like "l*"; `LD_LIBRARY_PATH; .z.o like "m*"; `DYLD_LIBRARY_PATH; .z.o like "w*"; `PATH; '"Unknown architecture"]
showStartup:{
    if [not .z.o like "w*";
        -1 "\nPlease exit q then start it with (set an alias):";
        -1 "\n", (string lib_path_env), "=", root_lib_path, " ", clib.qbase[], "/bin/q\n";
        ];
    }
initialiseRoot:{
    if [not .z.o like "w*";
        init_channel:$[count channel:getenv `QMAMBA_INIT_CHANNEL; channel; "conda-forge"];
        -1 "\nInstalling ", (2 _ raze {", ", x} each root_packages), " from ", init_channel, " into root prefix \"", root_prefix, "\"...";
        install (`PREFIX`ROOT_PREFIX`SPECS`CHANNEL`ALLOW_DOWNGRADE`ALLOW_UNINSTALL`FORCE_REINSTALL`YES`QUIET)!
            (root_prefix; root_prefix; root_packages; enlist init_channel; 1; 1; 1; 1; 1);
        -1 "Done";
        showStartup[];
        ];
    }
checkStartup:{
    if [not .z.o like "w*";
        $[(not (getenv lib_path_env) like root_lib_path, "*");
            [
                -1 "\nThe start of your $", (string lib_path_env), " does not specify the root prefix libraries.";
                -1 "Some modules may fail to load or have unpredictable results.";
                showStartup[]
            ];
            `PATH setenv root_prefix, "/bin:", getenv `PATH  // just in case they system "openssl" or "curl"
            ];
        ];
    }
init:{
    $[not count key `$(":", root_prefix);
        [
            -1 "\nThis is the first time you have used qmamba, initialising.";
            -1 "\nCreating root prefix \"", root_prefix, "\"";
            initialiseRoot[]
        ];
        checkStartup[]
        ];
    activate["base"]
    }
init[]

export:`0clib _ .z.m
