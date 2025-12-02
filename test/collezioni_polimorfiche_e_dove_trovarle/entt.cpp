#include <gmock/gmock.h>

#include <iostream>

#include <entt/entt.hpp>

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
  registry.view<on_off_t, level_t>().each(
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
