#include "backend.hpp"
#include "pipeline.hpp"

#include "oglfilm.hpp"
// shapes
#include "plane.hpp"
#include "disc.hpp"
#include "sphere.hpp"
#include "klein.hpp"
// materials
#include "depthmat.hpp"
#include "lambertmat.hpp"
#include "normalmat.hpp"
#include "solidcolormat.hpp"
#include "uvmat.hpp"
#include "samplermat.hpp"
#include "sincosshader.hpp"
#include "displacementmat.hpp"
#include "texture2d.hpp"

using namespace reyes;

#define MAKE_SHAPE(_name, _scene, _shp) _shp* _name = ::new(_scene.alloc(sizeof(_shp)).ptr) _shp

#ifndef ANIMATE_BACKEND
void mainApp()
{
    // REYES
    srand(time(NULL));
    printf("REYES renderer v1.9\n");

    // scene setup
    Scene scene;
    MAKE_SHAPE(ss, scene, reyes::lib::Sphere<lib::DisplacementMat>) ({ 0, 0.5f, 0 }, 0.3);
    ss->material.uniform.sampler = new lib::SinCosShader;
    MAKE_SHAPE(a, scene, reyes::lib::Plane<lib::SamplerMat>) ({ -0.5f, -0.5f, 0 }, { 1,1 });
    a->material.uniform.sampler = new lib::Texture2D(512, 512);
    static_cast<lib::Texture2D*>(a->material.uniform.sampler)->loadBMP("lena.bmp");

    // camera setup
    Camera<OGLFilm, 480, 480> camera;
    //camera.orthographic(-25, 25, -25, 25);
    //camera.lookAt(/*eye*/ { 0, 0, -5 }, /*target*/ { 0, 0, 0 } /*up*/);

    // render
    reyes::render(scene, camera);
    //camera.image.writePPM("test.ppm");
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