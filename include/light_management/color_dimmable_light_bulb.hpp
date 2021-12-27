#pragma once

#include <iostream>

struct color_dimmable_light_bulb_t final
{
    void draw(std::ostream &oss, size_t position = 0) const
    {
        oss << std::string(position, ' ') << "<color_dimmable_light_bulb_t/>\n";
    }
};
