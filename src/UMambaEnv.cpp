/////////////////////////////////////////////////////////////////////////
// Env export and remove are not native to libmamba                    //
// These implementations are based on the micromamba code:             //
// https://github.com/mamba-org/mamba/blob/main/micromamba/src/env.cpp //
/////////////////////////////////////////////////////////////////////////

#include <mamba/core/util.hpp>
#include "mamba/core/environments_manager.hpp"
#include "mamba/core/channel_context.hpp"
#include "mamba/core/prefix_data.hpp"

#include <mamba/api/configuration.hpp>
#include <mamba/api/remove.hpp>
#include <mamba/api/env.hpp>

#include "mamba/util/path_manip.hpp"
#include "mamba/util/string.hpp"

#include <string>
#include <sstream>
#include <iostream>


std::string UMambaEnvExport(mamba::Configuration& config,
  bool explicit_format, int no_md5, bool no_build, bool channel_subdir, bool from_history)
{
  auto& ctx = config.context();
  config.load();

  auto channel_context = mamba::ChannelContext::make_conda_compatible(ctx);

  auto json_format = config.at("json").get_cli_config<bool>();

  // Raise a warning if `--json` and `--explicit` are used together.
  if (json_format && explicit_format)
  {
    std::cerr << "Warning: `--json` and `--explicit` are used together but are incompatible. The `--json` flag will be ignored."
      << std::endl;
  }

  std::stringstream output;
  if (explicit_format)
  {
    // TODO: handle error
    auto pd = mamba::PrefixData::create(ctx.prefix_params.target_prefix, channel_context)
      .value();
    auto records = pd.sorted_records();
    output << "# This file may be used to create an environment using:\n"
      << "# $ conda create --name <env> --file <this file>\n"
      << "# platform: " << ctx.platform << "\n"
      << "@EXPLICIT\n";

    for (const auto& record : records)
    {
      output <<  //
        mamba::specs::CondaURL::parse(record.package_url)
        .transform(
          [](mamba::specs::CondaURL&& url)
          {
            return url.pretty_str(
              mamba::specs::CondaURL::StripScheme::no,
              0,  // don't strip any path characters
              mamba::specs::CondaURL::Credentials::Remove
            );
          }
        )
        .or_else(
          [&](const auto&) -> mamba::specs::expected_parse_t<std::string>
          { return record.package_url; }
        )
        .value();

      if (no_md5 != 1)
      {
        output << "#" << record.md5;
      }
      output << "\n";
      output.flush();
    }
  } else if (json_format)
  {
    auto pd = mamba::PrefixData::create(ctx.prefix_params.target_prefix, channel_context)
      .value();
    mamba::History& hist = pd.history();

    const auto& versions_map = pd.records();
    const auto& pip_versions_map = pd.pip_records();
    auto requested_specs_map = hist.get_requested_specs_map();
    std::stringstream dependencies;
    std::set<std::string> channels;

    bool first_dependency_printed = false;
    for (const auto& [k, v] : versions_map)
    {
      if (from_history && requested_specs_map.find(k) == requested_specs_map.end())
      {
        continue;
      }

      dependencies << (first_dependency_printed ? ",\n" : "") << "    \"";
      first_dependency_printed = true;

      auto chans = channel_context.make_channel(v.channel);

      if (from_history)
      {
        dependencies << requested_specs_map[k].str() << "\"";
      } else
      {
        if (channel_subdir)
        {
          dependencies
            // If the size is not one, it's a custom multi channel
            << ((chans.size() == 1) ? chans.front().display_name() : v.channel)
            << "/" << v.platform << "::";
        }
        dependencies << v.name << "=" << v.version;
        if (!no_build)
        {
          dependencies << "=" << v.build_string;
        }
        if (no_md5 == -1)
        {
          dependencies << "[md5=" << v.md5 << "]";
        }
        dependencies << "\"";
      }

      for (const auto& chan : chans)
      {
        channels.insert(chan.display_name());
      }
    }

    // Add a `pip` subsection in `dependencies` listing wheels installed from PyPI
    if (!pip_versions_map.empty())
    {
      dependencies << (first_dependency_printed ? ",\n" : "") << "     { \"pip\": [\n";
      first_dependency_printed = false;
      for (const auto& [k, v] : pip_versions_map)
      {
        dependencies << (first_dependency_printed ? ",\n" : "") << "      \""
          << v.name << "==" << v.version << "\"";
        first_dependency_printed = true;
      }
      dependencies << "\n    ]\n    }";
    }

    dependencies << (first_dependency_printed ? "\n" : "");

    output << "{\n";

    output << "  \"channels\": [\n";
    for (auto channel_it = channels.begin(); channel_it != channels.end(); ++channel_it)
    {
      auto last_channel = std::next(channel_it) == channels.end();
      output << "    \"" << *channel_it << "\"" << (last_channel ? "" : ",") << "\n";
    }
    output << "  ],\n";

    output << "  \"dependencies\": [\n" << dependencies.str() << "  ],\n";

    output << "  \"name\": \""
      << mamba::detail::get_env_name(ctx, ctx.prefix_params.target_prefix)
      << "\",\n";
    output << "  \"prefix\": " << ctx.prefix_params.target_prefix << "\n";

    output << "}\n";

    output.flush();
  } else
  {
    auto pd = mamba::PrefixData::create(ctx.prefix_params.target_prefix, channel_context)
      .value();
    mamba::History& hist = pd.history();

    const auto& versions_map = pd.records();
    const auto& pip_versions_map = pd.pip_records();

    output << "name: "
      << mamba::detail::get_env_name(ctx, ctx.prefix_params.target_prefix)
      << "\n";
    output << "channels:\n";

    auto requested_specs_map = hist.get_requested_specs_map();
    std::stringstream dependencies;
    std::set<std::string> channels;

    for (const auto& [k, v] : versions_map)
    {
      if (from_history && requested_specs_map.find(k) == requested_specs_map.end())
      {
        continue;
      }

      auto chans = channel_context.make_channel(v.channel);

      if (from_history)
      {
        dependencies << "  - " << requested_specs_map[k].str() << "\n";
      } else
      {
        dependencies << "  - ";
        if (channel_subdir)
        {
          dependencies
            // If the size is not one, it's a custom multi channel
            << ((chans.size() == 1) ? chans.front().display_name() : v.channel)
            << "/" << v.platform << "::";
        }
        dependencies << v.name << "=" << v.version;
        if (!no_build)
        {
          dependencies << "=" << v.build_string;
        }
        if (no_md5 == -1)
        {
          dependencies << "[md5=" << v.md5 << "]";
        }
        dependencies << "\n";
      }

      for (const auto& chan : chans)
      {
        channels.insert(chan.display_name());
      }
    }
    // Add a `pip` subsection in `dependencies` listing wheels installed from PyPI
    if (!pip_versions_map.empty())
    {
      dependencies << "  - pip:\n";
      for (const auto& [k, v] : pip_versions_map)
      {
        dependencies << "    - " << v.name << "==" << v.version << "\n";
      }
    }

    for (const auto& c : channels)
    {
      output << "  - " << c << "\n";
    }
    output << "dependencies:\n" << dependencies.str() << std::endl;

    output << "prefix: " << ctx.prefix_params.target_prefix << std::endl;

    output.flush();
  }

  return output.str();
}

bool UMambaEnvRemove(mamba::Configuration& config)
{
  // Remove specs if exist
  mamba::RemoveResult remove_env_result = remove(config, mamba::MAMBA_REMOVE_ALL);

  if (remove_env_result == mamba::RemoveResult::NO)
  {
    mamba::Console::stream() << "The environment was not removed.";
    return false;
  }

  if (remove_env_result == mamba::RemoveResult::EMPTY)
  {
    mamba::Console::stream() << "No packages to remove from environment.";

    auto res = mamba::Console::instance().prompt("Do you want to remove the environment?", 'Y');
    if (!res)
    {
      mamba::Console::stream() << "The environment was not removed.";
      return false;
    }
  }

  const auto& ctx = config.context();
  if (!ctx.dry_run)
  {
    const auto& prefix = ctx.prefix_params.target_prefix;
    // Remove env directory or rename it (e.g. if used)
    mamba::remove_or_rename(ctx, mamba::util::expand_home(prefix.string()));

    mamba::EnvironmentsManager env_manager{ ctx };
    // Unregister environment
    env_manager.unregister_env(mamba::util::expand_home(prefix.string()));

    mamba::Console::instance().print(mamba::util::join(
      "",
      std::vector<std::string>({ "Environment removed at prefix: ", prefix.string() })
    ));
    mamba::Console::instance().json_write({ { "success", true } });
    return true;
  } else
  {
    mamba::Console::stream() << "Dry run. The environment was not removed.";
    return false;
  }
}