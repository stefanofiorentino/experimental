#include <gmock/gmock.h>

#include <light_management/dimmable_light_bulb.hpp>
#include <light_management/include/document.hpp> // https://www.youtube.com/watch?v=QGcVXgEVMJg
#include <light_management/include/type_traits.hpp>
#include <light_management/light_bulb.hpp>

#include "const.hpp"

TEST(runtime_polymorphism, draw_plain_plant)
{
  collection_t c;
  c.emplace_back(on_off_light_t());
  c.emplace_back(dimmable_light_t());

  std::ostringstream oss;
  draw(c, oss, 0);
  ASSERT_EQ(EXPECTED_STRING_FULL, oss.str());
}
