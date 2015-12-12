#include "backend.hpp"
#include "ogl_help.hpp"
#include "pipeline.hpp"
#include "shadinglib.hpp"

using namespace reyes;

#define REYES_VERSION "1.8"

#define MAKE_SHAPE(_scene, _shp) ::new(_scene.alloc(sizeof(_shp)).ptr) _shp

GLuint tex, program, quad;
Scene scene;
Camera<GBuffer, 640, 480> camera;

void InitApp()
{
    // REYES
    {
        printf("REYES renderer v"REYES_VERSION"\n");

        // scene setup
        MAKE_SHAPE(scene, Quadrilateral<shading::UVColor>) ({ -1, 1, 0.5 }, { 1, 1, 0.5 }, { 2, -2, 0.5 }, { -2, -1, 0.5 });;
        MAKE_SHAPE(scene, Quadrilateral<shading::NormalColor>) ({ 0, 2, 0 }, { 1, 2, 1 }, { 2, 0, 1 }, { -1, -1, 0 });

        // camera setup
        //camera.orthographic(-25, 25, -25, 25);
        //camera.lookAt(/*eye*/ { 0, 0, -5 }, /*target*/ { 0, 0, 0 } /*up*/);

        // render
        reyes::render(scene, camera);
        //camera.image.writePPM("test.ppm");
    }

    // OPENGL
    {
        glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        opengl_init(tex, program, quad, 640, 480, camera.image.getRGB());
    }
}

void RenderApp()
{
    //PerfMarker("Frame
    opengl_display(tex, 640, 480, camera.image.getRGB());
}

void CleanupApp()
{
    opengl_cleanup(tex, program, quad);
}