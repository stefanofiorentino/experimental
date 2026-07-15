#pragma once

#include "include/document.hpp"
#include "light_bulb.hpp"
#include <iostream>

struct dimmable_light_t final // cppcheck-suppress ctuOneDefinitionRuleViolation
{
  void draw(std::ostream& oss, size_t /*position*/) const
  {
    oss << "<dimmable_light>\n";
    oss << "<is_on>\n";
    oss << std::boolalpha << is_on;
    oss << "\n";
    oss << "</is_on>\n";
    oss << "<level>\n";
    oss << level;
    oss << "\n";
    oss << "</level>\n";
    oss << "</dimmable_light>\n";
  }

  void do_switch(bool is_on) { this->is_on = is_on; }

  void do_dimm(int level) { this->level = level; }

private:
  bool is_on{ true };
  int level{ 42 };
};

template<>
inline void
draw(const dimmable_light_t& light, std::ostream& out, size_t position)
{
  light.draw(out, position);
}
