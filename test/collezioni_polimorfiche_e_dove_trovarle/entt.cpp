#include <charconv>
#include <gmock/gmock.h>

#include <iostream>

#include <entt/entt.hpp>
#include <pugixml.hpp>

#include <light_management/include/string_utils.hpp>

#include "const.hpp"

struct drawable_t // cppcheck-suppress ctuOneDefinitionRuleViolation
{};
struct level_t // cppcheck-suppress ctuOneDefinitionRuleViolation
{
  int level;
};
struct on_off_t // cppcheck-suppress ctuOneDefinitionRuleViolation
{
  bool is_on;
};

void
draw(entt::registry& registry, std::ostream& oss)
{
  oss << "<?xml version=\"1.0\"?>\n";
  oss << "<document>\n";
  registry.view<drawable_t, on_off_t>(entt::exclude<level_t>)
    .each([&oss](auto, auto& on_off) {
      oss << "<on_off_light>\n";
      oss << "<is_on>\n";
      oss << std::boolalpha << on_off.is_on;
      oss << "\n";
      oss << "</is_on>\n";
      oss << "</on_off_light>\n";
    });
  registry.view<drawable_t, on_off_t, level_t>().each(
    [&oss](auto, auto& on_off, auto& level) {
      oss << "<dimmable_light>\n";
      oss << "<is_on>\n";
      oss << std::boolalpha << on_off.is_on;
      oss << "\n";
      oss << "</is_on>\n";
      oss << "<level>\n";
      oss << level.level;
      oss << "\n";
      oss << "</level>\n";
      oss << "</dimmable_light>\n";
    });
  oss << "</document>\n";
}

TEST(entt, draw)
{
  entt::registry registry;

  const auto light_bulb = registry.create();
  registry.emplace<drawable_t>(light_bulb);
  registry.emplace<on_off_t>(light_bulb, true);

  const auto dimmable_light_bulb = registry.create();
  registry.emplace<drawable_t>(dimmable_light_bulb);
  registry.emplace<on_off_t>(dimmable_light_bulb, true);
  registry.emplace<level_t>(dimmable_light_bulb, 42);

  std::ostringstream oss;
  draw(registry, oss);
  ASSERT_EQ(EXPECTED_STRING_FULL, oss.str());
}

TEST(entt, parsing)
{
  entt::registry registry;

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_string(EXPECTED_STRING_FULL);
  ASSERT_TRUE(result);

  pugi::xml_node root = doc.child("document");
  for (pugi::xml_node child : root.children()) {
    if (child.name() == std::string("on_off_light")) {
      const auto light_bulb = registry.create();
      registry.emplace<drawable_t>(light_bulb);
      for (pugi::xml_node on_off_light : child.children()) {
        if (on_off_light.name() == std::string("is_on")) {
          if (iequals("false", trim(on_off_light.child_value()))) {
            registry.emplace<on_off_t>(light_bulb, false);
          } else {
            registry.emplace<on_off_t>(light_bulb, true);
          }
        }
      }
    }
  }
  ASSERT_EQ(1, registry.view<drawable_t>()->size());

  for (pugi::xml_node child : root.children()) {
    if (child.name() == std::string("dimmable_light")) {
      const auto dimmable_light_bulb = registry.create();
      registry.emplace<drawable_t>(dimmable_light_bulb);
      for (pugi::xml_node dimmable_light : child.children()) {
        if (dimmable_light.name() == std::string("level")) {
          int level = 42;
          if (parse_int(dimmable_light.child_value(), level)) {
            continue;
          }
          registry.emplace<level_t>(dimmable_light_bulb, level);
        }
        if (dimmable_light.name() == std::string("is_on")) {
          if (iequals("false", trim(dimmable_light.child_value()))) {
            registry.emplace<on_off_t>(dimmable_light_bulb, false);
          } else {
            registry.emplace<on_off_t>(dimmable_light_bulb, true);
          }
        }
      }
    }
  }
  ASSERT_EQ(2, registry.view<drawable_t>()->size());
  ASSERT_EQ(2, registry.view<on_off_t>()->size());
  ASSERT_EQ(1, registry.view<level_t>()->size());

  std::ostringstream oss;
  draw(registry, oss);
  ASSERT_EQ(EXPECTED_STRING_FULL, oss.str());
}
