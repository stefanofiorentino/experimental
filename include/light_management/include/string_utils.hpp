#pragma once

#include <algorithm>
#include <cctype>
#include <charconv>
#include <string>

template<typename U>
inline bool
iequals(std::string const& a, U&& b)
{
  if (a.size() != b.size())
    return false;
  return std::equal(
    a.begin(), a.end(), b.begin(), [](unsigned char x, unsigned char y) {
      return std::tolower(x) == std::tolower(y);
    });
}

inline bool
parse_int(const std::string& s, int& value)
{
  auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
  return ec == std::errc{} && ptr == s.data() + s.size();
}

inline std::string_view
trim(std::string_view sv)
{
  auto not_space = [](unsigned char ch) { return !std::isspace(ch); };

  auto begin = std::find_if(sv.begin(), sv.end(), not_space);
  auto end = std::find_if(sv.rbegin(), sv.rend(), not_space).base();

  if (begin >= end)
    return {}; // all whitespace
  return std::string_view{ begin, static_cast<std::size_t>(end - begin) };
}
