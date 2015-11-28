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
    camera.perspective(/*fov*/ 45.0f, /*ncp*/ 0.0001f, /*fcp*/ 1000.0f);
    camera.lookAt(/*from*/ { 10, 10, 0 }, /*to*/ { 0, 0, 0 } /*up*/);

    // image setup
    GBuffer image(10,10);

    // render 
    reyes::render<GBuffer>(scene, camera, image);
    
    printf("Hello world.\n");
}