#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>

struct Vec3
{
    float x;
    float y;
    float z;

    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(float xValue, float yValue, float zValue) : x(xValue), y(yValue), z(zValue) {}

    Vec3 operator+(const Vec3 &other) const
    {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator-(const Vec3 &other) const
    {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator*(float scalar) const
    {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    Vec3 &operator+=(const Vec3 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    float length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vec3 normalized() const
    {
        float len = length();
        if (len == 0.0f)
            return Vec3(0.0f, 0.0f, 0.0f);
        return Vec3(x / len, y / len, z / len);
    }

    static float dot(const Vec3 &a, const Vec3 &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static Vec3 cross(const Vec3 &a, const Vec3 &b)
    {
        return Vec3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x);
    }
};

#endif
