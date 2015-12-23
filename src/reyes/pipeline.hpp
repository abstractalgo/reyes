#pragma once

#include "mem.hpp"
#include "backend.hpp"
#include "shape.hpp"
#include "grid.hpp"
#include "camera.hpp"

namespace reyes
{
    static const vec2 RASTER_THRESHOLD = { 700, 700};

    template<class FilmTy, uint16_t width, uint16_t height>
    void render(Scene& scene, Camera<FilmTy, width, height>& camera)
    {
        mem::ObjectStack<Microgrid> grids(1<<23);   // 8MB
        while (scene)
        {
            mem::blk shp_blk = scene.pop();
            ShapeI* surface = static_cast<ShapeI*>(shp_blk.ptr);
            
            surface->dice(&camera, grids);                                      // DICE

            AABB2 bb = surface->grid->aabb();                                   // BOUND
            if (bb.max.x <= -1.0f || bb.min.x >= 1.0f ||                        // | try to cull
                bb.min.y >= 1.0f || bb.max.y <= -1.0f)                          // |
                goto memoryCleanup;                                             // |

            vec2 rasSz = camera.film.estimate(bb.max - bb.min);                 // SPLIT - estimate grid's raster size
            SplitDir dir = split_dir(rasSz, RASTER_THRESHOLD);                  // | determine if to split and how
            if (SplitDir::NoSplit != dir)                                       // |
            {                                                                   // |
                surface->split(dir, scene);                                     // | do split
            }                                                                   // |
            else                                                                // | don't split, so continue to raster
            {
                surface->shade();                                               // SHADE

                camera.film.rasterize(*surface->grid);                          // SAMPLE
            }

        memoryCleanup:
            grids.pop();
            scene.free(shp_blk);
            printf("\rSHAPES: %d    ", scene.cnt);
        }
        //camera.film.display();
        SwapBuffersBackend();
    }

    /*struct Renderer
    {
        Scene* scene;
        CameraI* camera;
        HANDLE threadHandle;
        DWORD threadId;
        bool running;

        Renderer(Scene* _scene, CameraI* _camera)
            : scene(_scene)
            , camera(_camera)
            , running(false)
        {
            threadHandle = CreateThread(0, 0, this->render, this, CREATE_SUSPENDED, &threadId);
        }

        void go()
        {
            running = true;
        }

        void pause()
        {
            running = false;
        }

        static DWORD WINAPI render(void* a)
        {
            Renderer* r = static_cast<Renderer*>(a);
            
            return 0;
        }
    };*/
}