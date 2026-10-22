#include "math/mat4.hpp"

#include <cmath>

Mat4::Mat4()
{
    for (int i = 0; i < 16; ++i)
        m[i] = 0.0f;
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;
}

Mat4 Mat4::identity()
{
    return Mat4();
}

Mat4 Mat4::translation(const Vec3 &offset)
{
    Mat4 result;
    result.m[12] = offset.x;
    result.m[13] = offset.y;
    result.m[14] = offset.z;
    return result;
}

Mat4 Mat4::scale(const Vec3 &factor)
{
    Mat4 result;
    result.m[0] = factor.x;
    result.m[5] = factor.y;
    result.m[10] = factor.z;
    return result;
}

Mat4 Mat4::rotationX(float radians)
{
    Mat4 result;
    float c = std::cos(radians);
    float s = std::sin(radians);
    result.m[5] = c;
    result.m[6] = s;
    result.m[9] = -s;
    result.m[10] = c;
    return result;
}

Mat4 Mat4::rotationY(float radians)
{
    Mat4 result;
    float c = std::cos(radians);
    float s = std::sin(radians);
    result.m[0] = c;
    result.m[2] = -s;
    result.m[8] = s;
    result.m[10] = c;
    return result;
}

Mat4 Mat4::rotationZ(float radians)
{
    Mat4 result;
    float c = std::cos(radians);
    float s = std::sin(radians);
    result.m[0] = c;
    result.m[1] = s;
    result.m[4] = -s;
    result.m[5] = c;
    return result;
}

Mat4 Mat4::perspective(float fovYRadians, float aspect, float nearPlane, float farPlane)
{
    Mat4 result;
    float f = 1.0f / std::tan(fovYRadians / 2.0f);
    float range = nearPlane - farPlane;

    result.m[0] = f / aspect;
    result.m[5] = f;
    result.m[10] = (farPlane + nearPlane) / range;
    result.m[11] = -1.0f;
    result.m[14] = (2.0f * farPlane * nearPlane) / range;
    result.m[15] = 0.0f;
    return result;
}

Mat4 Mat4::lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up)
{
    Vec3 forward = (center - eye).normalized();
    Vec3 side = Vec3::cross(forward, up).normalized();
    Vec3 realUp = Vec3::cross(side, forward);

    Mat4 result;
    result.m[0] = side.x;
    result.m[4] = side.y;
    result.m[8] = side.z;
    result.m[12] = -Vec3::dot(side, eye);

    result.m[1] = realUp.x;
    result.m[5] = realUp.y;
    result.m[9] = realUp.z;
    result.m[13] = -Vec3::dot(realUp, eye);

    result.m[2] = -forward.x;
    result.m[6] = -forward.y;
    result.m[10] = -forward.z;
    result.m[14] = Vec3::dot(forward, eye);

    result.m[3] = 0.0f;
    result.m[7] = 0.0f;
    result.m[11] = 0.0f;
    result.m[15] = 1.0f;
    return result;
}

Mat4 Mat4::operator*(const Mat4 &rhs) const
{
    Mat4 result;

    for (int column = 0; column < 4; ++column)
    {
        for (int row = 0; row < 4; ++row)
        {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k)
                sum += m[k * 4 + row] * rhs.m[column * 4 + k];
            result.m[column * 4 + row] = sum;
        }
    }
    return result;
}
