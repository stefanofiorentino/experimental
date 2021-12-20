#include <gmock/gmock.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <variant>

#include <light_management/type_traits.hpp>
#include <light_management/light_bulb.hpp>
#include <light_management/dimmable_light_bulb.hpp>
#include <light_management/color_dimmable_light_bulb.hpp>

using light_concept_t = std::variant<light_bulb_t, dimmable_light_bulb_t, color_dimmable_light_bulb_t>;
using collection_t = std::vector<light_concept_t>;

void draw(collection_t & collection, std::ostream& os)
{
    os << "<document>\n";
    std::for_each(collection.begin(), collection.end(), [&os](auto const& light_){
        std::visit([&os](auto&& light) {
            if constexpr (has_void_draw_v<decltype(light)>)
            {
                light.draw(os);
            }
        }, std::move(light_));
    });
    os << "</document>\n";
}

void do_switch(collection_t & collection, bool status)
{
    std::for_each(collection.begin(), collection.end(), [status](auto&& light_){
        std::visit([status](auto&& light) {
            if constexpr (has_void_do_switch_v<decltype(light)>)
            {
                light.do_switch(status);
            }
        }, light_);
    });
}

TEST(std_variant, draw)
{
    collection_t c;
    c.emplace_back(light_bulb_t());
    c.emplace_back(dimmable_light_bulb_t());
    c.emplace_back(color_dimmable_light_bulb_t());
    
    std::ostringstream oss;
    draw(c, oss);
    ASSERT_EQ("<document>\n<light_bulb_t>false</light_bulb_t>\n<dimmable_light_bulb_t/>\n<color_dimmable_light_bulb_t/>\n</document>\n", oss.str());
}

TEST(std_variant, do_switch)
{
    collection_t c;
    c.emplace_back(light_bulb_t());
    c.emplace_back(dimmable_light_bulb_t());
    c.emplace_back(color_dimmable_light_bulb_t());
    
    std::ostringstream oss;
    do_switch(c, true);
    draw(c, oss);
    ASSERT_EQ("<document>\n<light_bulb_t>true</light_bulb_t>\n<dimmable_light_bulb_t/>\n<color_dimmable_light_bulb_t/>\n</document>\n", oss.str());
}
