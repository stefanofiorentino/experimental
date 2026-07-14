// https://rawgit.com/google/cxx-std-draft/allocator-paper/allocator_example_usage.html

#include <gmock/gmock.h>

#include <new>
#include <string>
#include <iomanip>

#include "visitor_guide_allocators/arena.h"
#include "visitor_guide_allocators/arena_allocator.h"
#include "visitor_guide_allocators/constants.h"
#include "visitor_guide_allocators/scoped_arena_allocator.h"

using astring = std::basic_string<char, std::char_traits<char>, SA<char>>;

astring
make_astring(std::string const& rhs, ArenaAllocator<char>& arenaAllocator)
{
  return astring(rhs.cbegin(), rhs.cend(), arenaAllocator);
}

TEST(visitor_guide_allocators_astring, whenStdStringIsUsedThenItWorks)
{
  Arena arena(1024);
  ArenaAllocator<char> arenaAllocator(&arena);
  auto s2 =
    make_astring(experimental::constants::VERY_LONG_STRING, arenaAllocator);
  ASSERT_EQ(experimental::constants::VERY_LONG_STRING, s2.c_str());
}

TEST(visitor_guide_allocators_astring, whenCharPointerIsUsedThenItWorks)
{
  Arena arena(1024);
  ArenaAllocator<char> arenaAllocator(&arena);
  auto s2 = make_astring(experimental::constants::VERY_LONG_STRING_C_STR,
                         arenaAllocator);
  ASSERT_EQ(experimental::constants::VERY_LONG_STRING, s2.c_str());
}

TEST(visitor_guide_allocators_astring, whenExceedingSizeThenAnExceptionIsThrown)
{
  Arena arena(100);
  ArenaAllocator<char> arenaAllocator(&arena);
  ASSERT_THROW(
    make_astring(experimental::constants::VERY_LONG_STRING_CAUSING_EXCEPTION,
                 arenaAllocator),
    std::bad_alloc);
}

TEST(visitor_guide_allocators_astring, whenDestroyThenSpaceIsFreed)
{
  Arena arena(100);
  ArenaAllocator<char> arenaAllocator(&arena);
  auto s0 =
    make_astring(experimental::constants::VERY_LONG_STRING, arenaAllocator);
  ASSERT_EQ(0, memcmp(experimental::constants::VERY_LONG_STRING.c_str(), s0.c_str(), s0.length()));

  auto s1 =
    make_astring(experimental::constants::VERY_LONG_STRING_1, arenaAllocator);
  ASSERT_EQ(0,
            memcmp(experimental::constants::VERY_LONG_STRING_1.c_str(),
                   s1.c_str(),
                   s1.length()));

  ASSERT_THROW(
    make_astring(experimental::constants::VERY_LONG_STRING_2,
                 arenaAllocator),
    std::bad_alloc);
}
