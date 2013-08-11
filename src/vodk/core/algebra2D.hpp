
#ifndef VODK_CORE_ALGEBRA2D_HPP
#define VODK_CORE_ALGEBRA2D_HPP

#include <cmath>
#include <stdint.h>
#include <memory.h>

namespace vodk {

const float PI = 3.14159265358979323846;

struct Mat3;
struct Mat4;
struct Vec2;
struct Vec3;

inline float modulo(float a, float m) {
    float r = std::fmod(a,m);
    return r*m > 0 ? r : r + m;
}

/**
 * Signed anlge difference between two angles
 */
float angular_distance(float a, float b);

/**
 * return the same angle in [-PI..PI]
 */
float normalized_angle(float a);

namespace mat3 {
Mat3 rotate(float angle);
Mat3 translate(const Vec2& v);
Mat3 scale(const Vec2& v);
Mat3 identity();
Mat3 multiply(const Mat3& a, const Mat3& b);
void multiply_in_place(Mat3& a, const Mat3& b);
Vec3 multiply(const Mat3& a, const Vec3& b);
Vec2 multiply(const Mat3& a, const Vec2& b);
Mat3 add(const Mat3& a, const Mat3& b);
void add_in_place(Mat3& a, const Mat3& b);
Mat3 substract(const Mat3& a, const Mat3& b);
void substract_in_place(Mat3& a, const Mat3& b);
Mat3 rotate_and_translate(float angle, const Vec2& v);
} // mat3


struct Vec3 {
    Vec3()
    : x(0.0), y(0.0), z(0.0)
    {}

    Vec3(float ax, float ay, float az)
    : x(ax), y(ay), z(az)
    {}

    float& operator[](uint32_t i) { return *(pointer() + i); }
    const float& operator[](uint32_t i) const { return *(pointer() + i); }

    Vec3 operator + (const Vec3& other) {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator - (const Vec3& other) {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator * (float v) {
        return Vec3(x * v, y * v, z * v);
    }

    void operator *= (float s) {
        x *= s;
        y *= s;
        z *= s;
    }

    void operator += (const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    void operator -= (const Vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
    }

    float* pointer() { return &x; }

    const float* pointer() const { return &x; }

    float x, y, z;
};

struct Vec2 {
    Vec2()
    : x(0.0), y(0.0)
    {}

    Vec2(float ax, float ay)
    : x(ax), y(ay)
    {}

    float& operator[](uint32_t i) { return *(pointer() + i); }

    const float& operator[](uint32_t i) const { return *(pointer() + i); }

    Vec2 operator + (const Vec2& other) {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator - (const Vec2& other) {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 operator * (float v) {
        return Vec2(x * v, y * v);
    }

    void operator *= (float s) {
        x *= s;
        y *= s;
    }

    void operator += (const Vec2& other) {
        x += other.x;
        y += other.y;
    }

    void operator -= (const Vec2& other) {
        x -= other.x;
        y -= other.y;
    }

    float* pointer() { return &x; }

    const float* pointer() const { return &x; }

    float x, y;
};

struct Mat3 {
    float _00, _01, _02,
          _10, _11, _12,
          _20, _21, _22;

    Mat3(float a00, float a01, float a02,
         float a10, float a11, float a12,
         float a20, float a21, float a22)
    : _00(a00), _01(a01), _02(a02)
    , _10(a10), _11(a11), _12(a12)
    , _20(a20), _21(a21), _22(a22)
    {}

    Mat3(float* ptr) {
        memcpy(&_00, ptr, 9*sizeof(float));
    }

    float* pointer() { return &_00; }

    float& get(uint32_t col, uint32_t row) {
        return *(pointer() + col + row * 3);
    }

    Vec3& operator[](uint32_t index) {
        return *reinterpret_cast<Vec3*>(pointer()+index*3);
    }

    Mat3 operator*(const Mat3& rhs) {
        return mat3::multiply(*this, rhs);
    }

    Vec3 operator*(const Vec3& rhs) {
        return mat3::multiply(*this, rhs);
    }

    Vec2 operator*(const Vec2& rhs) {
        return mat3::multiply(*this, rhs);
    }

    void operator*=(const Mat3& rhs) {
        return mat3::multiply_in_place(*this, rhs);
    }
};

inline
Mat3& as_mat3(float* data) { return *reinterpret_cast<Mat3*>(data); }

inline
Mat4& as_mat4(float* data) { return *reinterpret_cast<Mat4*>(data); }

namespace mat3 {

inline
Mat3 identity() {
    return Mat3(1.0, 0.0, 0.0,
                0.0, 1.0, 0.0,
                0.0, 0.0, 1.0);
}

inline
Mat3 rotate(float angle) {
    return Mat3(cos(angle), -sin(angle), 0.0,
                sin(angle), cos(angle),  0.0,
                0.0,        0.0,         1.0);
}

inline
Mat3 scale(const Vec2& v) {
    return Mat3(v.x, 0.0, 0.0,
                0.0, v.y, 0.0,
                0.0, 0.0, 1.0);
}

inline
Mat3 translate(const Vec2& v) {
    return Mat3(1.0, 0.0, v.x,
                0.0, 1.0, v.y,
                0.0, 0.0, 1.0);
}

inline
Mat3 multiply(const Mat3& a, const Mat3& b) {
    return Mat3(
        a._00*b._00 + a._01*b._10 + a._02*b._20,
        a._00*b._01 + a._01*b._11 + a._02*b._21,
        a._00*b._02 + a._01*b._12 + a._02*b._22,

        a._10*b._00 + a._11*b._10 + a._12*b._20,
        a._10*b._01 + a._11*b._11 + a._12*b._21,
        a._10*b._02 + a._11*b._12 + a._12*b._22,

        a._20*b._00 + a._21*b._10 + a._22*b._20,
        a._20*b._01 + a._21*b._11 + a._22*b._21,
        a._20*b._02 + a._21*b._12 + a._22*b._22
    );
}

inline
void multiply_in_place(Mat3& a, const Mat3& b) {
    a._00 =    a._00*b._00 + a._01*b._10 + a._02*b._20;
    a._01 =    a._00*b._01 + a._01*b._11 + a._02*b._21;
    a._02 =    a._00*b._02 + a._01*b._12 + a._02*b._22;

    a._10 =    a._10*b._00 + a._11*b._10 + a._12*b._20;
    a._11 =    a._10*b._01 + a._11*b._11 + a._12*b._21;
    a._12 =    a._10*b._02 + a._11*b._12 + a._12*b._22;

    a._20 =    a._20*b._00 + a._21*b._10 + a._22*b._20;
    a._21 =    a._20*b._01 + a._21*b._11 + a._22*b._21;
    a._22 =    a._20*b._02 + a._21*b._12 + a._22*b._22;
}

inline
Vec3 multiply(const Mat3& a, const Vec3& b) {
    return Vec3(
        a._00*b.x + a._01*b.y + a._02*b.z,
        a._10*b.x + a._11*b.y + a._12*b.z,
        a._20*b.x + a._21*b.y + a._22*b.z
    );
}

inline
Vec2 multiply(const Mat3& a, const Vec2& b) {
    // like if b.z = 1.0
    return Vec2(
        a._00*b.x + a._01*b.y + a._02,
        a._10*b.x + a._11*b.y + a._12
    );
}


inline
Mat3 add(const Mat3& a, const Mat3& b) {
    return Mat3(
        a._00+b._00, a._01+b._01, a._02+b._02,
        a._10+b._10, a._11+b._11, a._12+b._02,
        a._20+b._20, a._21+b._21, a._22+b._02
    );
}

inline
void add_in_place(Mat3& a, const Mat3& b) {
    a._00 += b._00;
    a._01 += b._01;
    a._02 += b._02;

    a._10 += b._10;
    a._11 += b._11;
    a._12 += b._12;

    a._20 += b._20;
    a._21 += b._21;
    a._22 += b._22;
}

inline
Mat3 substract(const Mat3& a, const Mat3& b) {
    return Mat3(
        a._00-b._00, a._01-b._01, a._02-b._02,
        a._10-b._10, a._11-b._11, a._12-b._02,
        a._20-b._20, a._21-b._21, a._22-b._02
    );
}

inline
void substract_in_place(Mat3& a, const Mat3& b) {
    a._00 -= b._00;
    a._01 -= b._01;
    a._02 -= b._02;

    a._10 -= b._10;
    a._11 -= b._11;
    a._12 -= b._12;

    a._20 -= b._20;
    a._21 -= b._21;
    a._22 -= b._22;
}

inline
Mat3 rotate_and_translate(float angle, const Vec2& v) {
    // TODO decompose
    return rotate(angle) * translate(v);
}

inline
Mat3 transposed(const Mat3& a) {
    return Mat3(
        a._00, a._10, a._20,
        a._01, a._11, a._21,
        a._02, a._12, a._22
    );
}

} // mat3

void print(const Mat3& m);
void print(const Vec2& v);
void print(const Vec3& v);

} // vodk

#endif
