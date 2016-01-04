#pragma once

// core
#include "backend.hpp"
#include "mem.hpp"
#include "settings.hpp"
#include "aajob.hpp"
// reyes
#include "scene.hpp"
#include "camera.hpp"

namespace reyes
{
    struct Renderer
    {
        /*Scene* scene;
        Camera* camera;

        Renderer(Scene* _scene, Camera* _camera)
        : scene(_scene)
        , camera(_camera)
        {}*/

        //mem::Pool<Microgrid, GIRDPOOL_SIZE>* grids;

        Renderer()
        //, grids(new mem::Pool<Microgrid, GIRDPOOL_SIZE>)
        {
        }

        ~Renderer()
        {
        }

        void render()
        {

        }

    private:
        static void dice(void* data)
        {
            // get shape
            // allocate memory
            // dice
            // create bound task
            // dispatch
            Shape* _shape = static_cast<Shape*>(data);
            Microgrid* grid = new Microgrid;
            _shape->dice(*grid);

        }

        static void bound(void* data)
        {
            // get grid
            // get shape
            // calc bounds
            // compare to threshold
            // either create 4x split taks, or send to shade
            Microgrid* _shape = static_cast<Microgrid*>(data);
        }

        static void shade(void* data)
        {
            // get grid
            // shade
            // create sample task
        }

        static void sample(void* data)
        {
            // rasterize
        }
    };
}