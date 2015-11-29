#pragma once

#include <cmath>
#include <cstdint>

#define M_E 2.71828182845904523536
#define M_LOG2E 1.44269504088896340736
#define M_LOG10E 0.434294481903251827651
#define M_LN2 0.693147180559945309417
#define M_LN10 2.30258509299404568402
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.785398163397448309616
#define M_1_PI 0.318309886183790671538
#define M_2_PI 0.636619772367581343076
#define M_1_SQRTPI 0.564189583547756286948
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2 1.41421356237309504880
#define M_SQRT_2 0.707106781186547524401

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
        friend vec3 operator-(const vec3& a, const vec3& b)
        {
            return {a.x-b.x, a.y-b.y, a.z-b.z};
        }
        vec3 operator+(vec3& v)
        {
            return vec3(x + v.x, y + v.y, z + v.z);
        }
        vec3& operator+=(vec3& v)
        {
            return *this = *this + v;
        }
        friend vec3 operator+(const vec3& a, const vec3& b)
        {
            return{ a.x + b.x, a.y + b.y, a.z + b.z };
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
        friend vec3 operator/(const vec3& a, const float s)
        {
            return{ a.x / s, a.y / s, a.z / s };
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
        friend vec3 operator*(const vec3& a, const vec3& b)
        {
            return{ a.x * b.x, a.y * b.y, a.z * b.z };
        }
        friend vec3 operator*(const vec3& a, const float s)
        {
            return{ a.x * s, a.y *s, a.z * s };
        }
        float operator[](int i)
        {
            if (0 == i) return x;
            else if (1 == i) return y;
            else if (2 == i) return z;
            return 0;
        }
        friend vec3 cross(const vec3& a, const vec3& b)
        {
            float ax = a.x, ay = a.y, az = a.z;
            float bx = b.x, by = b.y, bz = b.z;

            vec3 c;
            c.x = ay * bz - az * by;
            c.y = az * bx - ax * bz;
            c.z = ax * by - ay * bx;
            return c;
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

        void transpose(void)
        {
            float tmp;
            tmp = e[1]; e[1] = e[4]; e[4] = tmp;
            tmp = e[2]; e[2] = e[8]; e[8] = tmp;
            tmp = e[6]; e[6] = e[9]; e[9] = tmp;

            tmp = e[3]; e[3] = e[12]; e[12] = tmp;
            tmp = e[7]; e[7] = e[13]; e[13] = tmp;
            tmp = e[11]; e[11] = e[14]; e[14] = tmp;
        }

        void orthographic(float left, float right, float bottom, float top, float near, float far)
        {
            float w = right - left;
            float h = top - bottom;
            float p = far - near;

            float x = (right + left) / w;
            float y = (top + bottom) / h;
            float z = (far + near) / p;

            e[0] = 2 / w;   e[4] = 0;       e[8] = 0;       e[12] = 0;
            e[1] = 0;       e[5] = 2 / h;   e[9] = 0;       e[13] = 0;
            e[2] = 0;       e[6] = 0;       e[10] = -2 / p; e[14] = 0;
            e[3] = -x;      e[7] = -y;      e[11] = -z;     e[15] = 1;
        }

        void frustum(float left, float right, float bottom, float top, float near, float far)
        {
            float x = 2 * near / (right - left);
            float y = 2 * near / (top - bottom);

            float a = (right + left) / (right - left);
            float b = (top + bottom) / (top - bottom);
            float c = -(far + near) / (far - near);
            float d = -2 * far * near / (far - near);

            e[0] = x; e[4] = 0; e[8] = a;   e[12] = 0;
            e[1] = 0; e[5] = y; e[9] = b;   e[13] = 0;
            e[2] = 0; e[6] = 0; e[10] = c;  e[14] = d;
            e[3] = 0; e[7] = 0; e[11] = -1; e[15] = 0;

            transpose();
        }

        void lookAt(const vec3 eye, const vec3 target, const vec3 up)
        {
            vec3 z = (eye-target).normalize();

            if (z.len() ==0 )
                z.z = 1;

            vec3 x = cross(up, z).normalize();

            if (x.len() == 0) {
                z.x += 0.0001f;
                x = cross(up, z).normalize();
            }

            vec3 y = cross(z, x);


            e[0] = x.x; e[4] = y.x; e[8] = z.x;
            e[1] = x.y; e[5] = y.y; e[9] = z.y;
            e[2] = x.z; e[6] = y.z; e[10] = z.z;
        }
    };

    /* Axis-algined boundig box (2D). */
    struct AABB2
    {
        vec2 min, max;
    };

    /* Axis-aligned bounding box (3D). */
    struct AABB3
    {
        vec3 min, max;
    };

    typedef vec3 position;
    typedef vec3 normal;
    typedef uint8_t material;

    struct PosNormalMat
    {
        position position;
        normal normal;
        material material;
    };
    struct PosColor
    {
        position position;
        color color;
    };
}