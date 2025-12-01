// https://rawgit.com/google/cxx-std-draft/allocator-paper/allocator_example_usage.html

#include <gmock/gmock.h>

#include <string>
#include <utility>

#include "visitor_guide_allocators/arena.h"
#include "visitor_guide_allocators/arena_allocator.h"
#include "visitor_guide_allocators/constants.h"
#include "visitor_guide_allocators/scoped_arena_allocator.h"

using astring = std::basic_string<char, std::char_traits<char>, SA<char>>;

astring
make_astring(std::string const& rhs, ArenaAllocator<std::byte>& arenaAllocator)
{
  return astring(rhs, arenaAllocator);
}

TEST(visitor_guide_allocators_astring, whenStdStringIsUsedThenItWorks)
{
  Arena arena(1024);
  ArenaAllocator<std::byte> arenaAllocator(&arena);
  auto s2 =
    make_astring(experimental::constants::VERY_LONG_STRING, arenaAllocator);
  ASSERT_EQ(experimental::constants::VERY_LONG_STRING, s2.c_str());
}

TEST(visitor_guide_allocators_astring, whenCharPointerIsUsedThenItWorks)
{
  Arena arena(1024);
  ArenaAllocator<std::byte> arenaAllocator(&arena);
  auto s2 = make_astring(experimental::constants::VERY_LONG_STRING_C_STR,
                         arenaAllocator);
  ASSERT_EQ(experimental::constants::VERY_LONG_STRING, s2.c_str());
}

TEST(visitor_guide_allocators_astring, whenExceedingSizeThenAnExceptionIsThrown)
{
  Arena arena(100);
  ArenaAllocator<std::byte> arenaAllocator(&arena);
  ASSERT_THROW(
    make_astring(experimental::constants::VERY_LONG_STRING_CAUSING_EXCEPTION,
                 arenaAllocator),
    std::bad_alloc);
}

TEST(visitor_guide_allocators_astring, whenDestroyThenSpaceIsFreed)
{
  Arena arena(100);
  ArenaAllocator<std::byte> arenaAllocator(&arena);
  auto s0 =
    make_astring(experimental::constants::VERY_LONG_STRING, arenaAllocator);
  ASSERT_EQ(0,
            memcmp("Very looooooooooooooong string\0"
                   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                   "\0\0\0\0",
                   s0.c_str(),
                   100));
  auto s1 =
    make_astring(experimental::constants::VERY_LONG_STRING_1, arenaAllocator);
  ASSERT_EQ(0,
            memcmp("Very looooooooooooooong string\0"
                   "Very looooooooooooooong string 1\0"
                   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                   "\0\0",
                   s0.c_str(),
                   100));
  auto s2 =
    make_astring(experimental::constants::VERY_LONG_STRING_2, arenaAllocator);
  ASSERT_EQ(0,
            memcmp("Very looooooooooooooong string\0"
                   "Very looooooooooooooong string 1\0"
                   "Very looooooooooooooong string 2\0"
                   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
                   s0.c_str(),
                   100));
  s1.clear();
  s1.shrink_to_fit();

  ASSERT_EQ(0,
            memcmp("Very looooooooooooooong string\0"
                   "Very looooooooooooooong string 2\0"
                   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                   "\0\0",
                   s0.c_str(),
                   100));

  ASSERT_NO_THROW(s1 = make_astring(experimental::constants::VERY_LONG_STRING_3,
                                    arenaAllocator));

  ASSERT_EQ(0,
            memcmp("Very looooooooooooooong string\0"
                   "Very looooooooooooooong string 2\0"
                   "Very looooooooooooooong string 3\0"
                   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
                   s0.c_str(),
                   100));
}
