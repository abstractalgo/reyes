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

- transformations (translate, rotate, scale, project)
- perspective camera
- dicing
- finalize quadrilaterals and triangles
- ~~G-buffer~~ (***z-index arc***)
- ~~bounding and splitting~~
- ~~improved memorymanagment~~
- ~~entire reyes pipeline~~
- geometry
    - plane (aka quad)
    - sphere
    - bezier patch (cubic)
        - newell teapot
- shading
    - ~~matlib~~
        - ~~lambert~~
        - ~~normal~~
        - ~~uv~~
        - ~~tex2d~~
    - resources
        - ~~texture~~ (***untested yet***)
        - func. eval. (displacement)
    - lights
        - ~~directional~~
        - ~~point~~
- normal-mapped, textured cube

####v3.0

- shadow mapping
- ~~more advanced rasterizer~~
- profiling
- even better memory managment
- dof
- motion blur
- SH light
- A-buffer
    - transparency support
    - inject optimizations
    - multisample buffer
- material
    - toon
    - phong

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