#pragma once

#include "include/document.hpp"
#include <iostream>

struct light_bulb_t final
{
  light_bulb_t()
    : m_status{ false }
  {
  }
  void draw(std::ostream& oss, size_t position) const
  {
    oss << std::string(position, ' ') << "<light_bulb_t>" << std::boolalpha
        << m_status << "</light_bulb_t>\n";
  }
  void do_switch(bool status) { m_status = status; }

private:
  bool m_status;
};

template<>
inline void
draw(const light_bulb_t& light, std::ostream& out, size_t position)
{
  light.draw(out, position);
}

template<>
inline void
do_switch(light_bulb_t& light, bool status)
{
  light.do_switch(status);
}
