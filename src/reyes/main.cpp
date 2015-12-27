#include "backend.hpp"
#include "pipeline.hpp"

#include "oglfilm.hpp"
// shapes
#include "plane.hpp"
#include "disc.hpp"
#include "sphere.hpp"
#include "klein.hpp"
#include "bezier16.hpp"
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
#include "teapot.hpp"

using namespace reyes;

#define MAKE_SHAPE(_name, _scene, _shp) _shp* _name = ::new(_scene.alloc(sizeof(_shp)).ptr) _shp

#ifndef ANIMATE_BACKEND
void mainApp()
{
    // REYES
    srand(time(NULL));
    printf("REYES renderer v1.95\n");

    // scene setup
    Scene scene;

    // sphere
    MAKE_SHAPE(sphere, scene, lib::Sphere);
    lib::DisplacementMat sp_mat;
    lib::SinCosSampler sc_sampler;
    sp_mat.uniform.sampler = &sc_sampler;
    sp_mat.uniform.k = 0.1f;
    sphere->material = &sp_mat;
    //sphere->transform.T = vec3(-0.5f, 0.5f, 0);
    //sphere->transform.S = vec3(0.3f, 0.3f, 0.3f);

    // textured plane
    //MAKE_SHAPE(square, scene, lib::Plane);
    //lib::SamplerMat sq_mat;
    //lib::BMPSampler lena_sampler(512, 512, "lena.bmp");
    //sq_mat.uniform.sampler = &lena_sampler;
    //square->material = &sq_mat;
    //square->transform.T = vec3(-0.5f, -0.5f, 0);
    //square->transform.S = vec3(.8f, .8f, .8f);
    //square->transform.R.z = -0.2;

    // textured teapot
    //lib::Lambert teapot_mat;
    //lib::BMPSampler stone_sampler(512, 512, "stone_albedo.bmp");
    //lib::DirectionalLight dir_lig({ 1, 1, 1, 1 }, { 1, 1, 0.5 });
    //teapot_mat.uniform.texture = &stone_sampler;
    //teapot_mat.uniform.light = &dir_lig;
    //for (uint32_t i = 0; i < kTeapotNumPatches; i++)
    //{
    //    vec3 control_points[16];
    //    for (uint32_t vi = 0; vi < 16; vi++)
    //    {
    //        control_points[vi].x = teapotVertices[teapotPatches[i][vi] - 1][0] *0.3f;
    //        control_points[vi].z = teapotVertices[teapotPatches[i][vi] - 1][1] * 0.3f;
    //        control_points[vi].y = teapotVertices[teapotPatches[i][vi] - 1][2] * 0.3f;
    //    }
    //    MAKE_SHAPE(patch, scene, lib::Bezier16) (control_points);
    //    patch->material = &teapot_mat;
    //    //patch->transform.S = vec3(0.15f, 0.15f, 0.15f);
    //    //patch->transform.T = vec3(0.45f, 0.3f, 0);
    //    //patch->transform.R.x = M_PI * 0.3f;
    //}

    // uv disc
    //MAKE_SHAPE(disc, scene, lib::Disc);
    //lib::UVColor uv_mat;
    //disc->material = &uv_mat;
    //disc->transform.S = vec3(.4f, .4f, .4f);
    //disc->transform.T = vec3(0.5f, -0.5f, 0);

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

    // box
    {
        /*MAKE_SHAPE(w1, scene, lib::Plane<lib::LitMat>);
        w1->material.uniform.albedoMap  = new lib::BMPSampler(512, 512, "stone_albedo.bmp");
        w1->material.uniform.normalMap  = new lib::BMPSampler(512, 512, "stone_normal.bmp");
        w1->material.uniform.light      = new lib::DirectionalLight({ 1, 1, 1, 1 }, { 0, 1, 0 });
        w1->transform.T = vec3(M_SQRT2 / 4.0f, 0, M_SQRT2 / 4.0f);
        w1->transform.R.y = -M_PI / 4.0f;
        w1->material.uniform.rot = w1->transform.R;

        MAKE_SHAPE(w2, scene, lib::Plane<lib::LitMat>);
        w2->material.uniform.albedoMap  = w1->material.uniform.albedoMap;
        w2->material.uniform.normalMap  = w1->material.uniform.normalMap;
        w2->material.uniform.light      = w1->material.uniform.light;
        w2->transform.T = vec3(-M_SQRT2 / 4.0f, 0, M_SQRT2 / 4.0f);
        w2->transform.R.y = M_PI / 4.0f;
        w2->material.uniform.rot = w2->transform.R;*/
    }

    // camera setup
    Camera<OGLFilm, 480, 480> camera;
    //camera.perspective(45, 1, 0.1, 100);
    //camera.lookAt(/*eye*/ { 0, 5, 0 }, /*target*/ { 0, 0, 0 } /*up*/);

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