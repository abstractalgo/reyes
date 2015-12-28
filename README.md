# reyes
reyes renderer

![cover](log/cover.png "cover")

current version v2.05

###TODO

####v1.0 rock

- ~~basic buffer (1 sample per pixel)~~
- ~~no bucketing~~
- ~~no splitting and bounding (dice entire primitives, each and every)~~
- ~~basic pipeline~~
- ~~basic rasterizer~~

####v2.0 dragonfly

- ~~transformations (translate, scale)~~
- ~~dicing~~
- ~~bounding and splitting~~
- ~~entire reyes pipeline~~
- ~~finalize quadrilaterals and triangles~~
- ~~G-buffer~~
- ~~improved memory management and speed~~
- ~~geometry: plane, sphere, disc, cubic bezier patch, Utah teapot~~
- ~~materials~~
- ~~samplers~~
- ~~lights: directional, point~~

####v3.0 nautilus

- threaded renderer
- profiling
- ~~even better memory managment~~
- A-buffer
    - stochastic (multi)sampling
    - transparency support
    - inject optimizations
- dof
- motion blur
- bucketing
- task (job) manager
- (more advanced rasterizer)
- (progress indicator)
- (camera; perspective and orthographic)
- (Klein bottle)
- (SH light)

####v4.0 cheetah

- optimized memory layout
- (GP)GPU implementation
- (pseudo) real-time

####v5.0 alien

- fix cracks (no need?)
- fix elongated micropolygons
- dice criterium different
- dynamic dicing

####v6.0 bambi (totally irrelevant)

- shadow mapping
- extra
    - additional post-processing effects
- animation
- materials (toon, phong, ggxD)
- additional geometry
    - catmull-clark subdiv
    - more bezier
    - nurbs (b-spline)
    - loop subdiv
- ~~shading language~~ (***needs to be more flexible***)

####v113A

- full Renderman specification :D

####challenge

- normal-mapped, textured cube