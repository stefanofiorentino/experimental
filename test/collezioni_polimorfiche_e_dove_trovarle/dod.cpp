#include <gmock/gmock.h>

#include <cctype>
#include <cstdint>

#include <algorithm>
#include <charconv>
#include <ios>
#include <iostream>
#include <optional>
#include <string_view>

#include <pugixml.hpp>

#include "const.hpp"

// https://youtu.be/SzjJfKHygaQ?si=8fP6lFRqNu1iJHNn&t=2533
struct on_off_t
{
  bool is_on;
};

struct level_t // cppcheck-suppress ctuOneDefinitionRuleViolation
{
  size_t level;
};

struct on_off_light_t // cppcheck-suppress ctuOneDefinitionRuleViolation
{
  size_t on_off_idx;
};

struct dimmable_light_t // cppcheck-suppress ctuOneDefinitionRuleViolation
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
  ASSERT_EQ(1, world.on_offs.size());
  ASSERT_EQ(1, world.on_off_lights.size());

  std::ostringstream oss;
  world.draw(oss);
  ASSERT_EQ(EXPECTED_STRING_SHORT, oss.str());
}

TEST(dod, draw)
{
  world_t world;
  auto on_off = on_off_t{ true };
  auto level = level_t{ 42 };
  auto idx = world.add_on_off_light(on_off);
  ASSERT_EQ(0, idx);
  ASSERT_EQ(1, world.on_offs.size());
  ASSERT_EQ(1, world.on_off_lights.size());

  idx = world.add_dimmable_light(on_off, level);
  ASSERT_EQ(0, idx);
  ASSERT_EQ(2, world.on_offs.size());
  ASSERT_EQ(1, world.levels.size());
  ASSERT_EQ(1, world.dimmable_lights.size());

  std::ostringstream oss;
  world.draw(oss);
  ASSERT_EQ(EXPECTED_STRING_FULL, oss.str());
}

template<typename U>
bool
iequals(std::string const& a, U&& b)
{
  if (a.size() != b.size())
    return false;
  return std::equal(
    a.begin(), a.end(), b.begin(), [](unsigned char x, unsigned char y) {
      return std::tolower(x) == std::tolower(y);
    });
}

bool
parse_int(const std::string& s, size_t& value)
{
  auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
  return ec == std::errc{} && ptr == s.data() + s.size();
}

std::string_view
trim(std::string_view sv)
{
  auto not_space = [](unsigned char ch) { return !std::isspace(ch); };

  auto begin = std::find_if(sv.begin(), sv.end(), not_space);
  auto end = std::find_if(sv.rbegin(), sv.rend(), not_space).base();

  if (begin >= end)
    return {}; // all whitespace
  return std::string_view{ begin, static_cast<std::size_t>(end - begin) };
}

TEST(dod, parsing)
{
  world_t world;

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_string(EXPECTED_STRING_FULL);
  ASSERT_TRUE(result);

  auto on_off_idx = 0u;
  pugi::xml_node root = doc.child("document");
  for (pugi::xml_node child : root.children()) {
    if (child.name() == std::string("on_off_light")) {
      auto on_off = on_off_t{ true };
      for (pugi::xml_node on_off_light : child.children()) {
        if (on_off_light.name() == std::string("is_on")) {
          if (iequals("false", trim(on_off_light.child_value()))) {
            on_off.is_on = false;
          }
        }
      }
      on_off_idx = world.add_on_off_light(on_off);
    }
  }
  ASSERT_EQ(0u, on_off_idx);
  ASSERT_EQ(1, world.on_offs.size());
  ASSERT_EQ(1, world.on_off_lights.size());
  ASSERT_EQ(0, world.dimmable_lights.size());
  ASSERT_TRUE(world.on_offs[world.on_off_lights[on_off_idx].on_off_idx]->is_on);

  auto dimmable_idx = 0u;
  for (pugi::xml_node child : root.children()) {
    if (child.name() == std::string("dimmable_light")) {
      auto on_off = on_off_t{ true };
      auto level = level_t{ 42 };
      for (pugi::xml_node dimmable_light : child.children()) {
        if (dimmable_light.name() == std::string("level")) {
          parse_int(dimmable_light.child_value(), level.level);
        }
        if (dimmable_light.name() == std::string("is_on")) {
          if (iequals("false", trim(dimmable_light.child_value()))) {
            on_off.is_on = false;
          }
        }
      }
      dimmable_idx = world.add_dimmable_light(on_off, level);
      break;
    }
  }
  ASSERT_EQ(0, dimmable_idx);
  ASSERT_EQ(2, world.on_offs.size());
  ASSERT_EQ(1, world.levels.size());
  ASSERT_EQ(1, world.dimmable_lights.size());
  ASSERT_TRUE(
    world.on_offs[world.dimmable_lights[dimmable_idx].on_off_idx]->is_on);

  std::ostringstream oss;
  world.draw(oss);
  ASSERT_EQ(EXPECTED_STRING_FULL, oss.str());
}
