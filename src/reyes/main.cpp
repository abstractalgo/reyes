#include "backend.hpp"
#include "ogl_help.hpp"
#include "pipeline.hpp"
#include "shadinglib.hpp"
#include "rect.hpp"
#include "disc.hpp"
#include "sphere.hpp"

using namespace reyes;

#define MAKE_SHAPE(_scene, _shp) ::new(_scene.alloc(sizeof(_shp)).ptr) _shp

GLuint tex, program, quad;
Scene scene;
Camera<GBuffer, 640, 480> camera;

void mainApp()
{
    // REYES
    {
        printf("REYES renderer v1.82\n");

        // scene setup
        //MAKE_SHAPE(scene, Quadrilateral<shading::UVColor>) ({ -1, 1, 0.5 }, { 1, 1, 0.5 }, { 2, -2, 0.5 }, { -2, -1, 0.5 });;
        //MAKE_SHAPE(scene, Quadrilateral<shading::NormalColor>) ({ 0, 2, 0 }, { 1, 2, 1 }, { 2, 0, 1 }, { -1, -1, 0 });
        MAKE_SHAPE(scene, reyes::Sphere<shading::UVColor>);

        // camera setup
        //camera.orthographic(-25, 25, -25, 25);
        //camera.lookAt(/*eye*/ { 0, 0, -5 }, /*target*/ { 0, 0, 0 } /*up*/);

        // render
        reyes::render(scene, camera);
        //camera.image.writePPM("test.ppm");
    }

    // OPENGL
    {
        opengl_init(tex, program, quad, camera.image.width, camera.image.height, camera.image.getRGB());
        opengl_display(tex, camera.image.width, camera.image.height, camera.image.getRGB());
        opengl_cleanup(tex, program, quad);
    }
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