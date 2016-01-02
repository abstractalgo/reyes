#pragma once

// core
#include "backend.hpp"
#include "mem.hpp"
#include "settings.hpp"
#include "worker.hpp"
// reyes
#include "scene.hpp"
#include "camera.hpp"

namespace reyes
{
    struct Renderer
    {
        static Renderer& getInstance(void)
        {
            static Renderer st_renderer;
            return st_renderer;
        }

        static void addTask(const Task& _task)
        {
            Renderer& _r = Renderer::getInstance();
            if (_r.oneThreadOnly)
            {
                _task.fn(_task.data);
            }
            else
            {
                _r.mTaskQueue.addJob(_task);
            }
        }

        static void render(DWORD threadCnt=0)
        {
            // init
            Renderer& _r = Renderer::getInstance();
            _r.oneThreadOnly = (threadCnt == 0);
            _r.workerSize = threadCnt;
            if (!_r.oneThreadOnly)
            {
                _r.workers = new Worker*[_r.workerSize];
                for (DWORD i = 0; i < _r.workerSize; ++i)
                {
                    _r.workers[i] = new Worker(i);
                }
            }

            // populate task queue with inital shapes ready to be diced
            // TODO

            // execute/flush
            Task _task = _r.mTaskQueue.getNext();
            while (_task.fn)
            {
                _task.fn(_task.data);
                _r.mTaskQueue.markJobDone(_task);
                _task = _r.mTaskQueue.getNext();
            }
            if (!_r.oneThreadOnly)
                _r.mTaskQueue.waitForAllDone();
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
            Task boundTask;
            boundTask.data = grid;
            boundTask.fn = &bound;

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

        /*Scene* scene;
        Camera* camera;

        Renderer(Scene* _scene, Camera* _camera)
            : scene(_scene)
            , camera(_camera)
        {}*/

        //mem::Pool<Microgrid, GIRDPOOL_SIZE>* grids;
        bool oneThreadOnly;
        TaskQueue mTaskQueue;
        Worker** workers;
        DWORD workerSize;

        Renderer()
            //, grids(new mem::Pool<Microgrid, GIRDPOOL_SIZE>)
        {
            Worker::mTaskQueue = &mTaskQueue;
        }

        ~Renderer()
        {
            if (!oneThreadOnly)
                for (DWORD i = 0; i < workerSize; ++i)
                    delete workers[i];
        }
    };
}