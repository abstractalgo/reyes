#pragma once

#include "vecmx.hpp"
#include "image.hpp"
#include "grid.hpp"

namespace reyes
{
    template<class FilmTy, uint16_t width, uint16_t height>
    /* Camera and its film. */
    struct Camera
    {
        mx4 view;
        mx4 projection;
        FilmTy image;

        Camera()
            : image(width, height)
        {}

        void capture(MicrogridI<PosColor>* grid)
        {
            // transform
            // TODO

            // project
            // TODO
            float aspect = width / height;

            /*for (uint16_t i = 0; i < 4; i++)
            {
                grid->data[i].p.x = (grid->data[i].p.x / width * 2.0f - 1.0f);
                grid->data[i].p.y = (grid->data[i].p.y / height * 2.0f - 1.0f);
            }*/

            // rasterize
            image.rasterize(*grid);
        }

        position project(position p)
        {
            return p;
        }

        /* Mark perspective frustum. */
        void perspective(float fov, float aspect, float near, float far)
        {
            float ymax = near * tan(((double)fov * 0.5)*M_PI/180.0);
            float ymin = -ymax;
            float xmin = ymin * aspect;
            float xmax = ymax * aspect;

            projection.frustum(xmin, xmax, ymin, ymax, near, far);
        }

        /* Make orthographics frustum. */
        void orthographic(float left, float right, float bottom, float top, float near=0.1f, float far=1000.0f)
        {
            float dx = (right - left) / 2;
            float dy = (top - bottom) / 2;
            float cx = (right + left) / 2;
            float cy = (top + bottom) / 2;

            projection.orthographic(cx - dx, cx + dx, cy + dy, cy - dy, near, far);
        }

        /* Orient camera to look at the target position, with selected up vector, from the selected origin. */
        void lookAt(const vec3 eye, vec3 target, const vec3 up = { 0, 1, 0 })
        {
            projection.lookAt(eye, target, up);
        }
    };
}