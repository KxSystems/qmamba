#pragma once

std::string UMambaEnvExport(mamba::Configuration& config,
  bool explicit_format, int no_md5, bool no_build, bool channel_subdir, bool from_history);
bool UMambaEnvRemove(mamba::Configuration& config);
