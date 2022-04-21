#include <gmock/gmock.h>

#include <document.hpp> // https://www.youtube.com/watch?v=QGcVXgEVMJg

TEST(runtime_polymorphism, draw_plain_plant) {
  collection_t c;
  c.emplace_back(42);

  std::ostringstream oss;
  draw(c, oss, 0);
  ASSERT_EQ("42\n", oss.str());
}
