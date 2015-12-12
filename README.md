# reyes
reyes renderer

current version v1.8

###TODO

####v1.0

- ~~basic buffer (1 sample per pixel)~~
- ~~no bucketing~~
- ~~orthographic camera~~
- ~~no splitting and bounding (dice entire primitives, each and every)~~
- ~~basic pipeline~~
- ~~basic rasterizer~~

![First render](log/reyes_first.png "First image")

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
- normal-mapped, textured cube (***!challenge!***)

####v3.0

- shadow mapping
- ~~more advanced rasterizer~~
- SH light
- perf
    - profiling
    - progress indicator
    - progressive (~~rendering in a window~~)
- even better memory managment
- dof
- motion blur
- A-buffer
    - transparency support
    - inject optimizations
    - multisample buffer
- material
    - toon
    - phong
- extra
    - additional post-processing effects

####v4.0

- fix cracks
- fix elongated micropolygons
- additional geometry
    - catmull-clark subdiv
    - more bezier
    - nurbs (b-spline)
    - loop subdiv

####v5.0 

- bucketing
- task (job) manager

####v6.0

- (GP)GPU implementation
- job affinity selection
- (pseudo) real-time

####v7.0

- animation
- ~~shading language~~ (***needs to be more flexible***)

####v113A

- full Renderman specification :D