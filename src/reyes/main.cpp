#include <cstdio>
#include "core.h"
#include "vecmx.hpp"
#include "shape.hpp"
#include "camera.hpp"
#include "renderer.hpp"

using namespace reyes;

void appMain()
{
    // scene setup
    ShapeI scene[1];
    Rectangle r({ 0, 0, 0 }, { 1, 1 });
    scene[0] = r;

    // camera setup
    camera camera;
    camera.perspective(/*fov*/ 45, /*ncp*/ 0.0001, /*fcp*/ 1000);
    camera.lookAt(/*from*/ { 10, 10, 0 }, /*to*/ { 0, 0, 0 } /*up*/);

    // render
    REYES.render();
    
    printf("Hello world.\n");
}