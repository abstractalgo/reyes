#pragma once

#include <cstdint>
#include "primitive.hpp"
#include "misc.hpp"

namespace reyes
{
    struct Microgrid
    {};

    template <class VertexTy>
    struct MicrogridI
    {
        VertexTy* data;
        uint16_t* indices;
        virtual Primitive* at(uint16_t idx) = 0;
        virtual uint16_t count() = 0;
        virtual void transform(mx4 m) = 0;
    };

    template <class VertexTy, size_t verticesCnt, size_t indicesCnt>
    struct GridI : public MicrogridI<VertexTy>
    {
        VertexTy data[verticesCnt];
        uint16_t indices[indicesCnt];
        virtual Primitive* at(uint16_t idx) = 0;
        virtual uint16_t count() = 0;
        void transform(mx4 m)
        {
            vec4 p;
            for (uint16_t i = 0; i < verticesCnt; i++)
            {
                p = vec4(data[i].p.x, data[i].p.y, data[i].p.z, 1.0f);
                // mul p and m
                // divide with w
                // store back
            }
        }
    };

    
    /*
    template<size_t verticesCnt, size_t indicesCnt>
    struct GGridI : public GridI<PosNormalUV, verticesCnt, indicesCnt>
    {};
    */

    /*
    template<size_t verticesCnt, size_t indicesCnt>
    struct SGridI : public GridI<PosColor, Primitive, verticesCnt, indicesCnt>
    {};
    */

    template<size_t verticesCnt, size_t indicesCnt>
    struct GTriGrid : public GridI<PosNormalUV, verticesCnt, indicesCnt> //, public GGridI<verticesCnt, indicesCnt>
    {
        GTriangle* at(uint16_t idx)
        {
            GTriangle* t = new GTriangle;
            t->a = data[indices[3 * idx + 0]];
            t->b = data[indices[3 * idx + 1]];
            t->c = data[indices[3 * idx + 2]];
            return t;
        }
        uint16_t count() { return indicesCnt / 3; }
    };

    template<size_t verticesCnt, size_t indicesCnt>
    struct STriGrid : public GridI<PosColor, verticesCnt, indicesCnt> /*, public SGridI */
    {
        STriangle* at(uint16_t idx)
        {
            STriangle* t = new STriangle;
            t->a = data[indices[3 * idx + 0]];
            t->b = data[indices[3 * idx + 1]];
            t->c = data[indices[3 * idx + 2]];
            return t;
        }
        uint16_t count() { return indicesCnt / 3; }
    };

    template<size_t verticesCnt, size_t indicesCnt>
    struct GQuadGrid : public GridI<PosNormalUV, verticesCnt, indicesCnt> //, public GGridI<verticesCnt, indicesCnt>
    {
        GQuad* at(uint16_t idx)
        {
            GQuad* q = new GQuad;
            q->a = data[indices[4 * idx + 0]];
            q->b = data[indices[4 * idx + 1]];
            q->c = data[indices[4 * idx + 2]];
            q->d = data[indices[4 * idx + 3]];
            return q;
        }
        uint16_t count() { return indicesCnt / 4; }
    };

    template<size_t verticesCnt, size_t indicesCnt>
    struct SQuadGrid : public GridI<PosColor, verticesCnt, indicesCnt> /*, public SGridI */
    {
        SQuad* at(uint16_t idx)
        {
            SQuad* q = new SQuad;
            q->a = data[indices[4 * idx + 0]];
            q->b = data[indices[4 * idx + 1]];
            q->c = data[indices[4 * idx + 2]];
            q->d = data[indices[4 * idx + 3]];
            return q;
        }
        uint16_t count() { return indicesCnt / 4; }
    };
}