#pragma once

#include "shading.hpp"
#include "directionalight.hpp"

namespace reyes
{
    namespace lib
    {
        MATERIAL(LitMat)
        {
            UNIFORM_BEGIN
                Sampler* albedoMap;
                Sampler* normalMap;
                DirectionalLight* light;
                vec3 rot;
            UNIFORM_END

            DISPLACE
            {
                return vertex.p;
            }

            SHADE
            {
                color _cn = uniform.normalMap->sample(vertex.uv);
                float Rx = uniform.rot.x;
                float Ry = uniform.rot.y;
                float Rz = uniform.rot.z;
                normal cn(_cn.r, _cn.g, _cn.b);
                cn.y = cn.y*cos(Rx) + sin(Rx)*cn.z;
                cn.z = -cn.y*sin(Rx) + cos(Rx)*cn.z;
                cn.x = cn.x*cos(Ry) - sin(Ry)*cn.z;
                cn.z = cn.x*sin(Ry) + cos(Ry)*cn.z;
                cn.x = cn.x*cos(Rz) + sin(Rz)*cn.y;
                cn.y = -cn.x*sin(Rz) + cos(Rz)*cn.y;
                cn.normalize();

                normal n(vertex.n.x + cn.x, vertex.n.y + cn.y, vertex.n.z + cn.z);
                n.normalize();
                color light = uniform.light->sample(PosNormal(vertex.p, n));
                color albedo = uniform.albedoMap->sample(vertex.uv);
                color color;
                color.r = albedo.r * light.r;
                color.g = albedo.g * light.g;
                color.b = albedo.b * light.b;
                color.a = 1;

                return color;
            }
        };
    }
}