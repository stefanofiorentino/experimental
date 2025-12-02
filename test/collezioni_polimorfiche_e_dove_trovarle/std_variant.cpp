#include <gmock/gmock.h>

#include <algorithm>
#include <iostream>
#include <variant>
#include <vector>

#include <light_management/dimmable_light_bulb.hpp>
#include <light_management/include/type_traits.hpp>
#include <light_management/light_bulb.hpp>

#include "const.hpp"

using light_concept_t = std::variant<on_off_light_t, dimmable_light_t>;
using variant_collection_t = std::vector<light_concept_t>;

void
draw(variant_collection_t& collection, std::ostream& os)
{
  os << "<?xml version=\"1.0\"?>\n";
  os << "<document>\n";
  std::for_each(
    collection.begin(), collection.end(), [&os](auto const& light_) {
      std::visit(
        [&os](auto const& light) {
          if constexpr (has_void_draw_v<decltype(light)>) {
            light.draw(os, 2);
          }
        },
        light_);
    });
  os << "</document>\n";
}

void
do_switch(variant_collection_t& collection, bool status)
{
  std::for_each(collection.begin(), collection.end(), [status](auto& light_) {
    std::visit(
      [status](auto& light) {
        if constexpr (has_void_do_switch_v<decltype(light)>) {
          light.do_switch(status);
        }
      },
      light_);
  });
}

TEST(std_variant, do_switch)
{
  variant_collection_t c;
  c.emplace_back(on_off_light_t());
  c.emplace_back(dimmable_light_t());

  std::ostringstream oss;
  do_switch(c, true);
  draw(c, oss);
  ASSERT_EQ(EXPECTED_STRING_FULL, oss.str());
}
