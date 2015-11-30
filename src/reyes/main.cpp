#include <cstdio>
#include "pipeline.hpp"
#include "grid.hpp"
#include "matlib.hpp"

using namespace reyes;

void appMain()
{
    // scene setup
    mem::ObjectStack<ShapeI> scene(1024);
    ShapeI& rectangle = *new(scene.alloc(sizeof(Rectangle<usemat(matlib::Lambert)>))) Rectangle<usemat(matlib::Lambert)>({ 0, 0, 0 }, { 5, 5 });

    // camera setup
    camera camera;
    camera.orthographic(-10, 10, 10, 10);
    camera.lookAt(/*eye*/ { 0, 0, -5 }, /*target*/ { 0, 0, 0 } /*up*/);

    // image setup
    GBuffer image(10,10);

    // render
    reyes::render(scene, camera, image);
}