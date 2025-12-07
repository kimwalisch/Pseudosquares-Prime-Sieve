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

#include "int128_t.hpp"

#include <string>
#include <vector>

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

CmdOptions parseOptions(int, char**);

#endif
