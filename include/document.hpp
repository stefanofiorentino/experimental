#pragma once

#include <iostream>

using collection_t = std::vector<int>;
void draw(collection_t const &c, std::ostream &out, size_t position) {
  std::for_each(c.cbegin(), c.cend(), [&out, position](auto&& value){
      out << std::string(position, ' ' ) << value << '\n';
  });
}
