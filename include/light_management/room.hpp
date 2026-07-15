#pragma once

#include <iostream>
#include <vector>

#include "light_bulb.hpp"

struct room_t final
{
  template<typename T>
  void emplace_back(T&& t)
  {
    children_.emplace_back(std::forward<T>(t));
  }

  void draw(std::ostream& oss, size_t position) const
  {
    if (children_.empty()) {
      oss << std::string(position, ' ') << "<room_t/>\n";
      return;
    }
    oss << std::string(position, ' ') << "<room_t>\n";
    ::draw(children_, oss, position);
    oss << std::string(position, ' ') << "</room_t>\n";
  }

  collection_t children_;
};

template<>
inline void
draw(const room_t& room, std::ostream& out, size_t position)
{
  room.draw(out, position);
}
