#include <gmock/gmock.h>

#include <map>
#include <scoped_allocator>
#include <string>
#include <utility>

#include "visitor_guide_allocators/constants.h"
#include "visitor_guide_allocators/statically_allocated_arena.hpp"
#include "visitor_guide_allocators/statically_allocated_arena_allocator.h"

using Arena1024 = Arena<1024>;
using ArenaAllocator1024 = ArenaAllocator<std::byte, Arena1024>;

template<typename T>
using SA1024 = std::scoped_allocator_adaptor<ArenaAllocator<T, Arena1024>>;

using astring = std::basic_string<char, std::char_traits<char>, SA1024<char>>;
template<typename K, typename V>
using amap = std::map<K, V, std::less<>, SA1024<std::pair<const K, V>>>;

astring
make_astring(std::string const& rhs, ArenaAllocator1024& arenaAllocator)
{
  return astring(rhs, arenaAllocator);
}

TEST(statically_allocated_string, whenStdStringIsUsedThenItWorks)
{
  Arena1024 arena;
  ArenaAllocator1024 arenaAllocator(&arena);
  auto str =
    make_astring(experimental::constants::VERY_LONG_STRING, arenaAllocator);
  ASSERT_EQ(experimental::constants::VERY_LONG_STRING, str.c_str());
}

TEST(statically_allocated_string, whenStdMapIsUsedTHenItWorks)
{
  Arena1024 arena;
  ArenaAllocator1024 arenaAllocator(&arena);
  amap<int, astring> m(arenaAllocator);
  m[42] = experimental::constants::VERY_SHORT_STRING;
  ASSERT_EQ(experimental::constants::VERY_SHORT_STRING, m.at(42).c_str());
}