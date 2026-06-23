#pragma once

#include <string>
#include <map>
#include <stdexcept>
#include <cctype>
#include <set>
#include <optional>
#include <vector>
#include <tuple>

#include "k.h"


// Supported options and defaults
using IntOptionDefaults = std::map<std::string, std::pair<std::optional<int64_t>, std::string>>;
using StringOptionDefaults = std::map<std::string, std::pair<std::optional<std::string>, std::string>>;
using StringListOptionDefaults = std::map<std::string, std::pair<std::vector<std::string>, std::string>>;
using AllOptionDefaults = std::tuple<IntOptionDefaults, StringOptionDefaults, StringListOptionDefaults>;


// Helper class for reading dictionary of options
//
// Dictionary key:    KS
// Dictionary value:  KS or
//                    KJ or
//                    0 of -KS|-KJ|KC
class KdbOptions
{
private:
  IntOptionDefaults int_options;
  StringOptionDefaults string_options;
  StringListOptionDefaults stringlist_options;

private:
  const std::string ToUpper(const std::string& str) const
  {
    std::string upper;
    for (const auto i : str)
      upper.push_back((unsigned char)std::toupper(i));
    return upper;
  }

  void ToUpper(std::string& str)
  {
    for (size_t i = 0; i < str.length(); ++i)
      str[i] = (unsigned char)std::toupper(str[i]);
  }

  template <typename T>
  void PopulateIntOptions(K keys, K values)
  {
    for (J i = 0; i < values->n; ++i) {
      const std::string key = kS(keys)[i];
      if (int_options.find(key) == int_options.end())
        throw InvalidOption(("Unsupported int option '" + key + "'").c_str());
      int_options[key].first = ((T*)kG(values))[i];
    }
  }

  void PopulateStringOptions(K keys, K values)
  {
    for (J i = 0; i < values->n; ++i) {
      const std::string key = kS(keys)[i];
      if (string_options.find(key) == string_options.end())
        throw InvalidOption(("Unsupported string option '" + key + "'" + " (should it be a stringlist?)").c_str());
      string_options[key].first = kS(values)[i];
    }
  }

  void PopulateMixedOptions(K keys, K values)
  {
    for (J i = 0; i < values->n; ++i) {
      const std::string key = kS(keys)[i];
      K value = kK(values)[i];
      switch (value->t) {
      case -KB:
      case -KG:
      case -KH:
      case -KI:
      case -KJ:
        if (int_options.find(key) == int_options.end())
          throw InvalidOption(("Unsupported int option '" + key + "'").c_str());
        if (value->t == -KB || value->t == -KG)
          int_options[key].first = value->g;
        else if (value->t == -KH)
          int_options[key].first = value->h;
        else if (value->t == -KI)
          int_options[key].first = value->i;
        else
          int_options[key].first = value->j;
        break;
      case -KS:
      case KC:
        if (string_options.find(key) == string_options.end())
          throw InvalidOption(("Unsupported string option '" + key + "'" + " (should it be a stringlist?)").c_str());
        if (value->t == -KS)
          string_options[key].first = value->s;
        else
          string_options[key].first = std::string((char*)kG(value), value->n);
        break;
      case KS:
      case 0:
        if (stringlist_options.find(key) == stringlist_options.end())
          throw InvalidOption(("Unsupported stringlist option '" + key + "'").c_str());
        if (value->t == KS) {
          for (J j = 0; j < value->n; ++j)
            stringlist_options[key].first.push_back(kS(value)[j]);
        } else {
          for (J j = 0; j < value->n; ++j)
            stringlist_options[key].first.push_back(GetKdbString(kK(value)[j]));
        }
        break;
      case 101:
        // Ignore ::
        break;
      default:
        throw InvalidOption(("option '" + key + "' value not -1|-4|-5|-6|-7|-11|0|10|11h").c_str());
      }
    }
  }

  void CheckMissingOptions()
  {
    for (const auto& i : int_options) {
      if (!i.second.first.has_value())
        throw InvalidOption(("Missing required int option '" + i.first + "'").c_str());
    }
    for (const auto& i : string_options) {
      if (!i.second.first.has_value())
        throw InvalidOption(("Missing required string option '" + i.first + "'").c_str());
    }
    for (const auto& i : stringlist_options) {
      if (i.second.first.size() == 0)
        throw InvalidOption(("Missing required stringlist option '" + i.first + "'").c_str());
    }
  }

  K Help() const
  {
    auto num = int_options.size() + string_options.size() + stringlist_options.size();

    K key_col = ktn(KS, 1);
    kS(key_col)[0] = ss((S)"option");

    K value_col = ktn(KS, 3);
    kS(value_col)[0] = ss((S)"type");
    kS(value_col)[1] = ss((S)"default");
    kS(value_col)[2] = ss((S)"description");

    K keys = ktn(KS, num);
    K types = ktn(KS, num);
    K defaults = ktn(0, num);
    K descriptions = ktn(0, num);

    K key_tab = xT(xD(key_col, knk(1, keys)));
    K value_tab = xT(xD(value_col, knk(3, types, defaults, descriptions)));

    size_t index = 0;
    for (const auto& i : int_options) {
      kS(keys)[index] = ss((S)i.first.c_str());
      kS(types)[index] = ss((S)"int");
      if (!i.second.first.has_value())
        kK(defaults)[index] = Identity();
      else
        kK(defaults)[index] = kj(i.second.first.value());
      kK(descriptions)[index] = kp((S)i.second.second.c_str());
      ++index;
    }

    for (const auto& i : string_options) {
      kS(keys)[index] = ss((S)i.first.c_str());
      kS(types)[index] = ss((S)"string");
      if (!i.second.first.has_value())
        kK(defaults)[index] = Identity();
      else
        kK(defaults)[index] = kp((S)i.second.first.value().c_str());
      kK(descriptions)[index] = kp((S)i.second.second.c_str());
      ++index;
    }

    for (const auto& i : stringlist_options) {
      kS(keys)[index] = ss((S)i.first.c_str());
      kS(types)[index] = ss((S)"stringlist");
      if (i.second.first.size() == 0) {
        kK(defaults)[index] = Identity();
      } else {
        auto len = i.second.first.size();
        K str_list = ktn(0, len);
        J str_index = 0;
        for (const auto& j : i.second.first) {
          kK(str_list)[str_index++] = kp((S)j.c_str());
        }
        kK(defaults)[index] = str_list;
      }
      kK(descriptions)[index] = kp((S)i.second.second.c_str());
      ++index;
    }

    return k(0, (S)"`option xasc", xD(key_tab, value_tab), (K)0);
  }

public:
  class InvalidOption : public std::invalid_argument
  {
  public:
    InvalidOption(const std::string message) : std::invalid_argument(message.c_str())
    {};
  };

  KdbOptions(K options, const std::vector<AllOptionDefaults>& option_defaults, K* help)
  {
    // Merge all the options into a single set
    for (const auto& i : option_defaults) {
      int_options.insert(std::get<0>(i).cbegin(), std::get<0>(i).cend());
      string_options.insert(std::get<1>(i).cbegin(), std::get<1>(i).cend());
      stringlist_options.insert(std::get<2>(i).cbegin(), std::get<2>(i).cend());
    }

    if (help && options->t == -KS && std::strcmp("help", options->s) == 0)
      *help = Help();
    else {
      if (options->t != 101) {
        if (options->t != 99)
          throw InvalidOption("options not -99h");
        K keys = kK(options)[0];
        if (keys->t != KS)
          throw InvalidOption("options keys not 11h");
        K values = kK(options)[1];

        switch (values->t) {
        case KB:
        case KG:
          PopulateIntOptions<G>(keys, values);
          break;
        case KH:
          PopulateIntOptions<H>(keys, values);
          break;
        case KI:
          PopulateIntOptions<I>(keys, values);
          break;
        case KJ:
          PopulateIntOptions<J>(keys, values);
          break;
        case KS:
          PopulateStringOptions(keys, values);
          break;
        case 0:
          PopulateMixedOptions(keys, values);
          break;
        default:
          throw InvalidOption("options values not 1|4|5|6|7|11|0h");
        }
      }

      //CheckMissingOptions();
    }
  }

  template <typename T>
  T GetIntOption(const std::string& key) const
  {
    const auto it = int_options.find(key);
    if (it == int_options.end())
      throw InvalidOption(("Unknown int option '" + key + "'").c_str());
    else {
      return (T)it->second.first.value();
    }
  }

  std::optional<std::string> GetStringOption(const std::string& key) const
  {
    const auto it = string_options.find(key);
    if (it == string_options.end())
      throw InvalidOption(("Unknown string option '" + key + "'").c_str());
    else {
      return it->second.first;
    }
  }

  std::vector<std::string> GetStringListOption(const std::string& key) const
  {
    const auto it = stringlist_options.find(key);
    if (it == stringlist_options.end())
      throw InvalidOption(("Unknown stringlist option '" + key + "'").c_str());
    else {
      return it->second.first;
    }
  }

  template <typename T>
  void SetIntOption(const std::string& key, mamba::Configurable& conf) const
  {
    const auto it = int_options.find(key);
    if (it == int_options.end())
      throw InvalidOption(("Unknown int option '" + key + "'").c_str());
    else if (it->second.first.has_value())
      conf.set_value((T)it->second.first.value());
  }

  template <typename T>
  void SetStringOption(const std::string& key, mamba::Configurable& conf) const
  {
    const auto it = string_options.find(key);
    if (it == string_options.end())
      throw InvalidOption(("Unknown string option '" + key + "'").c_str());
    else if (it->second.first.has_value())
      conf.set_value(T(it->second.first.value()));
  }

  void SetStringListOption(const std::string& key, mamba::Configurable& conf) const
  {
    const auto it = stringlist_options.find(key);
    if (it == stringlist_options.end())
      throw InvalidOption(("Unknown string list option '" + key + "'").c_str());
    else if (it->second.first.size() != 0) {
      conf.set_value(it->second.first);
    }
  }
};

