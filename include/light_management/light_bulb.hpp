#pragma once

#include "include/document.hpp"
#include <iostream>

struct on_off_light_t final // cppcheck-suppress ctuOneDefinitionRuleViolation
{
  on_off_light_t()
    : m_status{ true }
  {
  }
  void draw(std::ostream& oss, size_t /*position*/) const
  {
    oss << "<on_off_light>\n<is_on>\n"
        << std::boolalpha << m_status << "\n</is_on>\n</on_off_light>\n";
  }
  void do_switch(bool status) { m_status = status; }

private:
  bool m_status;
};

template<>
inline void
draw(const on_off_light_t& light, std::ostream& out, size_t position)
{
  light.draw(out, position);
}

template<>
inline void
do_switch(on_off_light_t& light, bool status)
{
  light.do_switch(status);
}
