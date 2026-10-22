#ifndef MAT4_HPP
#define MAT4_HPP

#include "math/vec3.hpp"

/*
** Column-major 4x4 matrix, directly compatible with glUniformMatrix4fv.
** m[column * 4 + row]
*/
struct Mat4
{
    float m[16];

    Mat4();

    static Mat4 identity();
    static Mat4 translation(const Vec3 &offset);
    static Mat4 scale(const Vec3 &factor);
    static Mat4 rotationX(float radians);
    static Mat4 rotationY(float radians);
    static Mat4 rotationZ(float radians);

    static Mat4 perspective(float fovYRadians, float aspect, float nearPlane, float farPlane);
    static Mat4 lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up);

    Mat4 operator*(const Mat4 &rhs) const;
};

#endif
