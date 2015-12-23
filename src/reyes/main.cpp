#include "backend.hpp"
#include "oglfilm.hpp"
#include "pipeline.hpp"
#include "shadinglib.hpp"
#include "rect.hpp"
#include "disc.hpp"
#include "sphere.hpp"
#include "klein.hpp"

using namespace reyes;

#define MAKE_SHAPE(_name, _scene, _shp) _shp* _name = (::new(_scene.alloc(sizeof(_shp)).ptr) _shp)

#ifndef ANIMATE_BACKEND
void mainApp()
{
    // REYES
    srand(time(NULL));
    
    printf("REYES renderer v1.82\n");
    Scene scene;
    Camera<OGLFilm, 480, 480> camera;
    // scene setup
    //MAKE_SHAPE(scene, reyes::Rectangle<shading::NormalColor>) ({ -1, 1, 0.5 }, { 1, 1, 0.5 }, { 2, -2, 0.5 }, { -2, -1, 0.5 });;
    //MAKE_SHAPE(scene, reyes::Rectangle<shading::NormalColor>) ({ 0, 2, 1 }, { 1, 2, 1 }, { 2, 0, 0 }, { -1, -1, 0 });
    MAKE_SHAPE(s, scene, reyes::Sphere<shading::NormalColor>);// ({ 0, 0, 0 }, 0.3);
    //Surface<shading::NormalColor>* s = MAKE_SHAPE(scene, reyes::Sphere<shading::NormalColor>) ({ 0, 0, 0 }, 1.0f);

    // camera setup
    //camera.orthographic(-25, 25, -25, 25);
    //camera.lookAt(/*eye*/ { 0, 0, -5 }, /*target*/ { 0, 0, 0 } /*up*/);

    // render
    //reyes::render(scene, camera);
    //camera.image.writePPM("test.ppm");
    

    mem::ObjectStack<Microgrid> grids(1 << 23);   // 8MB
    s->dice(0,grids);
    s->renderOGL();
    SwapBuffersBackend();
    mem::blk b = grids.pop();
    scene.free(scene.pop());
}
#else
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
#endif