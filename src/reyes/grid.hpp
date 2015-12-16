#pragma once

#include <cstdint>
#include <cmath>
#include "primitive.hpp"
#include "misc.hpp"

namespace reyes
{
    //struct Microgrid
    //{};

    //template <class VertexTy>
    //struct MicrogridI
    //{
    //    VertexTy* data;
    //    uint16_t* indices;
    //    virtual Primitive* at(uint16_t idx) = 0;
    //    virtual uint16_t count() = 0;
    //    //virtual uint16_t vCnt() = 0;
    //    virtual void transform(mx4 m) = 0;
    //    virtual AABB2 aabb(void) = 0;
    //};

    //template <class VertexTy, size_t verticesCnt, size_t indicesCnt>
    //struct GridI : public MicrogridI<VertexTy>
    //{
    //    VertexTy data[verticesCnt];
    //    uint16_t indices[indicesCnt];
    //    virtual Primitive* at(uint16_t idx) = 0;
    //    virtual uint16_t count() = 0;
    //    //virtual uint16_t vCnt() { return verticesCnt; }
    //    void transform(mx4 m)
    //    {
    //        //vec4 p;
    //        //for (uint16_t i = 0; i < verticesCnt; i++)
    //        //{
    //        //    p = vec4(data[i].p.x, data[i].p.y, data[i].p.z, 1.0f);
    //        //    // mul p and m
    //        //    // divide with w
    //        //    // store back
    //        //}
    //    }

    //    AABB2 aabb(void)
    //    {
    //        AABB2 bb;
    //        bb.max = vec2(data[0].p.x, data[0].p.y);
    //        bb.min = bb.max;
    //        for (uint16_t i = 1; i < verticesCnt; i++)
    //        {
    //            vec3 p = data[i].p;
    //            bb.min = vec2(fminf(bb.min.x, p.x), fminf(bb.min.y, p.y));
    //            bb.max = vec2(fmaxf(bb.max.x, p.x), fmaxf(bb.max.y, p.y));
    //        }
    //        return bb;
    //    }
    //};


    ///*
    //template<size_t verticesCnt, size_t indicesCnt>
    //struct GGridI : public GridI<PosNormalUV, verticesCnt, indicesCnt>
    //{};
    //*/

    ///*
    //template<size_t verticesCnt, size_t indicesCnt>
    //struct SGridI : public GridI<PosColor, Primitive, verticesCnt, indicesCnt>
    //{};
    //*/

    //template<size_t verticesCnt, size_t indicesCnt>
    //struct GTriGrid : public GridI<PosNormalUV, verticesCnt, indicesCnt> //, public GGridI<verticesCnt, indicesCnt>
    //{
    //    GTriangle* at(uint16_t idx)
    //    {
    //        GTriangle* t = new GTriangle;
    //        t->a = data[indices[3 * idx + 0]];
    //        t->b = data[indices[3 * idx + 1]];
    //        t->c = data[indices[3 * idx + 2]];
    //        return t;
    //    }
    //    uint16_t count() { return indicesCnt / 3; }
    //};

    //template<size_t verticesCnt, size_t indicesCnt>
    //struct STriGrid : public GridI<PosColor, verticesCnt, indicesCnt> /*, public SGridI */
    //{
    //    STriangle* at(uint16_t idx)
    //    {
    //        STriangle* t = new STriangle;
    //        t->a = data[indices[3 * idx + 0]];
    //        t->b = data[indices[3 * idx + 1]];
    //        t->c = data[indices[3 * idx + 2]];
    //        return t;
    //    }
    //    uint16_t count() { return indicesCnt / 3; }
    //};

    //template<size_t verticesCnt, size_t indicesCnt>
    //struct GQuadGrid : public GridI<PosNormalUV, verticesCnt, indicesCnt> //, public GGridI<verticesCnt, indicesCnt>
    //{
    //    GQuad* at(uint16_t idx)
    //    {
    //        GQuad* q = new GQuad;
    //        q->a = data[indices[4 * idx + 0]];
    //        q->b = data[indices[4 * idx + 1]];
    //        q->c = data[indices[4 * idx + 2]];
    //        q->d = data[indices[4 * idx + 3]];
    //        return q;
    //    }
    //    uint16_t count() { return indicesCnt / 4; }
    //};

    //template<size_t verticesCnt, size_t indicesCnt>
    //struct SQuadGrid : public GridI<PosColor, verticesCnt, indicesCnt> /*, public SGridI */
    //{
    //    SQuad* at(uint16_t idx)
    //    {
    //        SQuad* q = new SQuad;
    //        q->a = data[indices[4 * idx + 0]];
    //        q->b = data[indices[4 * idx + 1]];
    //        q->c = data[indices[4 * idx + 2]];
    //        q->d = data[indices[4 * idx + 3]];
    //        return q;
    //    }
    //    uint16_t count() { return indicesCnt / 4; }
    //};

    enum MicrogridType { QUAD=0, TRIANGLE };
    struct Microgrid
    {
        Vertex* vertices;
        Index* indices;
        uint16_t vertCount;
        uint16_t indiCount;
        MicrogridType type;

        Microgrid(MicrogridType _ty, uint16_t vcnt, uint16_t icnt)
            : type(_ty)
            , vertCount(vcnt)
            , indiCount(icnt)
            , vertices(new Vertex[vertCount])
            , indices(new Index[indiCount])
        {}

        ~Microgrid()
        {
            delete[] vertices;
            delete[] indices;
            vertices = 0;
            indices = 0;
        }

        uint16_t primCount(void)
        {
            if (MicrogridType::TRIANGLE == type)
            {
                return indiCount / 3;
            }
            else if (MicrogridType::QUAD == type)
            {
                return indiCount / 4;
            }
        }

        primitive::PrimitiveI* operator[](Index idx)
        {
            if (MicrogridType::TRIANGLE == type)
            {
                primitive::Triangle* t = new primitive::Triangle;
                t->a = vertices[indices[3 * idx + 0]];
                t->b = vertices[indices[3 * idx + 1]];
                t->c = vertices[indices[3 * idx + 2]];
                return t;
            }
            else if (MicrogridType::QUAD == type)
            {
                primitive::Quad* q = new primitive::Quad;
                q->a = vertices[indices[4 * idx + 0]];
                q->b = vertices[indices[4 * idx + 1]];
                q->c = vertices[indices[4 * idx + 2]];
                q->d = vertices[indices[4 * idx + 3]];
                return q;
            }
        }

        /* Bounding box for entire grid. */
        AABB2 aabb(void)
        {
            AABB2 bb;
            bb.max = vec2(vertices[0].p.x, vertices[0].p.y);
            bb.min = bb.max;
            for (uint16_t i = 1; i < vertCount; i++)
            {
                vec3 p = vertices[i].p;
                bb.min = vec2(fminf(bb.min.x, p.x), fminf(bb.min.y, p.y));
                bb.max = vec2(fmaxf(bb.max.x, p.x), fmaxf(bb.max.y, p.y));
            }
            return bb;
        }

        void transform(mx4 m)
        {
            // TODO
        }
    };
}