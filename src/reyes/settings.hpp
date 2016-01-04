#pragma once
#include "vecmx.hpp"
#define TRIANGLE_GRID 0
#define QUAD_GRID 1


// ---- <settings> ----------------------------

#define REYES_VERSION "2.0"
#define GRID_DIM 8
#define GRID_TYPE TRIANGLE_GRID
namespace reyes { static const vec2 RASTER_THRESHOLD = { 16, 16 }; }
#define GIRDPOOL_SIZE 20
#define SCENEMEM_SIZE 1<<23
//#define DEBUG_GRID
//#define DEBUG_PRIM
#ifdef DEBUG_GRID
//#define DEBUG_THREAD
#endif

// ---- </settings> ---------------------------

#define GRID_SIZE ((GRID_DIM + 1)*(GRID_DIM + 1))
#if GRID_TYPE==TRIANGLE_GRID
#define GRID_IDX_SIZE GRID_DIM*GRID_DIM * 6
#define GRID_PRIM_SIZE GRID_DIM*GRID_DIM*2
#else
#define GRID_IDX_SIZE GRID_DIM*GRID_DIM * 4
#define GRID_PRIM_SIZE GRID_DIM*GRID_DIM
#endif