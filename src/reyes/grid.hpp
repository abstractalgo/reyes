#pragma once

#include <cstdint>
#include "primitive.hpp"
#include "camera.hpp"
#include "misc.hpp"

namespace reyes
{
    struct GridI
    {};

    template <class VertexTy>
    struct GridVertexTy : public GridI
    {
        VertexTy* data;
        uint16_t* indices;
        virtual primitive::PrimitiveI<VertexTy>* at(uint16_t idx) = 0;
        virtual uint16_t count() = 0;
    };

    template<class VertexTy, uint16_t dataSize, uint16_t indicesCount, uint16_t primitiveCount>
    struct GridGeometry : public GridVertexTy<VertexTy>
    {
        VertexTy data[dataSize];
        uint16_t indices[indicesCount];
        virtual primitive::PrimitiveI<VertexTy>* at(uint16_t idx) = 0;
        uint16_t count() { return primitiveCount; }
    };

    // --- additional functions for pipeline -----------------------------------

    //template<uint16_t dataSize, uint16_t indicesCount, uint16_t primitiveCount>
    //struct GridGeometry<PosNormalUV, dataSize, indicesCount, primitiveCount>
    //{
    //    void transform(mx4& mx)
    //    {
    //        for (uint16_t i = 0; i < dataSize; i++)
    //        {
    //            // TODO
    //        }
    //    }

    //    void project(camera& camera)
    //    {
    //        transform(camera.view);
    //        transform(camera.projection);
    //    }
    //};

    // --- primitive-typed grids -----------------------------------------------

    template<class VertexTy, uint16_t dataSize, uint16_t indicesCount>
    struct TriGrid : public GridGeometry<VertexTy, dataSize, indicesCount, indicesCount/3>
    {
        primitive::Triangle<VertexTy>* at(uint16_t idx)
        {
            primitive::Triangle<VertexTy>& t = *(new primitive::Triangle<VertexTy>);
            t.a = data[indices[3 * idx + 0]];
            t.b = data[indices[3 * idx + 1]];
            t.c = data[indices[3 * idx + 2]];
            return &t;
        }
    };

    template<class VertexTy, uint16_t dataSize, uint16_t indicesCount>
    struct QuadGrid : public GridGeometry<VertexTy, dataSize, indicesCount, indicesCount/4>
    {
        primitive::Quadrilateral<VertexTy>* at(uint16_t idx)
        {
            primitive::Quadrilateral<VertexTy>& q = *(new primitive::Quadrilateral<VertexTy>);
            q.a = data[indices[4 * idx + 0]];
            q.b = data[indices[4 * idx + 1]];
            q.c = data[indices[4 * idx + 2]];
            q.d = data[indices[4 * idx + 3]];
            return &q;
        }
    };

    template<class VertexTy, uint16_t dataSize, uint16_t indicesCount, uint16_t primitiveCount>
    struct PolyGrid : public GridGeometry<VertexTy, dataSize, indicesCount, primitiveCount>
    {
        primitive::Polygon<VertexTy>* at(uint16_t idx)
        {
            primitive::Polygon<VertexTy>& p = *(new primitive::Polygon<VertexTy>);
            uint16_t offset = indices[2 * idx + 0];
            uint16_t size   = indices[2 * idx + 1];
            p.count = size;
            p.vertices = new VertexTy[size];
            for (uint16_t i = 0; i < size; i++)
            {
                p.vertices[i] = data[indices[offset + i]];
            }
            return &p;
        }
    };
}