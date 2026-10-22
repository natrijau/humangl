#ifndef MATRIX_STACK_HPP
#define MATRIX_STACK_HPP

#include <vector>

#include "math/mat4.hpp"
#include "math/vec3.hpp"

/*
** The project's own matrix stack. Every push() saves the current frame,
** every pop() restores it. Children are always drawn while their parent's
** matrix is at the top of the stack, which is what makes the hierarchy work.
*/
class MatrixStack
{
public:
    MatrixStack() : _stack(1, Mat4::identity()) {}

    void push()
    {
        _stack.push_back(_stack.back());
    }

    void pop()
    {
        if (_stack.size() > 1)
            _stack.pop_back();
    }

    void translate(const Vec3 &offset)
    {
        _stack.back() = _stack.back() * Mat4::translation(offset);
    }

    void scale(const Vec3 &factor)
    {
        _stack.back() = _stack.back() * Mat4::scale(factor);
    }

    void rotateX(float radians)
    {
        _stack.back() = _stack.back() * Mat4::rotationX(radians);
    }

    void rotateY(float radians)
    {
        _stack.back() = _stack.back() * Mat4::rotationY(radians);
    }

    void rotateZ(float radians)
    {
        _stack.back() = _stack.back() * Mat4::rotationZ(radians);
    }

    void apply(const Mat4 &transform)
    {
        _stack.back() = _stack.back() * transform;
    }

    const Mat4 &top() const
    {
        return _stack.back();
    }

    void reset()
    {
        _stack.assign(1, Mat4::identity());
    }

private:
    std::vector<Mat4> _stack;
};

#endif
