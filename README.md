# reyes
reyes renderer

current version v1.0

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
- ~~matlib (lambert, normal, uv)~~
    - ~~textured material~~ (***untested yet***)
- perspective camera
- bounding and ~~splitting~~
- dicing
- finalize quadrilaterals and triangles

####v3.0

- multiple samples per pixel (antialiasing)
- more advanced rasterizer
- profiling
- A-buffer (transparency support + optimizations)

####v4.0

- post-processing
- rendering in window

####v5.0

- bucketing
- ~~additional primitives intro (triangles and polygon)~~
- additional shapes (catmull-clark subdiv, bezier, nurbs, b-spline, loop subdiv)

####v6.0

- fix cracks
- task (job) manager

####v7.0

- (GP)GPU implementation
- job affinity selection

####v7.0

- ~~shading language~~ (***needs to be more flexible***)

####v113A

- full Renderman specification :D