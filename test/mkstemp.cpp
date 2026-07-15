#include <gmock/gmock.h>

#include <cstdlib>
#include <experimental/array>
#include <unistd.h>

// mkstemp(3) that works
template<std::size_t N>
int
tempfd(char const (&tmpl)[N])
{
  auto s = std::experimental::to_array(tmpl);
  int fd = mkstemp(s.data());
  if (fd != -1)
    unlink(s.data());

  return fd;
}

TEST(mkstemp, simple)
{
  int fd = tempfd("/tmp/test.XXXXXX");
  int rt = close(fd);
  ASSERT_EQ(0, rt);
}
