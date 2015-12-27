#pragma once

#include "mem.hpp"
#include "backend.hpp"
#include "shape.hpp"
#include "grid.hpp"
#include "camera.hpp"
#include "settings.hpp"

namespace reyes
{
    template<class FilmTy, uint16_t width, uint16_t height>
    void render(Scene& scene, Camera<FilmTy, width, height>& camera)
    {
        glUseProgram(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        //glEnable(GL_CULL_FACE); // do culling
        while (scene)
        {
            mem::blk shp_blk = scene.pop();
            Shape* shape = static_cast<Shape*>(shp_blk.ptr);
            
            shape->dice();                                                      // DICE

            AABB2 bb = shape->grid.aabb();                                      // BOUND
            if (bb.max.x <= -1.0f || bb.min.x >= 1.0f ||                        // | try to cull
                bb.min.y >= 1.0f || bb.max.y <= -1.0f)                          // |
                goto memoryCleanup;                                             // |

            vec2 rasSz = camera.film.estimate(bb.max - bb.min);                 // SPLIT - estimate grid's raster size
            if (requiresSplit(rasSz, RASTER_THRESHOLD))                         // |
            {                                                                   // |
                shape->split(scene);                                            // | do split
            }                                                                   // |
            else                                                                // | don't split, so continue to raster
            {
                shape->shade();                                                 // SHADE

                camera.film.rasterize(shape->grid);                             // SAMPLE
            }

        memoryCleanup:
            scene.free(shp_blk);
            printf("\rSHAPES: %d    ", scene.cnt);
        }
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