#include <cstdio>
//#include "pipeline.hpp"
#include "matlib.hpp"

using namespace reyes;

template<class VertexTy, class MaterialTy>
struct Grid
{
    VertexTy data[2];
    MaterialTy material;
};

void appMain()
{
    // scene setup
    //mem::ObjectStack<Shape> scene(1024);
    //Rectangle* r = new(scene.alloc(sizeof(Rectangle))) Rectangle({ 0, 0, 0 }, { 5, 5 });

    //// camera setup
    //camera camera;
    //camera.orthographic(-10, 10, 10, 10);
    //camera.lookAt(/*eye*/ { 0, 0, -5 }, /*target*/ { 0, 0, 0 } /*up*/);

    //// image setup
    //GBuffer image(10,10);

    // Rectangle<VertexTy, Lambert> rectangle...
    // rectangle.material = new Lambert
    // 
    //

    Grid<usemat(matlib::Lambert)> grid;
    grid.material.pShdr(grid.data[0]);


    //GridI<PosNormalMat>* gridA = new TriGrid<PosNormalMat>;
    //GridI<position>* gridB; // from gridA
    //GridI<PosColor>* gridC = new TriGrid<PosColor>;

    // render
    //reyes::render(scene, camera, image);
}