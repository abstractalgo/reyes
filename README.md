# reyes
REYES renderer in C++

![cover2](log/cover2.png "cover2")

### file structure

Source code is divided as following:

![ls](log/ls.png "ls")

- **core/**: responsible for systems independent of the reyes algorithm
	- [aajob.hpp](src/reyes/aajob.hpp): multithreading management
	- [backend.hpp](src/reyes/backend.hpp): win api and opengl init
	- [mem.hpp](src/reyes/mem.hpp): memory management
- **reyes/**
	- [settings.hpp](src/reyes/settings.hpp): modifies behavior and data structures
	- [pipeline.hpp](src/reyes/pipeline.hpp): main rendering procedure and heart of the system
	- [shape.hpp](src/reyes/shape.hpp): main abstraction of the input
	- [shading.hpp](src/reyes/shading.hpp): abstraction of a material and shading system
	- [grid.hpp](src/reyes/grid.hpp): microgrid abstraction
	- [scene.hpp](src/reyes/scene.hpp): convenient data structure
	- **camera/**
		- [scene.hpp](src/reyes/scene.hpp): abstraction of the virtual camera
		- [film.hpp](src/reyes/film.hpp): abstraction of the image's film
		- **film/**: different implementations of the film
	- **lib/**: different implementations of the core parts of the shading system
		- **lights/**
		- **materials/**
		- **samplers/**
		- **shapes/**
	- **misc/**: vector math and matrices

---

#### v1.0

- [x] basic buffer (1 sample per pixel)
- [x] no bucketing
- [x] no splitting and bounding (dice entire primitives, each and every)
- [x] basic pipeline
- [x] basic rasterizer

#### v2.0

- [x] transformations (translate, scale)
- [x] dicing
- [x] bounding and splitting
- [x] entire reyes pipeline
- [x] finalize quadrilaterals and triangles
- [x] G-buffer
- [x] improved memory management and speed
- [x] geometry: plane, sphere, disc, cubic bezier patch, Utah teapot
- [x] support for different materials (custom "shaders")
- [x] texture samplers in "shaders"
- [x] support for lights: directional, point

#### v3.0

- [x] threaded renderer
- [x] improved memory managment
- [ ] A-buffer
    - [ ] *stochastic (multi)sampling*
    - [ ] transparency support
    - [ ] inject optimizations
- [ ] dof
- [ ] motion blur
- [x] task (job) manager
- [ ] profiling
- [ ] bucketing
- [ ] (progress indicator)
- [ ] (camera; perspective and orthographic)
- [ ] (Klein bottle)
- [ ] (SH light)

#### v4.0

- [ ] optimized memory layout
- [ ] (GP)GPU implementation
- [ ] (pseudo) real-time

#### v5.0

- [ ] alogirthms for fixing cracks
- [ ] fix elongated micropolygons
- [ ] dice criterium different
- [ ] dynamic dicing

#### v6.0

- [ ] shadow mapping
- [ ] additional post-processing effects
- [ ] animations
- [ ] materials (toon, phong, ggxD)
- [ ] additional geometry
    - [ ] catmull-clark subdiv
    - [ ] more bezier
    - [ ] nurbs (b-spline)
    - [ ] loop subdiv
- [ ] full Renderman specification
