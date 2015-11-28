#pragma once

#include "vecmx.hpp"

namespace reyes
{
    /* Camera struct (POD type). */
    struct camera
    {
        mx4 view;
        mx4 projection;

        /* Mark perspective frustum. */
        void perspective(float FOV, float ncp, float fcp)
        {
            // TODO
        }

        /* Make orthographics frustum. */
        void orthographic(float ncp, float fcp, float width, float height)
        {
            // TODO
        }

        /* Orient camera to look at the target position, with selected up vector, from the selected origin. */
        void lookAt(const vec3 from, vec3 to, const vec3 up = { 0, 1, 0 })
        {
            // TODO
        }
    };
}