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

            // startuj glavnu nit
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

            // dodaj job-ove
            int vals[10000];
            for (int i = 0; i < 10000; i++)
            {
                vals[i] = i;
                aajob::JobDecl job(testJob, vals + i);
                aajob::RunJob(job);
            }

            // go!
            aajob::Flush();

            // cleanup
            aajob::Cleanup();

            // notify that render has ended
            _renderer->endOfRender();

            return 0;
        }

        void endOfRender()
        {
            SuspendThread(hRenderThread);
        }

        static AAJOB_ENTRY_POINT(testJob)
        {
            int* pN = static_cast<int*>(jobDataPtr);
            DWORD tid = GetCurrentThreadId();
            printf("%d [%d]\n", *pN, tid);

            int j = 0;
            for (int i = 0; i < 100000; i++)
            {
                j += i;
            }
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