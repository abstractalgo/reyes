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
        for (uint16_t i = 0; i < GRID_SIZE; i++)
        {
            material->pShdr(grid[i]);
        }
    }

    void shade()
    {
        for (uint16_t i = 0; i < GRID_SIZE; i++)
        {
            material->cShdr(grid[i]);
        }
    }

    bool requiresSplit()
    {
        // calc bounding box
        for (uint16_t i = 0; i < GRID_SIZE; i++)
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

/*ParametricSurface<USE_MAT(Lambert)> oblik;
ParametricSurface<USE_MAT(SolidColor)> kugla;
oblik.dice();
oblik.shade();
AbstractShape* shapes[2];
shapes[0] = &oblik;
shapes[1] = &kugla;

for (uint16_t i = 0; i < 2; i++)
{
shapes[i]->dice();
shapes[i]->split();
shapes[i]->shade();
shapes[i]->rasterize(0);
}*/

/*
// calculate model-world values
vx.uv = uv(start_u + u*wu, start_v + v*wv);
vx.p = P(_uv);
vx.n = N(_uv);
vx.p = material.pShdr(vx);

// TODO transformations: model, view, projection

// transform vertex
// 1. scale
vx.p.x *= transform.S.x;
vx.p.y *= transform.S.y;
vx.p.z *= transform.S.z;
// 2. rotate (XYZ order)
// 1 0  0
// 0 c -s
// 0 s  c
float Rx = transform.R.x;
vx.p.y = vx.p.y*cos(Rx) + sin(Rx)*vx.p.z;
vx.p.z = -vx.p.y*sin(Rx) + cos(Rx)*vx.p.z;
//  c 0 s
//  0 1 0
// -s 0 c
float Ry = transform.R.y;
vx.p.x = vx.p.x*cos(Ry) - sin(Ry)*vx.p.z;
vx.p.z = vx.p.x*sin(Ry) + cos(Ry)*vx.p.z;
// c -s 0
// s  c 0
// 0  0 1
float Rz = transform.R.z;
vx.p.x = vx.p.x*cos(Rz) + sin(Rz)*vx.p.y;
vx.p.y = -vx.p.x*sin(Rz) + cos(Rz)*vx.p.y;
// 3. translate
vx.p.x += transform.T.x;
vx.p.y += transform.T.y;
vx.p.z += transform.T.z;

// transform normal
vx.n.y = vx.n.y*cos(Rx) + sin(Rx)*vx.n.z;
vx.n.z = -vx.n.y*sin(Rx) + cos(Rx)*vx.n.z;
vx.n.x = vx.n.x*cos(Ry) - sin(Ry)*vx.n.z;
vx.n.z = vx.n.x*sin(Ry) + cos(Ry)*vx.n.z;
vx.n.x = vx.n.x*cos(Rz) + sin(Rz)*vx.n.y;
vx.n.y = -vx.n.x*sin(Rz) + cos(Rz)*vx.n.y;

vx.n.x *= transform.S.x;
vx.n.y *= transform.S.y;
vx.n.z *= transform.S.z;

vx.n.normalize();


grid->vertices[idx] = vx;
*/