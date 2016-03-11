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

reyes::Renderer ryRenderer;

#define MAKE_SHAPE(_scene, _shp, _name) _shp* _name = ::new(_scene.alloc(sizeof(_shp)).ptr) _shp
#define MAKE_MSHAPE(_scene, _shptype, _shpname, _shpmat) _shptype* _shpname = ::new(_scene.alloc(sizeof(_shptype)).ptr) _shptype; _shpname->material = &_shpmat;

void keyUpHandler(unsigned int key)
{
    if (key == INPUT_KEY_A)
    {
        RASTER_THRESHOLD.x /= 2;
        RASTER_THRESHOLD.y /= 2;
    }
    else if (key == INPUT_KEY_S)
    {
        RASTER_THRESHOLD.x *= 2;
        RASTER_THRESHOLD.y *= 2;
    }
    else if (key == INPUT_KEY_D)
    {
        mainApp();
    }
}

#ifndef ANIMATE_BACKEND
void mainApp()
{
    // mt test
    {
        // mt version
        //ryRenderer.render(3);
        // no-mt version
        //for (int i = 0; i < 10000; i++) reyes::Renderer::testJob(&i);
    }

    // REYES
    srand(time(NULL));
    printf("REYES renderer v" REYES_VERSION "(%.1f %.1f)\n", RASTER_THRESHOLD.x, RASTER_THRESHOLD.y);
    INPUT_f_keyUp(keyUpHandler);

    // camera and scene
    Scene scene;
    Camera camera;
    OGLFilm film(800, 800);
    camera.film = &film;
    // sphere
    lib::DisplacementMat sp_mat;
    lib::SinCosSampler sc_sampler;
    sp_mat.uniform.k = 0.03f;
    sp_mat.uniform.sampler = &sc_sampler;
    sp_mat.uniform.T = vec3(-0.5f, 0.5f, 0);
    sp_mat.uniform.S = vec3(0.3f, 0.3f, 0.3f);
    MAKE_MSHAPE(scene, lib::Sphere, sphere, sp_mat);
    // plane
    lib::SamplerMat sq_mat;
    lib::BMPSampler lena_sampler(512, 512, "lena.bmp");
    sq_mat.uniform.sampler = &lena_sampler;
    sq_mat.uniform.T = vec3(-0.5f, -0.5f, 0);
    sq_mat.uniform.S = vec3(.8f, .8f, .8f);
    MAKE_MSHAPE(scene, lib::Plane, square, sq_mat);
    // disc
    lib::UVColor uv_mat;
    uv_mat.uniform.S = vec3(.4f, .4f, .4f);
    uv_mat.uniform.T = vec3(0.5f, -0.5f, 0);
    MAKE_MSHAPE(scene, lib::Disc, disc, uv_mat);
    // teapot
    lib::NormalColor n_mat;
    n_mat.uniform.S = vec3(0.15f, 0.15f, 0.15f);
    n_mat.uniform.T = vec3(0.45f, 0.3f, 0);
    for (uint32_t i = 0; i < kTeapotNumPatches; i++)
    {
        vec3 control_points[16];
        for (uint32_t vi = 0; vi < 16; vi++)
        {
            control_points[vi].x = teapotVertices[teapotPatches[i][vi] - 1][0];
            control_points[vi].z = teapotVertices[teapotPatches[i][vi] - 1][1];
            control_points[vi].y = teapotVertices[teapotPatches[i][vi] - 1][2];
        }
        MAKE_SHAPE(scene, lib::Bezier16, patch) (control_points);
        patch->material = &n_mat;
    }

    // render
    glUseProgram(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    reyes::render(scene, camera);

    SwapBuffersBackend();
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