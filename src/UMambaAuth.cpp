///////////////////////////////////////////////////////////////////////////
// Auth login and logout are not native to libmamba                      //
// These implementations are based on the micromamba code:               //
// https://github.com/mamba-org/mamba/blob/main/micromamba/src/login.cpp //
///////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#include <mamba/core/util.hpp>
#include <mamba/util/url.hpp>
#include <mamba/util/string.hpp>
#include <mamba/util/environment.hpp>
#include "mamba/util/encoding.hpp"

#include <nlohmann/json.hpp>

#include "UMambaAuth.h"


std::string
get_token_base(const std::string& host)
{
  const auto url = mamba::util::URL::parse(host).value();

  std::string maybe_colon_and_port{};
  if (!url.port().empty())
  {
    maybe_colon_and_port.push_back(':');
    maybe_colon_and_port.append(url.port());
  }
  return mamba::util::concat(
    url.host(),
    maybe_colon_and_port,
    mamba::util::rstrip(url.pretty_path(), '/')
  );
}

void UMambaAuthLogin(const std::string& host, const std::string& user, const std::string& pass, const std::string& token, const std::string& bearer)
{
  if (host.empty())
  {
    throw std::runtime_error("No host given.");
  }

  // remove any scheme etc.
  auto token_base = get_token_base(host);

  static const auto path = mamba::fs::u8path(mamba::util::user_home_dir()) / ".mamba"
    / "auth";
  mamba::fs::create_directories(path);

  nlohmann::json auth_info;
  const mamba::fs::u8path auth_file = path / "authentication.json";

  try
  {
    if (mamba::fs::exists(auth_file))
    {
      auto fi = mamba::open_ifstream(auth_file);
      fi >> auth_info;
    } else
    {
      auth_info = nlohmann::json::object();
    }
    nlohmann::json auth_object = nlohmann::json::object();

    if (pass.empty() && token.empty() && bearer.empty())
    {
      throw std::runtime_error("No password or token given.");
    }

    if (!pass.empty())
    {
      auth_object["type"] = "BasicHTTPAuthentication";

      auto pass_encoded = mamba::util::encode_base64(mamba::util::strip(pass));
      if (!pass_encoded)
      {
        throw pass_encoded.error();
      }

      auth_object["password"] = pass_encoded.value();
      auth_object["user"] = user;
    } else if (!token.empty())
    {
      auth_object["type"] = "CondaToken";
      auth_object["token"] = mamba::util::strip(token);
    } else if (!bearer.empty())
    {
      auth_object["type"] = "BearerToken";
      auth_object["token"] = mamba::util::strip(bearer);
    }

    auth_info[token_base] = auth_object;
  }
  catch (std::exception& e)
  {
    throw std::runtime_error("Could not modify " + auth_file.string() + " : " + e.what());
  }

  auto out = mamba::open_ofstream(auth_file);
  out << auth_info.dump(4);
  std::cout << "Successfully stored login information" << std::endl;
}

void UMambaAuthLogout(const std::string& host, bool all)
{
  static auto path = mamba::fs::u8path(mamba::util::user_home_dir()) / ".mamba" / "auth";
  const mamba::fs::u8path auth_file = path / "authentication.json";

  if (all)
  {
    if (mamba::fs::exists(auth_file))
    {
      mamba::fs::remove(auth_file);
    }
    return;
  }

  nlohmann::json auth_info;

  try
  {
    if (mamba::fs::exists(auth_file))
    {
      auto fi = mamba::open_ifstream(auth_file);
      fi >> auth_info;
    }

    auto token_base = get_token_base(host);
    auto it = auth_info.find(token_base);
    if (it != auth_info.end())
    {
      auth_info.erase(it);
      std::cout << "Logged out from " << token_base << std::endl;
    } else
    {
      std::cout << "You are not logged in to " << token_base << std::endl;
    }
  }
  catch (std::exception& e)
  {
    throw std::runtime_error("Could not parse " + auth_file.string() + " : " + e.what());
  }

  auto fo = mamba::open_ofstream(auth_file);
  fo << auth_info;
}
