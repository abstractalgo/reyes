#pragma once

#include "mem.hpp"
#include "backend.hpp"
#include "parametric.hpp"
#include "grid.hpp"
#include "camera.hpp"
#include "image.hpp"

namespace reyes
{
    static const vec2 RASTER_THRESHOLD = { 8, 8 };

    template<class FilmTy, uint16_t width, uint16_t height>
    void render(Scene& scene, Camera<FilmTy, width, height>& camera)
    {
        while (scene)
        {
            mem::blk shp_blk = scene.pop();
            SurfaceI* surface = static_cast<SurfaceI*>(shp_blk.ptr);
            
            surface->dice(&camera);                                             // DICE

            AABB2 bb = surface->grid->aabb();                                   // BOUND
            if (bb.max.x <= -1.0f || bb.min.x >= 1.0f ||                        // | try to cull
                bb.min.y >= 1.0f || bb.max.y <= -1.0f)                          // |
                goto memoryCleanup;                                             // |

            vec2 rasSz = camera.image.estimate(bb.max - bb.min);                // SPLIT - estimate grid's raster size
            SplitDir dir = split_dir(rasSz, RASTER_THRESHOLD);                  // | determine if to split and how
            if (SplitDir::NoSplit != dir)                                       // |
            {                                                                   // |
                surface->split(dir, scene);                                     // | do split
            }                                                                   // |
            else                                                                // | don't split, so continue to raster
            {
                surface->shade();                                               // SHADE

                camera.capture(surface->grid);                                  // SAMPLE
            }

        memoryCleanup:
            delete surface->grid;
            scene.free(shp_blk);
        }
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