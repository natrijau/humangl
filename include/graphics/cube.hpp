#ifndef CUBE_HPP
#define CUBE_HPP

#include "graphics/opengl.hpp"

/*
** A single 1x1x1 cube centered on the origin.
** draw() is the only function used to render a body part: every part calls
** it exactly once, after its matrix has been placed on the stack.
*/
class Cube
{
public:
    Cube();
    ~Cube();

    Cube(const Cube &) = delete;
    Cube &operator=(const Cube &) = delete;

    void draw() const;

private:
    GLuint _vao;
    GLuint _vbo;
    GLuint _ebo;
};

#endif
