#pragma once

#include "color.hpp"

#define UNIFORM(matName) struct matName##UnifBlk
#define MATERIAL(matName) struct matName : public Material<matName##UnifBlk>
#define DISPLACE position pShdr(Vertex vertex)
#define SHADE color cShdr(Vertex vertex)

namespace reyes
{
    template<class UniformBlockTy>
    /* Material interface and storage. */
    struct Material
    {
        UniformBlockTy uniform;
        virtual position pShdr(Vertex a) = 0;
        virtual color cShdr(Vertex a) = 0;
    };

    template<class InputTy, class OutputTy>
    /* Resource interface. Base for textures and lights. */
    struct Resource
    {
        virtual OutputTy sample(InputTy input) = 0;
    };
    typedef Resource<uv, color> Sampler;

    /* Light interface. */
    struct Light : public Resource<PosNormal, color>
    {
        /* Returns RGB color. Alpha serves as a intensity, but colors
        should be premultiplied before returning value. */
        virtual color sample(PosNormal point) = 0;
    };
}