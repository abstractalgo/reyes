#pragma once

#include <cstdint>
#include "camera.hpp"
#include "primitive.hpp"
#include "misc.hpp"

namespace reyes
{
    //template<class GridDataTy>
    ///* Template struct for grid data. */
    //struct Grid
    //{
    //    GridDataTy* data;
    //    uint16_t count;

    //    // used for other primitives (TODO later)
    //    uint16_t* indices;
    //    uint8_t size;

    //    void project(camera& camera)
    //    {
    //        // TODO
    //    }
    //};

    //template<>
    ///* Grid with only positions. */
    //struct Grid<position>
    //{
    //    bool visible(camera& camera)
    //    {
    //        // TODO
    //        return false;
    //    }

    //    vec2 rasterEstimate()
    //    {
    //        // TODO
    //        return{ 0, 0 };
    //    }
    //};

    //template<>
    ///* Grid with information required for shading. */
    //struct Grid<PosNormalMat>
    //{
    //    Grid<PosColor> shade(void)
    //    {
    //        // TODO
    //        return Grid<PosColor>{};
    //    }
    //};

    //template<>
    ///* All in one grid structure (consumes more data, but no reallocations). */
    //struct Grid<PosNormalMatColor>
    //{
    //    
    //};

    template<class DataTy>
    struct GridI
    {
        DataTy* data;
        uint16_t* indices;
        uint16_t cnt;
        PrimTy at(uint16_t idx) = 0;
        uint16_t count() { return cnt; }
    };

    template<class DataTy>
    struct TriGrid : public GridI<DataTy>
    {
        Triangle at(uint16_t idx)
        {
            Triangle t;
            t.a.color       = data[indices[3 * idx + 0]].color;
            t.a.position    = data[indices[3 * idx + 0]].position;
            t.b.color       = data[indices[3 * idx + 1]].color;
            t.b.position    = data[indices[3 * idx + 1]].position;
            t.c.color       = data[indices[3 * idx + 2]].color;
            t.c.position    = data[indices[3 * idx + 2]].position;
            return t;
        }
    };

    template<class DataTy>
    struct QuadGrid : public GridI<DataTy>
    {
        Quadrilateral at(uint16_t idx)
        {
            Quadrilateral q;
            q.a.color       = data[indices[4 * idx + 0]].color;
            q.a.position    = data[indices[4 * idx + 0]].position;
            q.b.color       = data[indices[4 * idx + 1]].color;
            q.b.position    = data[indices[4 * idx + 1]].position;
            q.c.color       = data[indices[4 * idx + 2]].color;
            q.c.position    = data[indices[4 * idx + 2]].position;
            q.d.color       = data[indices[4 * idx + 3]].color;
            q.d.position    = data[indices[4 * idx + 3]].position;
            return q;
        }
    };

    template<class DataTy>
    struct PolyGrid<DaraTy> : public GridI<DataTy>
    {
        Polygon at(uint16_t idx)
        {
            Polygon p;
            uint16_t offset = indices[2 * idx + 0];
            uint16_t size   = indices[2 * idx + 1];
            p.count = size;
            p.vertices = new PosColor[size];
            for (uint16_t i = 0; i < size; i++)
            {
                DataTy vertex           = data[indeices[offset + i]];
                p.vertices[i].position  = vertex.position;
                p.vertices[i].color     = vertex.color;
            }
            return p;
        }
    };

    template<>
    struct GridI<PosNormalMat>
    {
        GridI<PosColor> shade(void)
        {
            // TODO
            return GridI<PosColor>{};
        }
    };

    
}