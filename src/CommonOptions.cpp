#include <mamba/api/configuration.hpp>

#include "HelperFunctions.h"
#include "Options.h"
#include "OptionsParser.h"
#include "CommonOptions.h"


void SetRCOptions(mamba::Configuration& config, const KdbOptions& options)
{
  options.SetIntOption<bool>(RCOptions::NO_RC, config.at("no_rc"));
  options.SetIntOption<bool>(RCOptions::NO_ENV, config.at("no_env"));

  auto rc_file = options.GetStringListOption(RCOptions::RC_FILE);
  if (rc_file.size()) {
    std::vector<mamba::fs::u8path> rc_paths;
    for (const auto& i : rc_file)
      rc_paths.push_back(i);
    config.at("rc_files").set_value(rc_paths);
  }
}

void SetGeneralOptions(mamba::Configuration& config, const KdbOptions& options)
{
  options.SetIntOption<int>(GeneralOptions::VERBOSE, config.at("verbose"));
  options.SetIntOption<bool>(GeneralOptions::QUIET, config.at("quiet"));
  options.SetIntOption<bool>(GeneralOptions::YES, config.at("always_yes"));
  options.SetIntOption<bool>(GeneralOptions::OFFLINE, config.at("offline"));
  options.SetIntOption<bool>(GeneralOptions::DRY_RUN, config.at("dry_run"));
}

void SetPrefixOptions(mamba::Configuration& config, const KdbOptions& options)
{
  // Work out root_prefix and target_prefix ourselves.
  // If libmamba does this it will use various overrides which is confusing.
  auto& prefix_params = config.context().prefix_params;

  // Root prefix priority:
  // 1. From ROOT_PREFIX option
  // 2. From active environment
  // 3. Error
  auto root_prefix = mamba::fs::u8path(options.GetStringOption(PrefixOptions::ROOT_PREFIX).value_or(""));
  if (root_prefix.empty())
    root_prefix = prefix_params.root_prefix;
  if (root_prefix.empty())
    throw KdbOptions::InvalidOption("No ROOT_PREFIX set");

  // Target prefix priority:
  // 1. From PREFIX option
  // 2  From NAME option
  // 3. From active environment
  // 4. Same as root_prefix
  auto target_prefix = mamba::fs::u8path(options.GetStringOption(PrefixOptions::PREFIX).value_or(""));
  auto name = options.GetStringOption(PrefixOptions::NAME).value_or("");
  if (target_prefix.empty() && !name.empty()) {
    if (name != "base")
      target_prefix = root_prefix / "envs" / name;
    else
      target_prefix = root_prefix;
  }
  if (target_prefix.empty())
    target_prefix = prefix_params.target_prefix;
  if (target_prefix.empty())
    target_prefix = root_prefix;

  config.at("root_prefix").set_value(root_prefix);
  config.at("target_prefix").set_value(target_prefix);
  options.SetStringOption<std::string>(PrefixOptions::NAME, config.at("env_name"));
  options.SetStringOption<mamba::fs::u8path>(PrefixOptions::RELOCATE_PREFIX, config.at("relocate_prefix"));

  // Make sure the root environment (e.g. $QHOME/root) is a proper conda env
  config.at("create_base").set_value(true);
}

void SetNetworkOptions(mamba::Configuration& config, const KdbOptions& options)
{
  options.SetStringOption<std::string>(NetworkOptions::SSL_VERIFY, config.at("ssl_verify"));
  options.SetIntOption<bool>(NetworkOptions::SSL_NO_REVOKE, config.at("ssl_no_revoke"));
  options.SetStringOption<std::string>(NetworkOptions::CACERT_PATH, config.at("cacert_path"));
  options.SetIntOption<size_t>(NetworkOptions::LOCAL_REPODATA_TTL, config.at("local_repodata_ttl"));
  options.SetIntOption<bool>(NetworkOptions::RETRY_CACHE_CLEAN, config.at("retry_clean_cache"));
}

void SetChannelOptions(mamba::Configuration& config, const KdbOptions& options)
{
  options.SetStringListOption(ChannelOptions::CHANNEL, config.at("channels"));
  options.SetStringOption<std::string>(ChannelOptions::CHANNEL_ALIAS, config.at("channel_alias"));
  options.SetIntOption<bool>(ChannelOptions::VERIFY_ARTIFACTS, config.at("verify_artifacts"));
  options.SetStringListOption(ChannelOptions::TRUSTED_CHANNELS, config.at("trusted_channels"));

  std::map<std::string, mamba::ChannelPriority> cp_map = { { "disabled", mamba::ChannelPriority::Disabled },
                                                           { "flexible", mamba::ChannelPriority::Flexible },
                                                           { "strict", mamba::ChannelPriority::Strict } };
  auto cp = options.GetStringOption(ChannelOptions::CHANNEL_PRIORITY);
  if (cp.has_value()) {
    auto cp_value = cp_map.find(cp.value());
    if (cp_value == cp_map.end())
      throw KdbOptions::InvalidOption("Invalid CHANNEL_PRIORITY value");
    config.at("channel_priority").set_value(cp_value->second);
  }
}

void SetInstallCommonOptions(mamba::Configuration& config, const KdbOptions& options)
{
  options.SetStringListOption(InstallCommonOptions::SPECS, config.at("specs"));
  options.SetStringListOption(InstallCommonOptions::FILE, config.at("file_specs"));

  options.SetIntOption<bool>(InstallCommonOptions::ALLOW_UNINSTALL, config.at("allow_uninstall"));
  options.SetIntOption<bool>(InstallCommonOptions::ALLOW_DOWNGRADE, config.at("allow_downgrade"));
  options.SetIntOption<bool>(InstallCommonOptions::ALWAYS_COPY, config.at("always_copy"));

  std::map<std::string, mamba::VerificationLevel> vl_map = { { "enabled", mamba::VerificationLevel::Enabled },
                                                             { "warn", mamba::VerificationLevel::Warn },
                                                             { "disabled", mamba::VerificationLevel::Disabled } };
  auto vl = options.GetStringOption(InstallCommonOptions::SAFETY_CHECKS);
  if (vl.has_value()) {
    auto vl_value = vl_map.find(vl.value());
    if (vl_value == vl_map.end())
      throw KdbOptions::InvalidOption("Invalid SAFETY_CHECKS value");
    config.at("safety_checks").set_value(vl_value->second);
  }

}
