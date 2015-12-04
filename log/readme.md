This log serves a purpose of reminding myself of some of the problems that I faced, frustrations that kept wake at night and how I handled/managed them. Log wasn't started immediately as the project did, but it's late a few days.

***3.12.2015.***
---

First rendered image.

![alt text](reyes_first.png "First image")

I will also soon discuss some of the issues that I came along during the initial effort to build a pipeline, new realizations and problems that weren't initially planned. Yet, I'm pretty happy to see the thing displayed, even though it was a master cheat to get that image. Image is actually 10x10, there is practically no camera, no projection, no bound-split, not even a real dicing. No shaders. Only quarter of parallel quadrilateral (blue) is visible. I said it was a cheat. :D

So, v1.0 us out. I've modified goals in a readme.

***on inverse interpolation:*** While I was thinking about rasterization, I needed some way of getting interpolated attributes for a given point in space. So this is the problem: the primitive is ignorant of the rasterization that will happen on it. And the Image is responsible for generating samples (some Image might generate only one sample, while anti-aliased buffers will try to generate many samples). In conclusion, it is the Image that will generate sample points, and therefore I need some way of getting a color for a generated sample. So, I need piece of code that will input a Primitive (or the Shape that is made of bunch of Primitives) and a sample point (location), and the output would be a color of the Primitive at that location (interpolated value). I currently plan three basic Primitives: quadrilaterals, triangles and polygons. So for each of those, I need inverse interpolation function. [Shadertoy - quadrilateral interpolation](https://www.shadertoy.com/view/ldt3Wr) is the toy that I made, with my own version of the method (which is wrong and introduces UV seams), but later got corrected by Inigo Quilez how to do it properly. Entire code is on the Shadertoy. So, quadrilaterals are good. I also found [inverse barycentric](http://ofekshilon.com/2013/01/21/geometric-inverse-application-1-barycentric-coordinates/) article. I'll throw another Shadertoy example soon ([inverse barycentric](https://www.shadertoy.com/view/4d3GDH).

---

***on reyes:*** Figuring out the REYES was a fun ride. It took me a while to see what is the pipeline, what are the required functions, what are the pros and cons of the REYES over simple rasterization and pathtracing. So, where does reyes fit into rendering strategies?

REYES is rasterization algorithm. One things that is good at - adaptive tessellation. Like it's presented in the "reyes architecture and implementation" slides in the papers folder, the usual, uniform tessellation of the primitives often leads to problem of over-tessellation or under-tessellation (I have no idea if these are valid words). So REYES' pipeline is built on "screen-space tessellation". It bounds shape, approximates its dimensions, and splits if needed to reach a threshold. The goal of splitting to some threshold is to be memory-friendly, mostly. Eventually, the shape will be diced, i.e. cut, into small, tiny, tiny chunks, called micropolygons. Micropolygons are polygons that are around or under 1px size (area or diameter). Dicing entire shape into micropolygons without bound-split would leave us with massive amounts of micropolygons in memory. Also, dicing the shape into bunch of micropoygons is like super highres uniform tessellation, and we already concluded that's not always the best way to go about rendering. So bound-split servers a purpose of adaptive tessellation estimation, and dicing is there to actually tessellate the final, split parts of the shape.

So, bound-split and dicing, all good. We've adaptively tessellated our shape by splitting it into smaller parts of it, and then we uniformly tessellated those parts. What's left are microgrids of micropolygons.

Next step is shading. Micrpolygons are so small, that they are 1px in size, they are like fragments (from fragments shader). We can shade. Just points inside the microgrid, and interpolate colors accross the micropolygon.

Final step is sampling, aka rasterization. It is very much like traditional rasterization where pipeline would generate sampling points, the primitives (micropolygons) are sampled at those points and everything is accumulated in the color buffer.

One neat things is bucketing. We can subdivide screen into smaller regions and go through them independently. On the slides preivously mentiones, they also mention that storing microgrids is not very memory-efficient neither is holding entire color buffer. So we can just do things in chunks, which is good.