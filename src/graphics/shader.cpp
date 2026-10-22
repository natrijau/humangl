#include "graphics/shader.hpp"

#include <stdexcept>
#include <string>

static const char *kVertexSource = R"GLSL(#version 400 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uViewProjection;

out vec3 vNormal;

void main()
{
    vNormal = normalize(mat3(uModel) * aNormal);
    gl_Position = uViewProjection * uModel * vec4(aPosition, 1.0);
}
)GLSL";

static const char *kFragmentSource = R"GLSL(#version 400 core
in vec3 vNormal;

uniform vec3 uColor;

out vec4 fragColor;

void main()
{
    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(vec3(0.45, 0.85, 0.60));
    float diffuse = max(dot(normal, lightDirection), 0.0);
    float ambient = 0.35;
    vec3 color = uColor * (ambient + 0.65 * diffuse);
    fragColor = vec4(color, 1.0);
}
)GLSL";

GLuint Shader::compile(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[4096];
        GLsizei length = 0;
        glGetShaderInfoLog(shader, sizeof(log), &length, log);
        glDeleteShader(shader);
        throw std::runtime_error(std::string("shader compilation failed:\n") + log);
    }
    return shader;
}

void Shader::checkLink(GLuint program)
{
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[4096];
        GLsizei length = 0;
        glGetProgramInfoLog(program, sizeof(log), &length, log);
        throw std::runtime_error(std::string("shader link failed:\n") + log);
    }
}

Shader::Shader()
    : _program(0)
{
    GLuint vertexShader = compile(GL_VERTEX_SHADER, kVertexSource);
    GLuint fragmentShader = 0;

    try
    {
        fragmentShader = compile(GL_FRAGMENT_SHADER, kFragmentSource);
    }
    catch (...)
    {
        glDeleteShader(vertexShader);
        throw;
    }

    _program = glCreateProgram();
    glAttachShader(_program, vertexShader);
    glAttachShader(_program, fragmentShader);
    glLinkProgram(_program);

    glDetachShader(_program, vertexShader);
    glDetachShader(_program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    try
    {
        checkLink(_program);
    }
    catch (...)
    {
        glDeleteProgram(_program);
        _program = 0;
        throw;
    }
}

Shader::~Shader()
{
    if (_program != 0)
        glDeleteProgram(_program);
}

void Shader::use() const
{
    glUseProgram(_program);
}

void Shader::setMat4(const char *name, const Mat4 &value) const
{
    glUniformMatrix4fv(glGetUniformLocation(_program, name), 1, GL_FALSE, value.m);
}

void Shader::setVec3(const char *name, const Vec3 &value) const
{
    glUniform3f(glGetUniformLocation(_program, name), value.x, value.y, value.z);
}
