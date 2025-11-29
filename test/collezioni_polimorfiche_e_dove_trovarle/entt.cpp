#include <gmock/gmock.h>

#include <iostream>

#include <entt/entt.hpp>

#include "const.hpp"

struct drawable_t
{};
struct level_t
{
  int level;
};
struct on_off_t
{
  bool is_on;
};

void
draw(entt::registry& registry, std::ostream& os)
{
  os << "<document>\n";
  registry.view<drawable_t, on_off_t>(entt::exclude<level_t>)
    .each([&os](auto, auto& on_off) {
      os << "<on_off_light>\n";
      os << "<is_on>\n";
      os << std::boolalpha << on_off.is_on;
      os << "\n";
      os << "</is_on>\n";
      os << "</on_off_light>\n";
    });
  registry.view<on_off_t, level_t>().each(
    [&os](auto, auto& on_off, auto& level) {
      os << "<dimmable_light>\n";
      os << "<is_on>\n";
      os << std::boolalpha << on_off.is_on;
      os << "\n";
      os << "</is_on>\n";
      os << "<level>\n";
      os << level.level;
      os << "\n";
      os << "</level>\n";
      os << "</dimmable_light>\n";
    });
  os << "</document>\n";
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
