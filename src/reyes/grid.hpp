#pragma once

#include <cstdint>
#include "primitive.hpp"
#include "camera.hpp"
#include "misc.hpp"

namespace reyes
{
    //struct GridI
    //{};

    //template <class VertexTy>
    //struct GridVertexTy : public GridI
    //{
    //    VertexTy* data;
    //    uint16_t* indices;
    //    virtual primitive::PrimitiveI<VertexTy>* at(uint16_t idx) = 0;
    //    virtual uint16_t count() = 0;
    //};

    //template<class VertexTy, uint16_t dataSize, uint16_t indicesCount, uint16_t primitiveCount>
    //struct GridGeometry : public GridVertexTy<VertexTy>
    //{
    //    VertexTy data[dataSize];
    //    uint16_t indices[indicesCount];
    //    virtual primitive::PrimitiveI<VertexTy>* at(uint16_t idx) = 0;
    //    uint16_t count() { return primitiveCount; }
    //};

    //// --- primitive-typed grids -----------------------------------------------

    //template<class VertexTy, uint16_t dataSize, uint16_t indicesCount>
    //struct TriGrid : public GridGeometry<VertexTy, dataSize, indicesCount, indicesCount/3>
    //{
    //    primitive::Triangle<VertexTy>* at(uint16_t idx)
    //    {
    //        Triangle<VertexTy>& t = *(new primitive::Triangle<VertexTy>);
    //        t.a = data[indices[3 * idx + 0]];
    //        t.b = data[indices[3 * idx + 1]];
    //        t.c = data[indices[3 * idx + 2]];
    //        return &t;
    //    }
    //};

    //template<class VertexTy, uint16_t dataSize, uint16_t indicesCount>
    //struct QuadGrid : public GridGeometry<VertexTy, dataSize, indicesCount, indicesCount/4>
    //{
    //    primitive::Quadrilateral<VertexTy>* at(uint16_t idx)
    //    {
    //        primitive::Quadrilateral<VertexTy>& q = *(new primitive::Quadrilateral<VertexTy>);
    //        q.a = data[indices[4 * idx + 0]];
    //        q.b = data[indices[4 * idx + 1]];
    //        q.c = data[indices[4 * idx + 2]];
    //        q.d = data[indices[4 * idx + 3]];
    //        return &q;
    //    }
    //};

    template <class VertexTy, class PrimitiveTy, size_t verticesCnt, size_t indicesCnt>
    struct GridI
    {
        VertexTy data[verticesCnt];
        uint16_t* indices[indicesCnt];
        virtual PrimitiveTy* at(uint16_t idx) = 0;
        virtual uint16_t count() = 0;
    };

    /*
    struct GGridI
    {

    };

    struct SGridI
    {

    };
    */

    template<size_t verticesCnt, size_t indicesCnt>
    struct GTriGrid : public GridI<PosNormalUV, GTriangle, verticesCnt, indicesCnt> /*, public GGridI */
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
    struct STriGrid : public GridI<PosColor, STriangle, verticesCnt, indicesCnt> /*, public SGridI */
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
    struct GQuadGrid : public GridI<PosNormalUV, GQuad, verticesCnt, indicesCnt> /*, public GGridI */
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
    struct SQuadGrid : public GridI<PosColor, SQuad, verticesCnt, indicesCnt> /*, public SGridI */
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