#pragma once

#include <cstdint>
#include "camera.hpp"
#include "primitive.hpp"
#include "misc.hpp"

namespace reyes
{
    template<class VertexTy>
    struct GridI
    {
        VertexTy* data;
        uint16_t* indices;
        uint16_t cnt;
        virtual PrimitiveI<VertexTy>* at(uint16_t idx) = 0;
        void shade();
        uint16_t count() { return cnt; }

        GridI(uint16_t dataCount, uint16_t indicesCount, uint16_t primitiveCount)
            : data(new VertexTy[dataCount])
            , indices(new uint16_t[indicesCount])
            , cnt(primitiveCount)
        {}
    };

    template<class VertexTy>
    struct TriGrid : public GridI<VertexTy>
    {
        TriGrid()
            : GridI(dataCout, indicesCount, primitiveCount)
        {}

        Triangle<VertexTy>* at(uint16_t idx)
        {
            Triangle<VertexTy>& t = *(new Triangle<VertexTy>);
            t.a = (VertexTy)data[indices[3 * idx + 0]];
            t.b = (VertexTy)data[indices[3 * idx + 1]];
            t.c = (VertexTy)data[indices[3 * idx + 2]];
            return &t;
        }
    };

    template<class VertexTy>
    struct QuadGrid : public GridI<VertexTy>
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

    template<class VertexTy>
    struct PolyGrid : public GridI<VertexTy>
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

    template<>
    struct TriGrid<PosNormalMat>
    {
        void shade(mem::ObjectStack<GridI<PosColor>>& stack)
        {
            // TODO
            GridI<PosColor>* gr = new(stack.alloc(sizeof(TriGrid<PosColor>))) TriGrid<PosColor>(0, 0, 0);
        }
    };

    
}