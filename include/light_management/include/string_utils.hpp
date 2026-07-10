#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>

inline std::string trim(const std::string& str)
{
  auto not_space = [](unsigned char ch) { return !std::isspace(ch); };

  auto begin = std::find_if(str.begin(), str.end(), not_space);
  auto end = std::find_if(str.rbegin(), str.rend(), not_space).base();

  if (begin >= end)
    return {};
  return std::string{ begin, static_cast<std::size_t>(end - begin) };
}

template<typename U>
inline bool
iequals(const std::string& a, U&& b)
{
  if (a.size() != b.size())
    return false;
  return std::equal(
    a.begin(), a.end(), b.begin(), [](unsigned char x, unsigned char y) {
      return std::tolower(x) == std::tolower(y);
    });
}

inline bool
parse_int(const std::string& s, size_t& value)
{
  auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
  return ec == std::errc{} && ptr == s.data() + s.size();
}

inline bool
parse_int(const std::string& s, int& value)
{
  auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
  return ec == std::errc{} && ptr == s.data() + s.size();
}