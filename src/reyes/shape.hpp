#pragma once

#include "mem.hpp"
#include "vecmx.hpp"
#include "grid.hpp"
#include "misc.hpp"
#include "camera.hpp"
#include "scene.hpp"

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

    /* Abstract class for shapes. */
    struct ShapeI
    {
        mx4 transform;
        virtual void split(SplitDir direction, Scene& scene) = 0;
        virtual mem::blk dice(CameraTransform* camera, mem::AllocatorI* alloc) = 0;
        virtual mem::blk shade(MicrogridI<PosNormalUV>* dgrid, mem::AllocatorI* alloc) = 0;
        virtual position P(uint16_t idx) = 0;
        virtual normal N(uint16_t idx) = 0;
        virtual uv UV(uint16_t idx) = 0;
    };

    template<class MaterialTy>
    /* Material-typed abstract class for shapes. */
    struct Shape : public ShapeI
    {
        MaterialTy material;
    };
}