#pragma once

#include <cstdint>
#include "camera.hpp"
#include "primitive.hpp"
#include "misc.hpp"

namespace reyes
{
    struct GridI
    {};

    template <class VertexTy>
    struct GridVertexTy : public GridI
    {
        
    };

    template<class VertexTy, uint16_t dataSize, uint16_t indicesCount, uint16_t primitiveCount>
    struct GridGeometry : public GridVertexTy<VertexTy>
    {
        VertexTy data[dataSize];
        uint16_t indices[indicesCount];
        virtual PrimitiveI<VertexTy>* at(uint16_t idx) = 0;
        uint16_t count() { return primitiveCount; }
    };

    // -------------------------------------------------------------------------

    template<class VertexTy, uint16_t dataSize, uint16_t indicesCount, uint16_t primitiveCount>
    struct TriGrid : public GridGeometry<VertexTy, dataSize, indicesCount, primitiveCount>
    {
        Triangle<VertexTy>* at(uint16_t idx)
        {
            Triangle<VertexTy>& t = *(new Triangle<VertexTy>);
            t.a = data[indices[3 * idx + 0]];
            t.b = data[indices[3 * idx + 1]];
            t.c = data[indices[3 * idx + 2]];
            return &t;
        }
    };

    template<class VertexTy, uint16_t dataSize, uint16_t indicesCount, uint16_t primitiveCount>
    struct QuadGrid : public GridGeometry<VertexTy, dataSize, indicesCount, primitiveCount>
    {
        Quadrilateral<VertexTy>* at(uint16_t idx)
        {
            Quadrilateral<VertexTy>& q = *(new Quadrilateral<VertexTy>);
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
        Polygon<VertexTy>* at(uint16_t idx)
        {
            Polygon<VertexTy>& p = *(new Polygon<VertexTy>);
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

    // -------------------------------------------------------------------------

}