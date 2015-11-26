#pragma once

#include <cmath>
#include "vec.hpp"

typedef float* mat3_t;
typedef float* mat4_t;

#define mat3(_name) float _name[9] = {}
#define mat4(_name) float _name[16] = {}