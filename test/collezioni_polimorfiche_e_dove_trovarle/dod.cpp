#include <cstdint>
#include <gmock/gmock.h>

#include <ios>
#include <iostream>
#include <optional>

// https://youtu.be/SzjJfKHygaQ?si=8fP6lFRqNu1iJHNn&t=2533
struct on_off_t
{
  bool is_on;
};

struct level_t
{
  size_t level;
};

struct on_off_light_t
{
  size_t on_off_idx;
};

struct dimmable_light_t
{
  size_t on_off_idx;
  size_t level_idx;
};

struct world_t
{
  std::vector<std::optional<on_off_t>> on_offs;
  std::vector<std::optional<level_t>> levels;
  std::vector<on_off_light_t> on_off_lights;
  std::vector<dimmable_light_t> dimmable_lights;

  size_t add_on_off(const on_off_t& on_off)
  {
    for (auto i = 0u; i < on_offs.size(); ++i)
      if (!on_offs[i].has_value()) {
        on_offs[i].emplace(on_off);
        return i;
      }
    on_offs.emplace_back(on_off);
    return on_offs.size() - 1;
  }
  size_t add_level(const level_t& level)
  {
    for (auto i = 0u; i < levels.size(); ++i)
      if (!levels[i].has_value()) {
        levels[i].emplace(level);
        return i;
      }
    levels.emplace_back(level);
    return levels.size() - 1;
  }
  size_t add_on_off_light(const on_off_t& on_off)
  {
    on_off_light_t on_off_light;
    on_off_light.on_off_idx = add_on_off(on_off);
    on_off_lights.emplace_back(on_off_light);
    return on_off_lights.size() - 1;
  }
  size_t add_dimmable_light(const on_off_t& on_off, const level_t& level)
  {
    dimmable_light_t dimmable_light;
    dimmable_light.on_off_idx = add_on_off(on_off);
    dimmable_light.level_idx = add_level(level);
    dimmable_lights.emplace_back(dimmable_light);
    return dimmable_lights.size() - 1;
  }
  void draw(std::ostream& oss) const
  {
    oss << "<document>\n";
    for (const auto& on_off_light : on_off_lights)
      oss << "<on_off_light>\n<is_on>\n"
          << std::boolalpha << on_offs[on_off_light.on_off_idx]->is_on
          << "\n</is_on>\n</on_off_light>\n";
    for (const auto& dimmable_light : dimmable_lights)
      oss << "<dimmable_light>\n<is_on>\n"
          << std::boolalpha << on_offs[dimmable_light.on_off_idx]->is_on
          << "\n</is_on>\n<level>\n"
          << std::to_string(levels[dimmable_light.level_idx]->level)
          << "\n</level>\n</dimmable_light>\n";
    oss << "</document>\n";
  }
};

TEST(dod, simple_draw)
{
  world_t world;
  auto on_off = on_off_t{ true };
  auto idx = world.add_on_off_light(on_off);
  ASSERT_EQ(0, idx);

  std::ostringstream oss;
  world.draw(oss);
  ASSERT_EQ(R"(<document>
<on_off_light>
<is_on>
true
</is_on>
</on_off_light>
</document>
)",
            oss.str());
}

TEST(dod, draw)
{
  world_t world;
  auto on_off = on_off_t{ true };
  auto level = level_t{ 42 };
  auto idx = world.add_on_off_light(on_off);
  ASSERT_EQ(0, idx);
  idx = world.add_dimmable_light(on_off, level);
  ASSERT_EQ(0, idx);

  std::ostringstream oss;
  world.draw(oss);
  ASSERT_EQ(R"(<document>
<on_off_light>
<is_on>
true
</is_on>
</on_off_light>
<dimmable_light>
<is_on>
true
</is_on>
<level>
42
</level>
</dimmable_light>
</document>
)",
            oss.str());
}
