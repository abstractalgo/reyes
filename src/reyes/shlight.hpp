#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        struct SHLight : public Light
        {
            float* coefficients;
            uint16_t count;
            color sample(PosNormal point)
            {
                // TODO: sample spherical harmonics light probe
                // caclulate items and mul with coefficients
                return{ 0, 0, 0, 1 };
            }
        };
    }
}