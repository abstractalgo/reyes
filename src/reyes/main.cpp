#include <cstdio>
#include "pipeline.hpp"
#include "matlib.hpp"

#define MAKE_SHAPE(_scene, _shp) ::new(_scene.alloc(sizeof(_shp)).ptr) _shp

using namespace reyes;

void appMain()
{
    // scene setup
    Scene scene;
    MAKE_SHAPE(scene, Quadrilateral<matlib::UVColor>) ({ -1, 1, 0.5 }, { 1, 1, 0.5 }, { 2, -2, 0.5 }, { -2, -1, 0.5 });;
    MAKE_SHAPE(scene, Quadrilateral<matlib::UVColor>) ({ 0, 2, 1 }, { 1, 2, 1 }, { 2, 0, 1 }, { -1, -1, 0 });

    // camera setup
    Camera<GBuffer, 240, 240> camera;
    //camera.orthographic(-25, 25, -25, 25);
    //camera.lookAt(/*eye*/ { 0, 0, -5 }, /*target*/ { 0, 0, 0 } /*up*/);

    // render
    reyes::render(scene, camera);

    camera.image.writePPM("test.ppm");
}