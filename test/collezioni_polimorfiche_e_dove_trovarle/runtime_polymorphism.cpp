#include <gmock/gmock.h>

#include <light_management/include/document.hpp> // https://www.youtube.com/watch?v=QGcVXgEVMJg
#include <pugixml.hpp>

#include <light_management/dimmable_light_bulb.hpp>
#include <light_management/include/string_utils.hpp>
#include <light_management/include/type_traits.hpp>
#include <light_management/light_bulb.hpp>

#include "const.hpp"

TEST(runtime_polymorphism, draw_plain_plant)
{
  collection_t c;
  c.emplace_back(on_off_light_t());
  c.emplace_back(dimmable_light_t());

  std::ostringstream oss;
  draw(c, oss, 0);
  ASSERT_EQ(EXPECTED_STRING_FULL, oss.str());
}

TEST(runtime_polymorphism, parsing)
{
  collection_t c;

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_string(EXPECTED_STRING_FULL);
  ASSERT_TRUE(result);

  pugi::xml_node root = doc.child("document");
  for (pugi::xml_node child : root.children()) {
    if (child.name() == std::string("on_off_light")) {
      auto on_off = on_off_light_t();
      for (pugi::xml_node on_off_light : child.children()) {
        if (on_off_light.name() == std::string("is_on")) {
          if (iequals("false", trim(on_off_light.child_value()))) {
            on_off.do_switch(false);
          }
        }
      }
      c.emplace_back(on_off);
    }
  }
  ASSERT_EQ(1, c.size());

  for (pugi::xml_node child : root.children()) {
    if (child.name() == std::string("dimmable_light")) {
      auto dimmable = dimmable_light_t();
      for (pugi::xml_node dimmable_light : child.children()) {
        if (dimmable_light.name() == std::string("level")) {
          int level = 42;
          if (parse_int(dimmable_light.child_value(), level)) {
            continue;
          }
          dimmable.do_dimm(level);
        }
        if (dimmable_light.name() == std::string("is_on")) {
          if (iequals("false", trim(dimmable_light.child_value()))) {
            dimmable.do_switch(false);
          }
        }
      }
      c.emplace_back(dimmable);
    }
  }
  ASSERT_EQ(2, c.size());

  std::ostringstream oss;
  draw(c, oss, 0);
  ASSERT_EQ(EXPECTED_STRING_FULL, oss.str());
}
