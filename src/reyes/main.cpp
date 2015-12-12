#include "pipeline.hpp"
#include "shadinglib.hpp"

using namespace reyes;

#define MAKE_SHAPE(_scene, _shp) ::new(_scene.alloc(sizeof(_shp)).ptr) _shp

int main()
{
    // scene setup
    Scene scene;
    MAKE_SHAPE(scene, Quadrilateral<shading::UVColor>) ({ -1, 1, 0.5 }, { 1, 1, 0.5 }, { 2, -2, 0.5 }, { -2, -1, 0.5 });;
    MAKE_SHAPE(scene, Quadrilateral<shading::NormalColor>) ({ 0, 2, 0 }, { 1, 2, 1 }, { 2, 0, 1 }, { -1, -1, 0 });

    // camera setup
    Camera<GBuffer, 640, 480> camera;
    //camera.orthographic(-25, 25, -25, 25);
    //camera.lookAt(/*eye*/ { 0, 0, -5 }, /*target*/ { 0, 0, 0 } /*up*/);

    // render
    reyes::render(scene, camera);

    camera.image.writePPM("test.ppm");

    return 0;
}

//void h_MouseUp(int btn)
//{
//}
//
//void InitApp()
//{
//    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
//}
//
//void RenderApp()
//{
//    PerfMarker("Frame");
//    glClear(GL_COLOR_BUFFER_BIT);
//}
//
//void CleanupApp()
//{
//
//}