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
        HANDLE hRenderThread;
        uint8_t threadCnt;

        Renderer()
        //, grids(new mem::Pool<Microgrid, GIRDPOOL_SIZE>)
        {
            hRenderThread = CreateThread(0, 0, &renderProc, this, CREATE_SUSPENDED, 0);
        }

        void render(DWORD _threadCnt = 1)
        {
            threadCnt = _threadCnt - 1;

            // kickoff
            ResumeThread(hRenderThread);
        }

        ~Renderer()
        {
            SuspendThread(hRenderThread);
            CloseHandle(hRenderThread);
        }

        static DWORD WINAPI renderProc(void* pR)
        {
            Renderer* _renderer = (Renderer*)pR;

            // init other threads besides main rendering
            aajob::Init(_renderer->threadCnt);

            // add jobs
            // TODO

            // go!
            aajob::Flush();

            // cleanup
            aajob::Cleanup();

            // notify that render has ended
            // TODO

            return 0;
        }

        

    private:
        struct _InRenderData
        {
            Scene* scene;
            Shape* shape;
            Camera* camera;
        };
        static AAJOB_ENTRY_POINT(renderShape)
        {
            _InRenderData* pRD = static_cast<_InRenderData*>(jobDataPtr);
            Scene* scene = pRD->scene;
            Shape* shape = pRD->shape;
            Camera* camera = pRD->camera;

            Microgrid grid;

            shape->dice(grid);                                                  // DICE

            AABB2 bb = grid.aabb();                                             // BOUND
            if (!isInFrustum(bb))                                               // | try to cull
                goto memoryCleanup;                                             // |

            vec2 rasSz = camera->film->estimate(bb.max - bb.min);               // SPLIT - estimate grid's raster size
            if (requiresSplit(rasSz, RASTER_THRESHOLD))                         // |
            {                                                                   // |
                Shape* sptrs[4];                                                // |
                shape->split(*scene, sptrs);                                    // | do split
                for (uint8_t i = 0; i < 4; i++)
                {
                    _InRenderData& rd = *new _InRenderData;
                    rd.camera = camera;
                    rd.scene = scene;
                    rd.shape = sptrs[i];
                    aajob::JobDecl job(renderShape, &rd); //// SH*T!
                }
            }                                                                   // |
            else                                                                // | don't split, so continue to raster
            {
                shape->shade(grid);                                             // SHADE

                camera->film->rasterize(grid);                                  // SAMPLE
            }
        memoryCleanup:
            delete pRD;
        }
    };
}