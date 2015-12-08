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
    MAKE_SHAPE(scene, Quadrilateral<matlib::YellowColor>)();

    // camera setup
    Camera<RGB8Image, 50, 50> camera;
    //camera.orthographic(-25, 25, -25, 25);
    //camera.lookAt(/*eye*/ { 0, 0, -5 }, /*target*/ { 0, 0, 0 } /*up*/);

    // render
    reyes::render(scene, camera);

    camera.image.writePPM("test.ppm");
}