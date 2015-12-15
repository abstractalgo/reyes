#pragma once

#include "mem.hpp"
#include "vecmx.hpp"
#include "grid.hpp"
#include "misc.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "shading.hpp"

namespace reyes
{
    /* Enum used to determine split direction of the Shape. U - vertical cut, V - hozirontal cut*/
    enum SplitDir { U, V, NoSplit };
    SplitDir split_dir(vec2 size, vec2 threshold)
    {
        if (size.x <= threshold.x && size.y <= threshold.y)
            return SplitDir::NoSplit;
        return ((size.x > size.y)
            ? SplitDir::V
            : SplitDir::U);
    }

    template<class MaterialTy>
    struct ParametricSurface
    {
        float start_u, end_u, start_v, end_v;
        ParametricSurface()
            : start_u(0)
            , end_u(1)
            , start_v(0)
            , end_v(1)
        {}

        MaterialTy material;
        mx4 transform;

        ~ParametricSurface() { delete material; }

        void splitUV(SplitDir direction, ParametricSurface& one, ParametricSurface& two)
        {
            if (U == direction)
            {
                one.start_u = start_u;
                one.end_u = end_u;
                one.start_v = start_v;
                one.end_v = (start_v + end_v)*0.5f;

                two.start_u = start_u;
                two.end_u = end_u;
                two.start_v = (start_v + end_v)*0.5f;
                two.end_v = end_v;
            }
            else
            {
                one.start_u = start_u;
                one.end_u = (start_u + end_u)*0.5f;
                one.start_v = start_v;
                one.end_v = end_v;

                two.start_u = (start_u + end_u)*0.5f;
                two.end_u = end_u;
                two.start_v = start_v;
                two.end_v = end_v;
            }
        }

        virtual void split(SplitDir direction, Scene& scene) = 0;
        virtual mem::blk dice(CameraTransform* camera, mem::AllocatorI* alloc) = 0;
        virtual mem::blk shade(MicrogridI<PosNormalUV>* dgrid, mem::AllocatorI* alloc) = 0;
        virtual position P(uv uv) = 0;
        virtual normal N(uv uv) = 0;
    };
}