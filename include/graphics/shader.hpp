#ifndef SHADER_HPP
#define SHADER_HPP

#include "graphics/opengl.hpp"
#include "math/mat4.hpp"
#include "math/vec3.hpp"

class Shader
{
public:
    Shader();
    ~Shader();

    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    void use() const;
    void setMat4(const char *name, const Mat4 &value) const;
    void setVec3(const char *name, const Vec3 &value) const;

private:
    GLuint _program;

    static GLuint compile(GLenum type, const char *source);
    static void checkLink(GLuint program);
};

#endif
