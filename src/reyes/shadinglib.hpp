#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace shading
    {
        struct Texture2D : public Resource<uv, color>
        {
            uint16_t width, height;
            char* data;

            Texture2D(uint16_t w, uint16_t h)
                : width(w)
                , height(h)
            {}

            bool loadBMP(const char* filename)
            {
                unsigned char header[54];
                unsigned int dataPos;
                unsigned int width, height;
                unsigned int imageSize;
                data = 0;

                // Open the file
                FILE * file = fopen(filename, "rb");
                if (!file)
                    return false;
                if (fread(header, 1, 54, file) != 54)
                    return false;

                if (header[0] != 'B' || header[1] != 'M')
                    return false;

                // Read ints from the byte array
                dataPos = *(int*)&(header[0x0A]);
                imageSize = *(int*)&(header[0x22]);
                width = *(int*)&(header[0x12]);
                height = *(int*)&(header[0x16]);

                if (imageSize == 0)    imageSize = width*height * 3;
                if (dataPos == 0)      dataPos = 54;
                data = new char[imageSize];
                if (!data)
                    return false;
                fread(data, 1, imageSize, file);
                fclose(file);

                return true;
            }

            color sample(uv uv)
            {
                // repeat
                /*while (uv.x > 1.0f) uv.x -= 1.0f;
                while (uv.y > 1.0f) uv.y -= 1.0f;*/

                // calc offset
                uint16_t x = (uint16_t)(uv.x*width);
                uint16_t y = (uint16_t)(uv.y*height);
                uint16_t offset = y*width + x;

                // sample
                return{ data[3 * offset] / 255.0f, data[3 * offset + 1] / 255.0f, data[3 * offset + 2] / 255.0f, 1.0f };
            }
        };
    }
}

namespace reyes
{
    namespace shading
    {
        struct PointLight : public Light
        {
            color col;
            position pos;
            color sample(PosNormal point)
            {
                float dist = (pos - point.p).len();
                float max_dist = 10.0f;
                float k = 1.0f - dist / max_dist;
                k = (k < 0.0 ? 0.0 : k);
                return{ col.r*k, col.g*k, col.b*k, 1.0f };
            }
        };

        struct DirectionalLight : public Light
        {
            vec3 direction;
            color col;
            color sample(PosNormal point)
            {
                vec3 n = point.n.normalize();
                direction.normalize();
                float k = n.x*direction.x + n.y*direction.y + n.z*direction.z;
                k = k < 0.0f ? 0.0f : k;
                return{ col.r*k, col.g*k, col.b*k, 1.0f };
            }
        };

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

#define UNIFORM(matName) struct matName##UnifBlk
#define MATERIAL(matName) struct matName : public Material<matName##UnifBlk>
#define DISPLACE position pShdr(PosNormalUV& vertex)
#define SHADE color cShdr(PosNormalUV& vertex)

namespace reyes
{
    namespace shading
    {
        // ------------------------------------------- Lambert
        UNIFORM(Lambert)
        {
            uint8_t lightCnt;
            Light** lights;
            color color;
            LambertUnifBlk()
                : lightCnt(0)
                , lights(0)
            {}
        };
        MATERIAL(Lambert)
        {
            DISPLACE
            {
                return vertex.p;
            }

            SHADE
            {
                PosNormal pn(vertex.p, vertex.n);
                color total;
                for (uint8_t i = 0; i < uniform.lightCnt; i++)
                {
                    color lightcol = uniform.lights[i]->sample(pn);
                    total.r += lightcol.r*uniform.color.r;
                    total.g += lightcol.g*uniform.color.g;
                    total.b += lightcol.b*uniform.color.b;
                }
                total.r = total.r>1.0 ? 1.0f : total.r;
                total.g = total.g>1.0 ? 1.0f : total.g;
                total.b = total.b>1.0 ? 1.0f : total.b;
                return total;
            }
        };

        // ------------------------------------------- UVColor
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

        // ------------------------------------------- NormalColor
        UNIFORM(NormalColor) {};
        MATERIAL(NormalColor)
        {
            DISPLACE
            {
                return vertex.p;
            }

            SHADE
            {
                return{ vertex.n.x, vertex.n.y, vertex.n.z, 1 };
            }
        };

        // ------------------------------------------- SimpleTex2D
        UNIFORM(SimpleTex2D)
        {
            Texture2D* tex;
        };
        MATERIAL(SimpleTex2D)
        {
            DISPLACE
            {
                return vertex.p;
            }

            SHADE
            {
                return uniform.tex->sample(vertex.uv);
            }
        };

        // ------------------------------------------- Yellow
        UNIFORM(Yellow)
        {
        };
        MATERIAL(Yellow)
        {
            DISPLACE
            {
            return vertex.p;
        }

            SHADE
            {
                return{1,1,0,1};
            }
        };
    }
}

#undef UNIFORM
#undef MATERIAL
#undef DISPLACE
#undef SHADE