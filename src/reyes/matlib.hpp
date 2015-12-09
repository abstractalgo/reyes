#pragma once

#include "material.hpp"

#define UNIFORM(matName) struct matName##UnifBlk
#define MATERIAL(matName) struct matName : public Material<matName##UnifBlk>
#define DISPLACE position pShdr(PosNormalUV& vertex)
#define SHADE color cShdr(PosNormalUV& vertex)

namespace reyes
{
    namespace matlib
    {
        UNIFORM(Lambert)
        {
            struct Light
            {
                // directional light
                vec3 position;
                color color;
            } light;
            color color;
        };
        MATERIAL(Lambert)
        {
            DISPLACE
            {
                return vertex.p;
            }

            SHADE
            {
                vec3 ld = uniform.light.position.normalize();
                vec3 nn = vertex.n.normalize();
                float k = ld.x*nn.x + ld.y*nn.y + ld.z*nn.z;
                k = k < 0.0f ? 0.0f : k;
                return {
                    uniform.color.r * k * uniform.light.color.r,
                    uniform.color.g * k * uniform.light.color.g,
                    uniform.color.b * k * uniform.light.color.b,
                    1
                };
            }
        };

        UNIFORM(UVColor) {};
        MATERIAL(UVColor)
        {
            DISPLACE
            {
                return vertex.p;
            }

            SHADE
            {
                return{ vertex.uv.x, vertex.uv.y, 0, 1 };
            }
        };

        UNIFORM(NormalColor) {};
        MATERIAL(NormalColor)
        {
            DISPLACE
            {
                return vertex.p;
            }

            SHADE
            {
                vec3 n = vertex.p.normalize();
                return{ n.x, n.y, n.z, 1 };
            }
        };
    }
}

#undef UNIFORM
#undef MATERIAL
#undef DISPLACE
#undef SHADE