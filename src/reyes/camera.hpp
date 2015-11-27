#pragma once

#include "vecmx.hpp"

namespace reyes
{
    /* Camera struct (POD type). */
    struct camera
    {
        math::mx4 view;
        math::mx4 projection;
        camera() {}

        camera& perspective(float FOV, float ncp, float fcp)
        {
            return *this;
        }

        camera& orthographic(float ncp, float fcp, float width, float height)
        {
            return *this;
        }

        /* Orient camera to look at the target position, with selected up vector, from the selected origin. */
        camera& lookAt(const math::vec3 from, math::vec3 to, const math::vec3 up = { 0, 1, 0 })
        {
            return *this;
        }
    };
}