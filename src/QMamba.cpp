#include <mamba/core/context.hpp>
#include "mamba/core/execution.hpp"
#include "mamba/core/output.hpp"
#include <mamba/core/util_os.hpp>

#include <mamba/util/environment.hpp>

#include <mamba/api/configuration.hpp>
#include <mamba/api/info.hpp>
#include <mamba/api/list.hpp>
#include <mamba/api/create.hpp>
#include <mamba/api/install.hpp>
#include <mamba/api/remove.hpp>
#include <mamba/api/update.hpp>
#include <mamba/api/repoquery.hpp>
#include <mamba/api/clean.hpp>
#include <mamba/api/env.hpp>
#include <mamba/api/constants.hpp>

#include <string>
#include <stdexcept>
#include <functional>
#include <iostream>
#include <fstream>

#ifndef _WIN32
#include <stdlib.h>
#endif

#include "UMambaAuth.h"
#include "UMambaEnv.h"

#include "QMamba.h"
#include "HelperFunctions.h"
#include "Options.h"
#include "CommonOptions.h"


// Currently active environment
mamba::Context::PrefixParams active_prefix_params;

void kdbgetline(std::istream& input_stream, std::string& response)
{
  std::cout << std::endl;
  K result = k(0, (S)"read0 0", (K)0);
  response = std::string((S)kG(result), result->n);
  r0(result);
}

// Each of the libmamba operations follows a similar pattern:
// * Parse the options
// * Create the mamba context
// * Create the mamba config
// * Do specific stuff
// * Handle errors
// 
// Therefore all the common functionality in this helper function, with a callback to perform the specifics
K helper(K k_options, std::vector<AllOptionDefaults> option_defaults, std::function<void(mamba::Configuration&, KdbOptions&)> func)
{
  KDB_EXCEPTION_TRY

  // All mamba operations use both RC and General options.
  option_defaults.push_back(RCOptions::all);
  option_defaults.push_back(GeneralOptions::all);

  K help = NULL;
  auto options = KdbOptions(k_options, { option_defaults }, &help);
  if (help)
    return help;

  mamba::MainExecutor scoped_threads;

  mamba::Context ctx{ {
      /* .enable_logging = */ true,
      /* .enable_signal_handling = */ false,
  } };
  ctx.prefix_params = active_prefix_params;
  ctx.graphics_params.no_progress_bars = options.GetIntOption<bool>(GeneralOptions::NO_PROGRESS_BARS);
  ctx.ascii_only = true;

  mamba::Console console{ ctx, [](std::istream& input_stream, std::string& response) -> void { kdbgetline(input_stream, response); } };
  mamba::Configuration config{ ctx };

  mamba::init_console();
 
  try {
    SetRCOptions(config, options);
    SetGeneralOptions(config, options);

    func(config, options);
  } catch (...) {
    std::cout.flush();
    spdlog::dump_backtrace();
    mamba::reset_console();
    throw;
  }
  
  std::cout.flush();
  mamba::reset_console();

  return (K)0;

  KDB_EXCEPTION_CATCH
}

K info(K k_options)
{
  return helper(k_options,
    { PrefixOptions::all, InfoOptions::all }, 
    [](mamba::Configuration& config, KdbOptions& options) {
      SetPrefixOptions(config, options);

      config.insert(mamba::Configurable("print_licenses", options.GetIntOption<bool>(InfoOptions::LICENSES)));
      config.insert(mamba::Configurable("base", options.GetIntOption<bool>(InfoOptions::BASE)));
      config.insert(mamba::Configurable("environments", options.GetIntOption<bool>(InfoOptions::ENVS)));

      mamba::info(config);
    }
  );
}

K list(K k_options)
{
  return helper(k_options,
    { PrefixOptions::all, ListOptions::all },
    [](mamba::Configuration& config, KdbOptions& options) {
      SetPrefixOptions(config, options);

      std::string regex_str = options.GetStringOption(ListOptions::REGEX).value();
      config.insert(mamba::Configurable("list_regex", regex_str));

      config.insert(mamba::Configurable("full_name", options.GetIntOption<bool>(ListOptions::FULL_NAME)));
      config.insert(mamba::Configurable("no_pip", options.GetIntOption<bool>(ListOptions::NO_PIP)));
      config.insert(mamba::Configurable("reverse", options.GetIntOption<bool>(ListOptions::REVERSE)));
      config.insert(mamba::Configurable("explicit", options.GetIntOption<bool>(ListOptions::EXPLICIT)));
      config.insert(mamba::Configurable("md5", options.GetIntOption<bool>(ListOptions::MD5)));
      config.insert(mamba::Configurable("canonical", options.GetIntOption<bool>(ListOptions::CANONICAL)));
      config.insert(mamba::Configurable("export", options.GetIntOption<bool>(ListOptions::EXPORT)));
      config.insert(mamba::Configurable("revisions", options.GetIntOption<bool>(ListOptions::REVISIONS)));

      mamba::list(config, regex_str);
    }
  );
}

K create(K k_options)
{
  return helper(k_options,
    { PrefixOptions::all, NetworkOptions::all, ChannelOptions::all, InstallCommonOptions::all },
    [](mamba::Configuration& config, KdbOptions& options) {
      SetPrefixOptions(config, options);
      SetNetworkOptions(config, options);
      SetChannelOptions(config, options);
      SetInstallCommonOptions(config, options);

      mamba::create(config);
    }
  );
}

K install(K k_options)
{
  return helper(k_options,
    { PrefixOptions::all, NetworkOptions::all, ChannelOptions::all, InstallCommonOptions::all, InstallOptions::all },
    [](mamba::Configuration& config, KdbOptions& options) {
      SetPrefixOptions(config, options);
      SetNetworkOptions(config, options);
      SetChannelOptions(config, options);
      SetInstallCommonOptions(config, options);

      options.SetIntOption<bool>(InstallOptions::FREEZE_INSTALLED, config.at("freeze_installed"));
      options.SetIntOption<bool>(InstallOptions::FORCE_REINSTALL, config.at("force_reinstall"));

      mamba::install(config);
    }
  );
}

K kremove(K k_options)
{
  return helper(k_options,
    { PrefixOptions::all, RemoveOptions::all },
    [](mamba::Configuration& config, KdbOptions& options) {
      SetPrefixOptions(config, options);

      auto remove_specs = options.GetStringListOption(RemoveOptions::SPECS);
      if (!remove_specs.empty())
        config.at("specs").set_value(remove_specs);

      int flags = 0;
      if (options.GetIntOption<bool>(RemoveOptions::ALL))
        flags |= mamba::MAMBA_REMOVE_ALL;
      if (options.GetIntOption<bool>(RemoveOptions::FORCE))
        flags |= mamba::MAMBA_REMOVE_FORCE;
      if (options.GetIntOption<bool>(RemoveOptions::PRUNE_DEPS))
        flags |= mamba::MAMBA_REMOVE_PRUNE;

      mamba::remove(config, flags);
    }
  );
}

K update(K k_options)
{
  return helper(k_options,
    { PrefixOptions::all, NetworkOptions::all, ChannelOptions::all, InstallCommonOptions::all, UpdateOptions::all },
    [](mamba::Configuration& config, KdbOptions& options) {
      SetPrefixOptions(config, options);
      SetNetworkOptions(config, options);
      SetChannelOptions(config, options);
      SetInstallCommonOptions(config, options);

      auto update_params = mamba::UpdateParams{
        options.GetIntOption<bool>(UpdateOptions::ALL) ? mamba::UpdateAll::Yes : mamba::UpdateAll::No,
        options.GetIntOption<bool>(UpdateOptions::PRUNE_DEPS) ? mamba::PruneDeps::Yes : mamba::PruneDeps::No,
        mamba::EnvUpdate::No,
        mamba::RemoveNotSpecified::No };

      mamba::update(config, update_params);
    }
  );
}

K active(K unused)
{
  K keys = ktn(KS, 2);
  kS(keys)[0] = ss((S)"PREFIX");
  kS(keys)[1] = ss((S)"ROOT_PREFIX");
  K values = ktn(0, 2);
  kK(values)[0] = kp((S)active_prefix_params.target_prefix.string().c_str());
  kK(values)[1] = kp((S)active_prefix_params.root_prefix.string().c_str());
  return xD(keys, values);
}

K activate(K k_options)
{
  return helper(k_options,
    { PrefixOptions::all, ActivateOptions::all },
    [](mamba::Configuration& config, KdbOptions& options) {
      SetPrefixOptions(config, options);

      config.at("target_prefix_checks")
        .set_value(
          mamba::MAMBA_ALLOW_EXISTING_PREFIX |
          mamba::MAMBA_NOT_ALLOW_MISSING_PREFIX |
          mamba::MAMBA_NOT_ALLOW_NOT_ENV_PREFIX |
          mamba::MAMBA_EXPECT_EXISTING_PREFIX
        );
      config.load();

      active_prefix_params.target_prefix = config.context().prefix_params.target_prefix;
      active_prefix_params.root_prefix = config.context().prefix_params.root_prefix;
    }
  );
}

K deactivate(K unused)
{
  active_prefix_params = {};

  return (K)0;
}

K repoquery(K subcomm, K k_options)
{
  if (subcomm->t != -KS)
    return krr((S)"subcomm expected -11h");

  mamba::QueryType type;
  if (std::strcmp(subcomm->s, "search") == 0)
    type = mamba::QueryType::Search;
  else if (std::strcmp(subcomm->s, "whoneeds") == 0)
    type = mamba::QueryType::WhoNeeds;
  else if (std::strcmp(subcomm->s, "depends") == 0)
    type = mamba::QueryType::Depends;
  else
    return krr((S)"subcomm expected `search|`whoneeds|`depends");

  return helper(k_options,
    { PrefixOptions::all, NetworkOptions::all, ChannelOptions::all, RepoqueryOptions::all },
    [type](mamba::Configuration& config, KdbOptions& options) {
      SetPrefixOptions(config, options);
      SetNetworkOptions(config, options);
      SetChannelOptions(config, options);

      auto search_specs = options.GetStringListOption(RepoqueryOptions::SPECS);

      // I don't like the pretty option being turned on when using a wildcard.
      // It makes the output very lengthy particularly since the wildcard means
      // it is typically returning numerous packages.
      // Just turn it off - user can always specify PRETTY option.
      bool specs_has_wildcard = false; // std::any_of(search_specs.cbegin(), search_specs.cend(),
      //  [](std::string_view spec) {
      //    return spec.find('*') != std::string_view::npos;
      //  });

      bool pretty = options.GetIntOption<bool>(RepoqueryOptions::PRETTY);
      bool tree = options.GetIntOption<bool>(RepoqueryOptions::TREE);
      bool recursive = options.GetIntOption<bool>(RepoqueryOptions::RECURSIVE);

      mamba::QueryResultFormat format = mamba::QueryResultFormat::Table;
      if (type == mamba::QueryType::Depends && recursive)
        format = mamba::QueryResultFormat::RecursiveTable;
      if (type == mamba::QueryType::Depends && tree)
        format = mamba::QueryResultFormat::Tree;
      if (type == mamba::QueryType::Search && (pretty || specs_has_wildcard ))
        format = mamba::QueryResultFormat::Pretty;

      bool local = options.GetIntOption<bool>(RepoqueryOptions::LOCAL);
      if (local && type != mamba::QueryType::Search)
        std::cout << "Ignoring LOCAL option for repoquery depends/whoneeds\n" << std::endl;
      bool remote = options.GetIntOption<bool>(RepoqueryOptions::REMOTE);
      if (remote && type == mamba::QueryType::Search)
        std::cout << "Ignoring REMOTE option for repoquery search\n" << std::endl;

      bool channel_passed = options.GetStringListOption(ChannelOptions::CHANNEL).size() > 0;

      bool use_local = type == mamba::QueryType::Search ? local : !(remote || channel_passed);

      auto result = mamba::repoquery(config, type, format, use_local, search_specs);

      if (!result) {
        if (type == mamba::QueryType::Search) {
          if (!local && !channel_passed)
            std::cout << "\nTry looking in a different channel with CHANNEL option." << std::endl;
        } else {
          if (!remote)
            std::cout << "\nTry looking remotely with REMOTE option." << std::endl;
          else if (!channel_passed)
            std::cout << "\nTry looking in a different channel with CHANNEL option." << std::endl;
        }
      }
    }
  );
}

K clean(K k_options)
{
  return helper(k_options,
    { CleanOptions::all },
    [](mamba::Configuration& config, KdbOptions& options) {
      int flags = 0;
      if (options.GetIntOption<bool>(CleanOptions::ALL))
        flags |= mamba::MAMBA_CLEAN_ALL;
      if (options.GetIntOption<bool>(CleanOptions::INDEX_CACHE))
        flags |= mamba::MAMBA_CLEAN_INDEX;
      if (options.GetIntOption<bool>(CleanOptions::PACKAGES))
        flags |= mamba::MAMBA_CLEAN_PKGS;
      if (options.GetIntOption<bool>(CleanOptions::TARBALLS))
        flags |= mamba::MAMBA_CLEAN_TARBALLS;
      if (options.GetIntOption<bool>(CleanOptions::LOCKS))
        flags |= mamba::MAMBA_CLEAN_LOCKS;
      if (options.GetIntOption<bool>(CleanOptions::TRASH))
        flags |= mamba::MAMBA_CLEAN_TRASH;
      if (options.GetIntOption<bool>(CleanOptions::FORCE_PKGS_DIRS)) {
        if (config.at("always_yes").compute().value<bool>()
          || mamba::Console::instance().prompt("Remove all contents from the package caches?"))
        {
          flags |= mamba::MAMBA_CLEAN_FORCE_PKGS_DIRS;
        }
      }

      mamba::clean(config, flags);
    }
  );
}

K env_list(K k_options)
{
  return helper(k_options,
    { PrefixOptions::all },
    [](mamba::Configuration& config, KdbOptions& options) {
      SetPrefixOptions(config, options);

      mamba::print_envs(config);
    }
  );
}

K env_export(K k_options)
{
  std::string output;

  K result = helper(k_options,
    { PrefixOptions::all, EnvExportOptions::all },
    [&output](mamba::Configuration& config, KdbOptions& options) {
      SetPrefixOptions(config, options);

      output = UMambaEnvExport(config,
        options.GetIntOption<bool>(EnvExportOptions::EXPLICIT),
        options.GetIntOption<int>(EnvExportOptions::NO_MD5),
        options.GetIntOption<bool>(EnvExportOptions::NO_BUILD),
        options.GetIntOption<bool>(EnvExportOptions::CHANNEL_SUBDIR),
        options.GetIntOption<bool>(EnvExportOptions::FROM_HISTORY));

      auto filename = options.GetStringOption(EnvExportOptions::TO_FILE);
      if (filename.has_value()) {
        auto outfile = std::ofstream(filename.value(), std::ios_base::out | std::ios_base::trunc);
        outfile << output;
        outfile.flush();
        outfile.close();
        output.clear();
      }
    }
  );

  if (!result && !output.empty())
    result = kp((S)output.c_str());

  return result;
}

K env_remove(K k_options)
{
  bool removed_active = false;

  K result = helper(k_options,
    { PrefixOptions::all, EnvRemoveOptions::all },
    [&removed_active](mamba::Configuration& config, KdbOptions& options) {
      SetPrefixOptions(config, options);

      auto success = UMambaEnvRemove(config);
      if (success && active_prefix_params.target_prefix == config.context().prefix_params.target_prefix)
        removed_active = true;
    }
  );

  if (!result)
    result = kb(removed_active);

  return result;
}

K env_update(K k_options)
{
  return helper(k_options,
    { PrefixOptions::all, EnvUpdateOptions::all },
    [](mamba::Configuration& config, KdbOptions& options) {
      SetPrefixOptions(config, options);

      options.SetStringListOption(EnvUpdateOptions::FILE, config.at("file_specs"));
      bool remove_not_specified = options.GetIntOption<bool>(EnvUpdateOptions::PRUNE);

      auto update_params = mamba::UpdateParams{
        mamba::UpdateAll::No,
        mamba::PruneDeps::Yes,
        mamba::EnvUpdate::Yes,
        remove_not_specified ? mamba::RemoveNotSpecified::Yes : mamba::RemoveNotSpecified::No,
      };

      mamba::update(config, update_params);
    }
  );
}

K auth_login(K k_options)
{
  KDB_EXCEPTION_TRY

  K help = NULL;
  auto options = KdbOptions(k_options, { AuthLoginOptions::all }, &help);
  if (help)
    return help;

  auto host = options.GetStringOption(AuthLoginOptions::HOST).value_or("");
  auto username = options.GetStringOption(AuthLoginOptions::USERNAME).value_or("");
  auto password = options.GetStringOption(AuthLoginOptions::PASSWORD).value_or("");
  auto token = options.GetStringOption(AuthLoginOptions::TOKEN).value_or("");
  auto bearer = options.GetStringOption(AuthLoginOptions::BEARER).value_or("");

  UMambaAuthLogin(host, username, password, token, bearer);

  return (K)0;

  KDB_EXCEPTION_CATCH
}

K auth_logout(K k_options)
{
  KDB_EXCEPTION_TRY

  K help = NULL;
  auto options = KdbOptions(k_options, { AuthLogoutOptions::all }, &help);
  if (help)
    return help;

  auto host = options.GetStringOption(AuthLogoutOptions::HOST).value_or("");
  auto all = options.GetIntOption<bool>(AuthLogoutOptions::ALL);

  UMambaAuthLogout(host, all);

  return (K)0;

  KDB_EXCEPTION_CATCH
}


K get_prefix(K k_options)
{
  std::string prefix;

  K result = helper(k_options,
    { PrefixOptions::all },
    [&prefix](mamba::Configuration& config, KdbOptions& options) {
      SetPrefixOptions(config, options);

      config.load();
      prefix = config.context().prefix_params.target_prefix;
    }
  );

  if (!result)
    result = kp((S)prefix.c_str());

  return result;
}

K qbase(K unused)
{
  std::string exepath;
  exepath.resize(1024);

#ifdef _WIN32
  GetModuleFileName(NULL, &exepath.at(0), (int)exepath.size());
  auto sep = exepath.find_last_of('\\');
  if (sep != std::string::npos) {
    exepath.resize(sep);
    sep = exepath.find_last_of('\\');
  }
#else
  std::string exelink = "/proc/" + std::to_string(getpid()) + "/exe";
  readlink(exelink.c_str(), &exepath.at(0), exepath.size());
  auto sep = exepath.find_last_of('/');
  if (sep != std::string::npos) {
    exepath.resize(sep);
    sep = exepath.find_last_of('/');
  }
#endif // _WIN32

  if (sep != std::string::npos)
    exepath.resize(sep);
  
  return kp((S)exepath.c_str());
}

K kexport(K unused)
{
  K key = ktn(KS, 0);
  K value = ktn(0, 0);

#define REG_RENAME(func, name, args)  \
  js(&key, ss((S)#name));             \
  jk(&value, dl((V*)func, args));
#define REG(func, args) REG_RENAME(func, func, args);

  REG(info, 1);
  REG(list, 1);
  REG(repoquery, 2);
  REG(create, 1);
  REG(install, 1);
  REG_RENAME(kremove, remove, 1);
  REG(update, 1);
  REG(active, 1);
  REG(activate, 1);
  REG(deactivate, 1);
  REG(clean, 1);
  REG(env_list, 1);
  REG(env_export, 1);
  REG(env_remove, 1);
  REG(env_update, 1);
  REG(auth_login, 1);
  REG(auth_logout, 1);
  REG(get_prefix, 1);
  REG(qbase, 1);

  return xD(key, value);
}
