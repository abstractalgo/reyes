#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        struct SinCosShader : public TextureSampler
        {
            color sample(uv input)
            {
                float k = sin(input.x * M_PI * 20.0)*cos(input.y * 40.0);
                return{ k, k, k, 1 };
            }
        };
    }
}