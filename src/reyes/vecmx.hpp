#pragma once

#include <cmath>

namespace reyes
{
    struct color
    {
        float r, g, b, a;
        color(float _r = 0, float _g = 0, float _b = 0, float _a = 0)
            : r(_r)
            , g(_g)
            , b(_b)
            , a(_a)
        {}
    };

    struct vec2
    {
        float x, y;

        vec2(float _x = 0.0, float _y = 0.0)
            : x(_x)
            , y(_y)
        {}

        float len()
        {
            return sqrt(x*x + y*y);
        }
        vec2& normalize()
        {
            float l = len();
            if (l > 0.00001)
                *this /= l;
            return *this;
        }
        vec2& operator=(vec2& v)
        {
            x = v.x;
            y = v.y;
            return *this;
        }
        vec2 operator-(vec2& v)
        {
            return vec2(x - v.x, y - v.y);
        }
        vec2& operator-=(vec2& v)
        {
            return *this = *this - v;
        }
        vec2 operator+(vec2& v)
        {
            return vec2(x + v.x, y + v.y);
        }
        vec2& operator+=(vec2& v)
        {
            return *this = *this + v;
        }
        vec2 operator/(float s)
        {
            return vec2(x / s, y / s);
        }
        vec2& operator/=(float s)
        {
            return *this = *this / s;
        }
        vec2 operator*(float s)
        {
            return vec2(x * s, y * s);
        }
        vec2& operator*=(float s)
        {
            return *this = *this * s;
        }
        float operator*(vec2& v)
        {
            return x*v.y - y*v.x;
        }
    };

    struct vec3
    {
        float x, y, z;

        vec3(float _x = 0.0, float _y = 0.0, float _z = 0.0)
            : x(_x)
            , y(_y)
            , z(_z)
        {}

        float len()
        {
            return sqrt(x*x + y*y + z*z);
        }
        vec3& normalize()
        {
            float l = len();
            if (l > 0.00001)
                *this /= l;
            return *this;
        }
        vec3& operator=(vec3& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
            return *this;
        }
        vec3 operator-(vec3& v)
        {
            return vec3(x - v.x, y - v.y, z - v.z);
        }
        vec3& operator-=(vec3& v)
        {
            return *this = *this - v;
        }
        vec3 operator+(vec3& v)
        {
            return vec3(x + v.x, y + v.y, z + v.z);
        }
        vec3& operator+=(vec3& v)
        {
            return *this = *this + v;
        }
        vec3 operator/(float s)
        {
            float is = 1.0f / s;
            return *this*is;
        }
        vec3& operator/=(float s)
        {
            return *this = *this / s;
        }
        vec3 operator*(float s)
        {
            return vec3(x * s, y * s, z*s);
        }
        vec3& operator*=(float s)
        {
            return *this = *this * s;
        }
        float operator*(vec3& v)
        {
            return x*v.x + y*v.y + z*v.z;
        }

        float operator[](int i)
        {
            if (0 == i) return x;
            else if (1 == i) return y;
            else if (2 == i) return z;
            return 0;
        }
    };

    struct vec4
    {
        float x, y, z, w;

        vec4(float _x = 0.0, float _y = 0.0, float _z = 0.0, float _w = 0.0)
            : x(_x)
            , y(_y)
            , z(_z)
            , w(_w)
        {}

        float len()
        {
            return sqrt(x*x + y*y + z*z + w*w);
        }
        vec4& normalize()
        {
            float l = len();
            if (l > 0.00001)
                *this /= l;
            return *this;
        }
        vec4& operator=(vec4& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
            return *this;
        }
        vec4 operator-(vec4& v)
        {
            return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
        }
        vec4& operator-=(vec4& v)
        {
            return *this = *this - v;
        }
        vec4 operator+(vec4& v)
        {
            return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
        }
        vec4& operator+=(vec4& v)
        {
            return *this = *this + v;
        }
        vec4 operator/(float s)
        {
            float is = 1.0f / s;
            return *this*is;
        }
        vec4& operator/=(float s)
        {
            return *this = *this / s;
        }
        vec4 operator*(float s)
        {
            return vec4(x * s, y * s, z*s, w*s);
        }
        vec4& operator*=(float s)
        {
            return *this = *this * s;
        }
        float operator*(vec4& v)
        {
            return x*v.x + y*v.y + z*v.z + w*v.w;
        }

        float operator[](int i)
        {
            if (0 == i) return x;
            else if (1 == i) return y;
            else if (2 == i) return z;
            else if (3 == i) return w;
            return 0;
        }
    };

    struct mx3
    {
        float e[9];
        mx3()
        {
            for (char i = 0; i < 9; i++)
                e[i] = (0 == i % 4 ? 1.0f : 0.0f);
        }
    };

    struct mx4
    {
        float e[16];
        mx4()
        {
            for (char i = 0; i < 16; i++)
                e[i] = (0 == i % 5 ? 1.0f : 0.0f);
        }
    };
}