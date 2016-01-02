#include "backend.hpp"
#include "pipeline.hpp"
#include "settings.hpp"
#include "renderer.hpp"

#include "oglfilm.hpp"
// shapes
#include "plane.hpp"
#include "disc.hpp"
#include "sphere.hpp"
#include "bezier16.hpp"
#include "teapot.hpp"
// materials
#include "depthmat.hpp"
#include "lambertmat.hpp"
#include "normalmat.hpp"
#include "solidcolormat.hpp"
#include "uvmat.hpp"
#include "samplermat.hpp"
#include "displacementmat.hpp"
#include "litmat.hpp"
// samplers
#include "sincossampler.hpp"
#include "bmpsampler.hpp"

using namespace reyes;

#define MAKE_SHAPE(_name, _scene, _shp) _shp* _name = ::new(_scene.alloc(sizeof(_shp)).ptr) _shp

TaskQueue* Worker::mTaskQueue = 0;

#ifndef ANIMATE_BACKEND
void mainApp()
{
    reyes::Renderer::render(2);
    Renderer& r = reyes::Renderer::getInstance();
    /*
    // REYES
    srand(time(NULL));
    printf("REYES renderer v" REYES_VERSION "\n");

    // camera and scene
    Camera<OGLFilm, 480, 480> camera;
    Scene scene;

    // material for sphere
    lib::DisplacementMat sp_mat;
    lib::SinCosSampler sc_sampler;
    sp_mat.uniform.k = 0.03f;
    sp_mat.uniform.sampler = &sc_sampler;
    sp_mat.uniform.T = vec3(-0.5f, 0.5f, 0);
    sp_mat.uniform.S = vec3(0.3f, 0.3f, 0.3f);
    // material for plane
    lib::SamplerMat sq_mat;
    lib::BMPSampler lena_sampler(512, 512, "lena.bmp");
    sq_mat.uniform.sampler = &lena_sampler;
    sq_mat.uniform.T = vec3(-0.5f, -0.5f, 0);
    sq_mat.uniform.S = vec3(.8f, .8f, .8f);
    // material for teapot
    lib::NormalColor n_mat;
    n_mat.uniform.S = vec3(0.15f, 0.15f, 0.15f);
    n_mat.uniform.T = vec3(0.45f, 0.3f, 0);
    // material for disc
    lib::UVColor uv_mat;
    uv_mat.uniform.S = vec3(.4f, .4f, .4f);
    uv_mat.uniform.T = vec3(0.5f, -0.5f, 0);

    // sphere
    MAKE_SHAPE(sphere, scene, lib::Sphere);
    sphere->material = &sp_mat;
    // plane
    MAKE_SHAPE(square, scene, lib::Plane);
    square->material = &sq_mat;
    // teapot
    for (uint32_t i = 0; i < kTeapotNumPatches; i++)
    {
        vec3 control_points[16];
        for (uint32_t vi = 0; vi < 16; vi++)
        {
            control_points[vi].x = teapotVertices[teapotPatches[i][vi] - 1][0];
            control_points[vi].z = teapotVertices[teapotPatches[i][vi] - 1][1];
            control_points[vi].y = teapotVertices[teapotPatches[i][vi] - 1][2];
        }
        MAKE_SHAPE(patch, scene, lib::Bezier16) (control_points);
        patch->material = &n_mat;
    }
    // disc
    MAKE_SHAPE(disc, scene, lib::Disc);
    disc->material = &uv_mat;

    // render
    reyes::render(scene, camera);
    */
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

/*vec3 control_points[16];
control_points[0] = vec3(-.75f, .75f, 0);
control_points[1] = vec3(-.25f, .75f, 0);
control_points[2] = vec3(.25f, .75f, 0);
control_points[3] = vec3(.75f, .75f, 0);

control_points[4] = vec3(-.75f, 0.25f);
control_points[5] = vec3(-.25f, 0.25f, 1);
control_points[6] = vec3(.25f, 0.25f, 1);
control_points[7] = vec3(.75f, 0.25f, 0);

control_points[8] = vec3(-.75f, -.25f, 0);
control_points[9] = vec3(-.25f, -.25f, 1);
control_points[10] = vec3(.25f, -.25f, 1);
control_points[11] = vec3(.75f, -.25f, 0);

control_points[12] = vec3(-.75f, -.75f, 0);
control_points[13] = vec3(-.25f, -.75f, 0);
control_points[14] = vec3(.25f, -.75f, 0);
control_points[15] = vec3(.75f, -.75f, 0);
MAKE_SHAPE(patch, scene, lib::Bezier16<lib::NormalColor>) (control_points);
patch->transform.R.y = M_PI *0.25f;
patch->transform.T = vec3(0.5f, 0.5f, 0);
patch->transform.S = vec3(.5f, .5f, .5f);*/