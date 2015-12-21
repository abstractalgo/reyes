#include "backend.hpp"
#include "ogl_help.hpp"
#include "pipeline.hpp"
#include "shadinglib.hpp"
#include "rect.hpp"
#include "disc.hpp"
#include "sphere.hpp"
#include "klein.hpp"

using namespace reyes;

#define MAKE_SHAPE(_scene, _shp) ::new(_scene.alloc(sizeof(_shp)).ptr) _shp

Scene scene;
Camera<GBuffer, 480, 480> camera;

void mainApp()
{
    // REYES
    srand(time(NULL));
    
    printf("REYES renderer v1.82\n");

    // scene setup
    //MAKE_SHAPE(scene, reyes::Rectangle<shading::NormalColor>) ({ -1, 1, 0.5 }, { 1, 1, 0.5 }, { 2, -2, 0.5 }, { -2, -1, 0.5 });;
    //MAKE_SHAPE(scene, reyes::Rectangle<shading::NormalColor>) ({ 0, 2, 1 }, { 1, 2, 1 }, { 2, 0, 0 }, { -1, -1, 0 });
    //MAKE_SHAPE(scene, reyes::Sphere<shading::NormalColor>) ({ 0, -0.5f, 0 }, 1);
    MAKE_SHAPE(scene, reyes::Klein<shading::UVColor>);

    // camera setup
    //camera.orthographic(-25, 25, -25, 25);
    //camera.lookAt(/*eye*/ { 0, 0, -5 }, /*target*/ { 0, 0, 0 } /*up*/);

    // render
    opengl_init(camera.image.width, camera.image.height, 0);
    reyes::render(scene, camera);
    //camera.image.writePPM("test.ppm");
    opengl_cleanup();


    /*s->dice(0);
    s->renderOGL();
    SwapBuffersBackend();*/
}

void InitApp()
{
    // init
}

void RenderApp()
{
    PerfMarker("Frame");
    // update & display
}

void CleanupApp()
{
    // cleanup
}