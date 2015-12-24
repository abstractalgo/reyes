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

#include "displacementmat.hpp"
// samplers
#include "sincossampler.hpp"
#include "bmpsampler.hpp"

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

    MAKE_SHAPE(sphere, scene, reyes::lib::Sphere<lib::NormalColor>);
    //sphere->material.uniform.sampler = new lib::SinCosShader;
    sphere->transform.T = vec3(0, 0.5f, 0);
    sphere->transform.S = vec3(0.3f, 0.3f, 0.3f);
    //sphere->transform.R.y = M_PI / 4.0f;

    MAKE_SHAPE(square, scene, reyes::lib::Plane<lib::NormalColor>);
    //square->material.uniform.sampler = new lib::BMPSampler(512, 512, "lena.bmp");
    square->transform.T = vec3(-0.5f, -0.5f, 0);
    square->transform.R.x = 0.3;

    MAKE_SHAPE(disc, scene, reyes::lib::Disc<lib::UVColor>);
    disc->transform.S = vec3(.5f, .5f, .5f);
    disc->transform.T = vec3( 0.5f, -0.5f, 0 );
    disc->transform.R.x = M_PI / 4.0f;
    disc->transform.R.y = M_PI / 4.0f;

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