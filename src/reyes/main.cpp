#include "backend.hpp"
#include "pipeline.hpp"
#include "shadinglib.hpp"

using namespace reyes;

#define REYES_VERSION "1.8"

#define MAKE_SHAPE(_scene, _shp) ::new(_scene.alloc(sizeof(_shp)).ptr) _shp

void InitApp()
{
    // OPENGL
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RES_BACKEND, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    // REYES
    printf("REYES renderer v"REYES_VERSION"\n");

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

    // OPENGL
    glDeleteTextures(1, &tex);
}

void RenderApp()
{
    PerfMarker("Frame");
    glClear(GL_COLOR_BUFFER_BIT);
}

void CleanupApp()
{

}