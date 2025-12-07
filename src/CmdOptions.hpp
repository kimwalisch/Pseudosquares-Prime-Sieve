///
/// @file   CmdOptions.cpp
/// @brief  Command-line option handling.
///
/// Copyright (C) 2025 Kim Walisch, <kim.walisch@gmail.com>
///
/// This file is distributed under the BSD License. See the COPYING
/// file in the top level directory.
///

#ifndef CMDOPTIONS_HPP
#define CMDOPTIONS_HPP

#include "pseudosquares_prime_sieve.hpp"
#include "calculator.hpp"

#include <cstddef>
#include <cctype>
#include <map>
#include <stdint.h>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

void help(int exit_code);
void version();

namespace {

enum OptionID
{
  OPTION_DISTANCE,
  OPTION_HELP,
  OPTION_NUMBER,
  OPTION_PRINT,
  OPTION_THREADS,
  OPTION_VERSION
};

/// Command-line option
struct Option
{
  // Example:
  // str = "--threads=32"
  // opt = "--threads"
  // val = "32"
  std::string str;
  std::string opt;
  std::string val;

  template <typename T>
  T getValue() const
  {
    try {
      return calculator::eval<T>(val);
    }
    catch (std::exception& e) {
      throw std::runtime_error("invalid option '" + opt + "=" + val + "'\n" + e.what());
    }
  }
};

struct CmdOptions
{
  std::vector<uint128_t> numbers;
  std::vector<std::string> numbers_str;
  std::string optionStr;
  int option = -1;
  int threads = 0;
  bool print_primes = false;
  void optionDistance(Option& opt);
};

/// Some command-line options require an additional parameter.
/// Examples: --threads THREADS, -a ALPHA, ...
enum IsParam
{
  NO_PARAM,
  REQUIRED_PARAM,
  OPTIONAL_PARAM
};

/// Options start with "-" or "--", then
/// follows a Latin ASCII character.
///
bool isOption(const std::string& str)
{
  // Option of type: -o...
  if (str.size() >= 2 &&
      str[0] == '-' &&
      ((str[1] >= 'a' && str[1] <= 'z') ||
       (str[1] >= 'A' && str[1] <= 'Z')))
    return true;

  // Option of type: --o...
  if (str.size() >= 3 &&
      str[0] == '-' &&
      str[1] == '-' &&
      ((str[2] >= 'a' && str[2] <= 'z') ||
       (str[2] >= 'A' && str[2] <= 'Z')))
    return true;

  return false;
}

/// Parse the next command-line option.
/// e.g. "--threads=32"
/// -> opt.str = "--threads=32"
/// -> opt.opt = "--threads"
/// -> opt.val = "8"
///
template <typename T>
Option parseOption(int argc,
                   char* argv[],
                   int& i,
                   const T& optionMap)
{
  Option opt;
  opt.str = argv[i];

  if (opt.str.empty())
    throw std::runtime_error("unrecognized option ''");

  // Check if the option has the format:
  // --opt or -o (but not --opt=N)
  if (optionMap.count(opt.str))
  {
    opt.opt = opt.str;
    IsParam isParam = optionMap.at(opt.str).second;

    if (isParam == REQUIRED_PARAM)
    {
      i += 1;

      if (i < argc)
        opt.val = argv[i];

      // Prevent --threads --other-option
      if (opt.val.empty() || isOption(opt.val))
        throw std::runtime_error("missing value for option '" + opt.opt + "'");
    }

    // If the option takes an optional argument we
    // assume the next value is an optional argument
    // if the value is not a vaild option.
    if (isParam == OPTIONAL_PARAM &&
        i + 1 < argc &&
        !std::string(argv[i + 1]).empty() &&
        !isOption(argv[i + 1]))
    {
      i += 1;
      opt.val = argv[i];
    }
  }
  else
  {
    // Here the option is either:
    // 1) An option of type: --opt[=N]
    // 2) An option of type: --opt[N]
    // 3) A number (e.g. the start number)

    if (isOption(opt.str))
    {
      std::size_t pos = opt.str.find('=');

      // Option of type: --opt=N
      if (pos != std::string::npos)
      {
        opt.opt = opt.str.substr(0, pos);
        opt.val = opt.str.substr(pos + 1);

        // Print partial option: --opt (without =N)
        if (!optionMap.count(opt.opt))
          throw std::runtime_error("unrecognized option '" + opt.opt + "'");
      }
      else
      {
        // Option of type: --opt[N]
        pos = opt.str.find_first_of("0123456789");

        if (pos == std::string::npos)
          opt.opt = opt.str;
        else
        {
          opt.opt = opt.str.substr(0, pos);
          opt.val = opt.str.substr(pos);
        }

        // Print full option e.g.: --opt123
        if (!optionMap.count(opt.opt))
          throw std::runtime_error("unrecognized option '" + opt.str + "'");
      }

      // Prevent '--option='
      if (opt.val.empty() &&
          optionMap.at(opt.opt).second == REQUIRED_PARAM)
        throw std::runtime_error("missing value for option '" + opt.opt + "'");
    }
    else
    {
      // Here the option is actually a number or
      // an integer arithmetic expression.
      opt.opt = "--number";
      opt.val = opt.str;

      // This is not a valid number
      if (opt.str.find_first_of("0123456789") == std::string::npos)
        throw std::runtime_error("unrecognized option '" + opt.str + "'");

      // Prevent negative numbers as there are
      // no negative prime numbers.
      if (opt.str.at(0) == '-')
        throw std::runtime_error("unrecognized option '" + opt.str + "'");
    }
  }

  return opt;
}

void CmdOptions::optionDistance(Option& opt)
{
  uint128_t start = 0;
  uint128_t val = opt.getValue<uint128_t>();

  if (!numbers.empty())
    start = numbers.front();

  numbers.push_back(start + val);

  if (opt.val.find_first_of("0123456789") != 0)
    numbers_str.push_back(to_string(numbers.back()));
  else
  {
    if (numbers_str.empty())
      numbers_str.push_back(opt.val);
    else
      numbers_str.push_back(numbers_str.back() + "+" + opt.val);
  }
}

} // namespace

CmdOptions parseOptions(int argc, char* argv[])
{
  // No command-line options provided
  if (argc <= 1)
    help(/* exitCode */ 1);

  /// Command-line options
  const std::map<std::string, std::pair<OptionID, IsParam>> optionMap =
  {
    { "-d",                 std::make_pair(OPTION_DISTANCE, REQUIRED_PARAM) },
    { "--dist",             std::make_pair(OPTION_DISTANCE, REQUIRED_PARAM) },
    { "-h",                 std::make_pair(OPTION_HELP, NO_PARAM) },
    { "--help",             std::make_pair(OPTION_HELP, NO_PARAM) },
    { "--number",           std::make_pair(OPTION_NUMBER, REQUIRED_PARAM) },
    { "-p",                 std::make_pair(OPTION_PRINT, OPTIONAL_PARAM) },
    { "--print",            std::make_pair(OPTION_PRINT, OPTIONAL_PARAM) },
    { "-t",                 std::make_pair(OPTION_THREADS, REQUIRED_PARAM) },
    { "--threads",          std::make_pair(OPTION_THREADS, REQUIRED_PARAM) },
    { "-v",                 std::make_pair(OPTION_VERSION, NO_PARAM) },
    { "--version",          std::make_pair(OPTION_VERSION, NO_PARAM) }
  };

  CmdOptions opts;

  for (int i = 1; i < argc; i++)
  {
    Option opt = parseOption(argc, argv, i, optionMap);
    OptionID optionID = optionMap.at(opt.opt).first;

    switch (optionID)
    {
      case OPTION_DISTANCE: opts.optionDistance(opt); break;
      case OPTION_NUMBER:   opts.numbers.push_back(opt.getValue<uint128_t>());
                            opts.numbers_str.push_back(opt.val); break;
      case OPTION_PRINT:    opts.print_primes = true; break;
      case OPTION_THREADS:  opts.threads = opt.getValue<int>(); break;
      case OPTION_HELP:     help(0); break;
      case OPTION_VERSION:  version(); break;
    }
  }

  return opts;
}

#endif
