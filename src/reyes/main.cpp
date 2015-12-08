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
    camera camera;
    camera.orthographic(-10, 10, 10, 10);
    camera.lookAt(/*eye*/ { 0, 0, -5 }, /*target*/ { 0, 0, 0 } /*up*/);

    // image setup
    RGB8Image image(50, 50);

    // render
    reyes::render(scene, camera, image);

    image.writePPM("test.ppm");
}