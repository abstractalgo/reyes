# reyes
reyes renderer

current version v1.7

###TODO

####v1.0

- ~~basic buffer (1 sample per pixel)~~
- ~~no bucketing~~
- ~~orthographic camera~~
- ~~no splitting and bounding (dice entire primitives, each and every)~~
- ~~basic pipeline~~
- ~~basic rasterizer~~

####v2.0

- ~~G-buffer~~ (***z-index arc***)
- transformations (translate, rotate, scale, project)
- perspective camera
- ~~matlib~~
    - ~~lambert~~
    - ~~normal~~
    - ~~uv~~
    - sampling material (and for displ.)
        - textured
        - func. eval.
    - toon
    - phong
- ~~bounding and splitting~~
- dicing
- ~~improved memorymanagment~~
- ~~entire reyes pipeline~~
- finalize quadrilaterals and triangles
- geometry
    - plane (aka quad)
    - sphere
    - bezier patch (degree 3)
        - newell teapot
- lights
    - directional
    - point

####v3.0

- shadow mapping
- multiple samples per pixel (antialiasing)
- more advanced rasterizer
- profiling
- even better memory managment
- dof
- motion blur
- A-buffer (transparency support + optimizations)

####v4.0

- post-processing
- rendering in window

####v5.0

- animation
- bucketing
- additional geoemtry
    - catmull-clark subdiv
    - more bezier
    - nurbs (b-spline)
    - loop subdiv

####v6.0

- fix cracks
- fix elongated micropolygons
- task (job) manager

####v7.0

- (GP)GPU implementation
- job affinity selection
- (pseudo) real-time

####v8.0

- ~~shading language~~ (***needs to be more flexible***)

####v113A

- full Renderman specification :D