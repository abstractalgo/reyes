#include <cstdio>
#include "pipeline.hpp"

using namespace reyes;

void appMain()
{
    // scene setup
    Shape scene[1];
    Rectangle r({ 0, 0, 0 }, { 1, 1 });
    scene[0] = r;

    // camera setup
    camera camera;
    camera.orthographic(-10, 10, 10, 10);
    camera.lookAt(/*eye*/ { 0, 0, -5 }, /*target*/ { 0, 0, 0 } /*up*/);

    // image setup
    GBuffer image(10,10);

    // render 
    reyes::render(scene, camera, image);
}