#pragma once

#include "OptionsParser.h"


// Create AllOptionDefaults from int, string, stringlist components
inline AllOptionDefaults MakeAllOptionDefaults(
  const IntOptionDefaults& int_option_defaults,
  const StringOptionDefaults& string_option_defaults,
  const StringListOptionDefaults& stringlist_option_defaults) {
  return AllOptionDefaults{ int_option_defaults, string_option_defaults, stringlist_option_defaults };
}

namespace RCOptions
{
  // Int options
  const std::string NO_ENV = "NO_ENV";
  const std::string NO_RC = "NO_RC";

  const static IntOptionDefaults int_option_defaults = {
    {NO_ENV, {1, "Disable the use of environment variables"}},
    {NO_RC, {1, "Disable the use of configuration files"}},
  };

  // Stringlist options
  const std::string RC_FILE = "RC_FILE";

  const static StringListOptionDefaults stringlist_option_defaults = {
    {RC_FILE, {{}, "Paths to the configuration files to use"}},
  };

  const static auto all = MakeAllOptionDefaults(int_option_defaults, {}, stringlist_option_defaults);
}

namespace GeneralOptions
{
  // Int options
  const std::string VERBOSE = "VERBOSE";
  const std::string QUIET = "QUIET";
  const std::string YES = "YES";
  const std::string OFFLINE = "OFFLINE";
  const std::string DRY_RUN = "DRY_RUN";
  const std::string NO_PROGRESS_BARS = "NO_PROGRESS_BARS";

  const static IntOptionDefaults int_option_defaults = {
    {VERBOSE, {0, "Set verbosity (0, 1 or 2 with increasing verbosity)"}},
    {QUIET, {0, "Set quiet mode (print less output)"}},
    {YES, {0, "Automatically answer yes on prompted questions"}},
    {OFFLINE, {0, "Force use cached repodata"}},
    {DRY_RUN, {0, "Only display what would have been done"}},
    {NO_PROGRESS_BARS, {0, "Disable the use of progress bars in the output"}},
  };

  const static auto all = MakeAllOptionDefaults(int_option_defaults, {}, {});
}

namespace PrefixOptions
{
  // String options
  const std::string PREFIX = "PREFIX";
  const std::string ROOT_PREFIX = "ROOT_PREFIX";
  const std::string NAME = "NAME";
  const std::string RELOCATE_PREFIX = "RELOCATE_PREFIX";

  const static StringOptionDefaults string_option_defaults = {
    {PREFIX, {{}, "Path to the target prefix"}},
    {ROOT_PREFIX, {{}, "Path to the root prefix"}},
    {NAME, {{}, "Name of the target prefix"}},
    {RELOCATE_PREFIX, {{}, "Path to the relocation prefix"}},
  };

  const static auto all = MakeAllOptionDefaults({}, string_option_defaults, {});
}

namespace NetworkOptions
{
  // Int options
  const std::string SSL_NO_REVOKE = "SSL_NO_REVOKE";
  const std::string LOCAL_REPODATA_TTL = "LOCAL_REPODATA_TTL";
  const std::string RETRY_CACHE_CLEAN = "RETRY_CACHE_CLEAN";

  const static IntOptionDefaults int_option_defaults = {
    {SSL_NO_REVOKE, {0, "SSL certificate revocation checks"}},
    {LOCAL_REPODATA_TTL, {1, "Repodata time-to-live"}},
    {RETRY_CACHE_CLEAN, {0, "If solve fails, try to fetch updated repodata"}},
  };

  // String options
  const std::string SSL_VERIFY = "SSL_VERIFY";
  const std::string CACERT_PATH = "CACERT_PATH";

  const static StringOptionDefaults string_option_defaults = {
    {SSL_VERIFY, {{}, "Verify SSL certificates for HTTPS requests ('<false>' or PATH)"}},
    {CACERT_PATH, {{}, "Path (file or directory) to SSL certificate(s)"}},
  };

  const static auto all = MakeAllOptionDefaults(int_option_defaults, string_option_defaults, {});
}

namespace ChannelOptions
{
  // Int options
  const std::string VERIFY_ARTIFACTS = "VERIFY_ARTIFACTS";

  const static IntOptionDefaults int_option_defaults = {
    {VERIFY_ARTIFACTS, {0, "Run verifications on packages signatures (experimental)"}},
  };

  // String options
  const std::string CHANNEL_ALIAS = "CHANNEL_ALIAS";
  const std::string CHANNEL_PRIORITY = "CHANNEL_PRIORITY";

  const static StringOptionDefaults string_option_defaults = {
    {CHANNEL_ALIAS, {{}, "The prepended url location to associate with channel names"}},
    {CHANNEL_PRIORITY, {"flexible", "Define the channel priority ('disabled', 'flexible' or 'strict')"}},
  };

  // Stringlist options
  const std::string CHANNEL = "CHANNEL";
  const std::string TRUSTED_CHANNELS = "TRUSTED_CHANNELS";

  const static StringListOptionDefaults stringlist_option_defaults = {
    {CHANNEL, {{}, "Define the list of channels"}},
    {TRUSTED_CHANNELS, {{}, "The list of trusted channels allowing artifacts verification (see VERIFY_ARTIFACTS)"}},
  };

  const static auto all = MakeAllOptionDefaults(int_option_defaults, string_option_defaults, stringlist_option_defaults);
}

namespace InstallCommonOptions
{
  // Int options
  const std::string ALLOW_UNINSTALL = "ALLOW_UNINSTALL";
  const std::string ALLOW_DOWNGRADE = "ALLOW_DOWNGRADE";
  const std::string ALWAYS_COPY = "ALWAYS_COPY";

  const static IntOptionDefaults int_option_defaults = {
    {ALLOW_UNINSTALL, {1, "Allow uninstall when installing or updating packages"}},
    {ALLOW_DOWNGRADE, {0, "Allow downgrade when installing packages"}},
    {ALWAYS_COPY, {0, "Use copy instead of hard-link"}},
  };

  // String options
  const std::string SAFETY_CHECKS = "SAFETY_CHECKS";
  const std::string GUARD_IN_USE = "GUARD_IN_USE";

  const static StringOptionDefaults string_option_defaults = {
    {SAFETY_CHECKS, {"warn", "Safety checks policy ('enabled', 'warn' or 'disabled')"}},
    {GUARD_IN_USE, {{}, "Guard against making changes to an in use prefix ('permit', 'warn', 'prompt' or 'deny')"}},
  };

  // Stringlist options
  const std::string SPECS = "SPECS";
  const std::string FILE = "FILE";

  const static StringListOptionDefaults stringlist_option_defaults = {
    {SPECS, {{}, "Specs to install/update into the environment"}},
    {FILE, {{}, "File(s) providing package specifications (yaml or json)"}},
  };

  const static auto all = MakeAllOptionDefaults(int_option_defaults, string_option_defaults, stringlist_option_defaults);
}

namespace InstallOptions
{
  // Int options
  const std::string FREEZE_INSTALLED = "FREEZE_INSTALLED";
  const std::string FORCE_REINSTALL = "FORCE_REINSTALL";

  const static IntOptionDefaults int_option_defaults = {
    {FREEZE_INSTALLED, {0, "Freeze already installed dependencies"}},
    {FORCE_REINSTALL, {0, "Force reinstall of package"}},
  };

  const static auto all = MakeAllOptionDefaults(int_option_defaults, {}, {});
}

namespace RemoveOptions
{
  // Int options
  const std::string ALL = "ALL";
  const std::string FORCE = "FORCE";
  const std::string PRUNE_DEPS = "PRUNE_DEPS";

  const static IntOptionDefaults int_option_defaults = {
    {ALL, {0, "Remove all packages in the environment"}},
    {FORCE, {0, "Force removal of package (env consistency not guaranteed!)"}},
    {PRUNE_DEPS, {1, "Prune dependencies"}},
  };

  // String options
  const std::string GUARD_IN_USE = "GUARD_IN_USE";

  const static StringOptionDefaults string_option_defaults = {
    {GUARD_IN_USE, {{}, "Guard against making changes to an in use prefix ('permit', 'warn', 'prompt' or 'deny')"}},
  };

  // Stringlist options
  const std::string SPECS = "SPECS";

  const static StringListOptionDefaults stringlist_option_defaults = {
    {SPECS, {{}, "Specs to remove from the environment"}},
  };

  const static auto all = MakeAllOptionDefaults(int_option_defaults, string_option_defaults, stringlist_option_defaults);
}

namespace UpdateOptions
{
  // Int options
  const std::string PRUNE_DEPS = "PRUNE_DEPS";
  const std::string ALL = "ALL";

  const static IntOptionDefaults int_option_defaults = {
    {PRUNE_DEPS, {1, "Prune dependencies"}},
    {ALL, {0, "Update all packages in the environment"}},
  };

  // String options
  const std::string GUARD_IN_USE = "GUARD_IN_USE";

  const static StringOptionDefaults string_option_defaults = {
    {GUARD_IN_USE, {{}, "Guard against making changes to an in use prefix ('permit', 'warn', 'prompt' or 'deny')"}},
  };

  const static auto all = MakeAllOptionDefaults(int_option_defaults, string_option_defaults, {});
}

namespace ActivateOptions
{
  // String options
  const std::string GUARD_IN_USE = "GUARD_IN_USE";

  const static StringOptionDefaults string_option_defaults = {
    {GUARD_IN_USE, {{}, "Guard against modules being in use from the old prefix ('permit', 'warn', 'prompt' or 'deny')"}},
  };

  const static auto all = MakeAllOptionDefaults({}, string_option_defaults, {});
}

namespace InfoOptions
{
  // Int options
  const std::string LICENSES = "LICENSES";
  const std::string BASE = "BASE";
  const std::string ENVS = "ENVS";

  const static IntOptionDefaults int_option_defaults = {
    {LICENSES, {0, "Print licenses"}},
    {BASE, {0, "Display base environment path"}},
    {ENVS, {0, "List known environments"}}
  };

  const static auto all = MakeAllOptionDefaults(int_option_defaults, {}, {});
}

namespace ListOptions
{
  // Int options
  const std::string FULL_NAME = "FULL_NAME";
  const std::string NO_PIP = "NO_PIP";
  const std::string REVERSE = "REVERSE";
  const std::string EXPLICIT = "EXPLICIT";
  const std::string MD5 = "MD5";
  const std::string CANONICAL = "CANONICAL";
  const std::string EXPORT = "EXPORT";
  const std::string REVISIONS = "REVISIONS";

  const static IntOptionDefaults int_option_defaults = {
    {FULL_NAME, {0, "Only search for full names, i.e., ^<regex>$"}},
    {NO_PIP, {0, "Do not include pip-only installed packages"}},
    {REVERSE, {0, "List installed packages in reverse order"}},
    {EXPLICIT, {0, "List explicitly all installed packages with URL"}},
    {MD5, {0, "Add MD5 hashsum when using EXPLICIT"}},
    {CANONICAL, {0, "Output canonical names of packages only"}},
    {EXPORT, {0, "Output explicit, machine-readable requirement strings"}},
    {REVISIONS, {0, "List the revision history"}},
  };

  // String options
  const std::string REGEX = "REGEX";

  const static StringOptionDefaults string_option_defaults = {
    {REGEX, {"", "List only packages matching a regular expression"}}
  };

  const static auto all = MakeAllOptionDefaults(int_option_defaults, string_option_defaults, {});
}

namespace RepoqueryOptions
{
  // Int options
  const std::string TREE = "TREE";
  const std::string RECURSIVE = "RECURSIVE";
  const std::string PRETTY = "PRETTY";
  const std::string LOCAL = "LOCAL";
  const std::string REMOTE = "REMOTE";

  const static IntOptionDefaults int_option_defaults = {
    {TREE, {0, "Show result as a tree (depends)"}},
    {RECURSIVE, {0, "Show dependencies recursively (depends)"}},
    {PRETTY, {0, "Pretty print result with more detail for each package (search)"}},
    {LOCAL, {0, "Query local prefix instead of remote repositories (search)"}},
    {REMOTE, {0, "Query remote repositories instead of local prefix (depends or whoneeds)"}},
  };

  // Stringlist options
  const std::string SPECS = "SPECS";

  const static StringListOptionDefaults stringlist_option_defaults = {
    {SPECS, {{}, "Specs to query"}},
  };

  const static auto all = MakeAllOptionDefaults(int_option_defaults, {}, stringlist_option_defaults);
}

namespace AuthLoginOptions
{
  // String options
  const std::string HOST = "HOST";
  const std::string USERNAME = "USERNAME";
  const std::string PASSWORD = "PASSWORD";
  const std::string TOKEN = "TOKEN";
  const std::string BEARER = "BEARER";

  const static StringOptionDefaults string_option_defaults = {
    {HOST, {{}, "Host for the account"}},
    {USERNAME, {{}, "User name for the account"}},
    {PASSWORD, {{}, "Password for the account"}},
    {TOKEN, {{}, "Token for the account"}},
    {BEARER, {{}, "Bearer token for the account"}},
  };

  const static auto all = MakeAllOptionDefaults({}, string_option_defaults, {});
}

namespace AuthLogoutOptions
{
  // Int options
  const std::string ALL = "ALL";

  const static IntOptionDefaults int_option_defaults = {
    {ALL, {0, "Log out from all hosts"}},
  };

  // String options
  const std::string HOST = "HOST";

  const static StringOptionDefaults string_option_defaults = {
    {HOST, {{}, "Host for the account"}},
  };

  const static auto all = MakeAllOptionDefaults(int_option_defaults, string_option_defaults, {});
}

namespace CleanOptions
{
  // Int options
  const std::string ALL = "ALL";
  const std::string INDEX_CACHE = "INDEX_CACHE";
  const std::string PACKAGES = "PACKAGES";
  const std::string TARBALLS = "TARBALLS";
  const std::string LOCKS = "LOCKS";
  const std::string TRASH = "TRASH";
  const std::string FORCE_PKGS_DIRS = "FORCE_PKGS_DIRS";

  const static IntOptionDefaults int_option_defaults = {
    {ALL, {0, "Remove index cache, lock files, unused cache packages, and tarballs"}},
    {INDEX_CACHE, {0, "Remove index cache"}},
    {PACKAGES, {0, "Remove unused packages from writable package caches"}},
    {TARBALLS, {0, "Remove cached package tarballs"}},
    {LOCKS, {0, "Remove lock files from caches"}},
    {TRASH, {0, "Remove *.mamba_trash files from all environments"}},
    {FORCE_PKGS_DIRS, {0, "Remove *all* writable package caches (not included with ALL option)"}},
  };

  const static auto all = MakeAllOptionDefaults(int_option_defaults, {}, {});
}

namespace EnvExportOptions
{
  // Int options
  const std::string EXPLICIT = "EXPLICIT";
  const std::string NO_MD5 = "NO_MD5";
  const std::string NO_BUILD = "NO_BUILD";
  const std::string CHANNEL_SUBDIR = "CHANNEL_SUBDIR";
  const std::string FROM_HISTORY = "FROM_HISTORY";

  const static IntOptionDefaults int_option_defaults = {
    {EXPLICIT, {0, "Use explicit format"}},
    {NO_MD5, {0, "Disable md5"}},
    {NO_BUILD, {0, "Disable the build string in spec"}},
    {CHANNEL_SUBDIR, {0, "Enable channel/subdir in spec"}},
    {FROM_HISTORY, {0, "Build environment spec from explicit specs in history"}},
  };

  // String options
  const std::string TO_FILE = "TO_FILE";

  const static StringOptionDefaults string_option_defaults = {
    {TO_FILE, {{}, "Write output to the specified file"}},
  };

  const static auto all = MakeAllOptionDefaults(int_option_defaults, string_option_defaults, {});
}

namespace EnvUpdateOptions
{
  // Int options
  const std::string PRUNE = "PRUNE";

  const static IntOptionDefaults int_option_defaults = {
    {PRUNE, {0, "Remove installed packages not specified in the command and in environment file"}},
  };

  // String options
  const std::string GUARD_IN_USE = "GUARD_IN_USE";

  const static StringOptionDefaults string_option_defaults = {
    {GUARD_IN_USE, {{}, "Guard against making changes to an in use prefix ('permit', 'warn', 'prompt' or 'deny')"}},
  };

  // Stringlist options
  const std::string FILE = "FILE";

  const static StringListOptionDefaults stringlist_option_defaults = {
    {FILE, {{}, "File(s) providing package specifications (yaml or json)"}},
  };

  const static auto all = MakeAllOptionDefaults(int_option_defaults, string_option_defaults, stringlist_option_defaults);
}

namespace EnvRemoveOptions
{
  // String options
  const std::string GUARD_IN_USE = "GUARD_IN_USE";

  const static StringOptionDefaults string_option_defaults = {
    {GUARD_IN_USE, {{}, "Guard against making changes to an in use prefix ('permit', 'warn', 'prompt' or 'deny')"}},
  };

  const static auto all = MakeAllOptionDefaults({}, string_option_defaults, {});
}
