#pragma once

#include "vecmx.hpp"
#include "color.hpp"

struct VertexDescDefault
{
    reyes::color c;
    reyes::position p;
};

//struct Material
//{
//    virtual void pShdr(VertexDesc& vertex)
//    {
//    }
//    virtual void cShdr(VertexDesc& vertex)
//    {
//    }
//};

#define UNIFORM_BEGIN struct Uniform {
#define UNIFORM_END } uniform;
#define VERTEX_BEGIN struct VertexDesc : public VertexDescDefault {
#define VERTEX_END };
#define PSHDR_BEGIN void pShdr(VertexDesc& vertex) {
#define PSHDR_END }
#define CSHDR_BEGIN void cShdr(VertexDesc& vertex) {
#define CSHDR_END }

struct Lambert/* : public Material*/
{
    UNIFORM_BEGIN
        reyes::mx4 model;
        int a;
    UNIFORM_END

    VERTEX_BEGIN
        reyes::normal n;
        reyes::uv uv;
    VERTEX_END

    PSHDR_BEGIN
        uniform.a = 1;
    PSHDR_END

    CSHDR_BEGIN
    CSHDR_END
};

struct SolidColor/* : public Material*/
{
    UNIFORM_BEGIN
        reyes::color color;
    UNIFORM_END

    VERTEX_BEGIN
        reyes::uv _uv;
    VERTEX_END

    PSHDR_BEGIN
    PSHDR_END

    CSHDR_BEGIN
        vertex.c = uniform.color;
    CSHDR_END
};

#define USE_MAT(_name) _name, _name::VertexDesc

typedef uint16_t Index;

#define TRIANGLE_GRID 0
#define QUAD_GRID 1
#define GRID_DIM 80
#define GRID_TYPE TRIANGLE_GRID

struct AbstractShape
{
    virtual void dice() = 0;
    virtual void shade() = 0;
    virtual void split() = 0;
    virtual void rasterize(void* c) = 0;
};

template<class MatTy, class VertexTy>
struct Shape : public AbstractShape
{
    MatTy* material;
    VertexTy grid[(GRID_DIM + 1)*(GRID_DIM + 1)];
#if GRID_TYPE==0
    Index indices[GRID_DIM*GRID_DIM * 6];
#else
    Index indices[GRID_DIM*GRID_DIM * 4];
#endif

    Shape(MatTy* material = 0)
        : material(material)
    {
        if (!material)
            material = new MatTy;
    }

    void dice()
    {
        // popuni vertekse
        // popuni indice - mogu se izmestiti u zasebnu klasu/strukturu
        for (uint16_t i = 0; i < (GRID_DIM + 1)*(GRID_DIM + 1); i++)
        {
            material->pShdr(grid[i]);
        }
    }

    void shade()
    {
        for (uint16_t i = 0; i < (GRID_DIM + 1)*(GRID_DIM + 1); i++)
        {
            material->cShdr(grid[i]);
        }
    }

    bool requiresSplit()
    {
        // calc bounding box
        for (uint16_t i = 0; i < (GRID_DIM + 1)*(GRID_DIM + 1); i++)
        {

        }
        // compare to threshold
    }

    virtual void split()
    {

    }

    void rasterize(void* c)
    {
        // do shit
    }
};

template<class MatTy, class VertexTy>
struct ParametricSurface : public Shape<MatTy, VertexTy>
{
    float start_u, end_u, start_v, end_v;
    ParametricSurface(MatTy* material=0)
        : Shape(material)
    {}
    void split()
    {
        // napravi cetiri nova
        for (unsigned char i = 0; i < 4; i++)
        {
            ParametricSurface<MatTy, VertexTy>* shp = new ParametricSurface<MatTy, VertexTy>(material);
        }
        // raspodeli uv
    }
};