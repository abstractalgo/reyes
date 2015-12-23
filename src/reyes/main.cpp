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

using namespace reyes;

#define MAKE_SHAPE(_name, _scene, _shp) _shp* _name = ::new(_scene.alloc(sizeof(_shp)).ptr) _shp

#ifndef ANIMATE_BACKEND
void mainApp()
{
    // REYES
    srand(time(NULL));
    
    printf("REYES renderer v1.85\n");
    Scene scene;
    Camera<OGLFilm, 480, 480> camera;
    // scene setup
    //MAKE_SHAPE(scene, reyes::Rectangle<shading::NormalColor>) ({ -1, 1, 0.5 }, { 1, 1, 0.5 }, { 2, -2, 0.5 }, { -2, -1, 0.5 });;
    //MAKE_SHAPE(scene, reyes::Rectangle<shading::NormalColor>) ({ 0, 2, 1 }, { 1, 2, 1 }, { 2, 0, 0 }, { -1, -1, 0 });
    MAKE_SHAPE(ss, scene, reyes::lib::Sphere<lib::DisplacementMat>) ({ 0, 0, 0 }, 0.9);
    ss->material.uniform.sampler = new lib::SinCosShader;

    // camera setup
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