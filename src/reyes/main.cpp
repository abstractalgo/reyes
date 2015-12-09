#include <cstdio>
#include "pipeline.hpp"
#include "matlib.hpp"
#include "shape.hpp"

#define MAKE_SHAPE(_alloc, _shp) ::new(_alloc.alloc(sizeof(_shp))) _shp

using namespace reyes;

void appMain()
{
    // scene setup
    mem::ObjectStack<ShapeI> scene(1024);
    ShapeI* shape = MAKE_SHAPE(scene, Quadrilateral<matlib::YellowColor>);// ({ -1, 1, 0.5 }, { 1, 1, 0.5 }, { 2, -2, 0.5 }, { -2, -1, 0.5 });
    shape->split(SplitDir::U, scene);
    shape = MAKE_SHAPE(scene, Quadrilateral<matlib::YellowColor>) ({ 0, 2, 1 }, { 1, 2, 1 }, { 2, 0, 1 }, { -1, -1, 0 });
    shape->split(SplitDir::V, scene);

    // camera setup
    Camera<GBuffer, 320, 240> camera;
    //camera.orthographic(-25, 25, -25, 25);
    //camera.lookAt(/*eye*/ { 0, 0, -5 }, /*target*/ { 0, 0, 0 } /*up*/);

    // render
    reyes::render(scene, camera);

    camera.image.writePPM("test.ppm");
}