#include "graphics/window.hpp"

#include <iostream>
#include <stdexcept>

void Window::errorCallback(int code, const char *description)
{
    std::cerr << "GLFW error " << code << ": " << description << std::endl;
}

void Window::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (self == nullptr)
        return;
    self->_width = width;
    self->_height = height;
    glViewport(0, 0, width, height);
}

Window::Window(int width, int height, const std::string &title)
    : _window(nullptr), _width(width), _height(height)
{
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit())
        throw std::runtime_error("failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    _window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (_window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("failed to create the GLFW window (OpenGL 4.0 required)");
    }

    glfwMakeContextCurrent(_window);
    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, framebufferSizeCallback);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwDestroyWindow(_window);
        glfwTerminate();
        throw std::runtime_error("failed to load OpenGL functions with GLAD");
    }

    if (!GLAD_GL_VERSION_4_0)
    {
        glfwDestroyWindow(_window);
        glfwTerminate();
        throw std::runtime_error("an OpenGL 4.0 context is required");
    }

    glViewport(0, 0, width, height);
    std::cout << "OpenGL " << glGetString(GL_VERSION) << " - " << glGetString(GL_RENDERER) << std::endl;
}

Window::~Window()
{
    if (_window != nullptr)
        glfwDestroyWindow(_window);
    glfwTerminate();
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(_window) != 0;
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::swapBuffers()
{
    glfwSwapBuffers(_window);
}

void Window::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::setClearColor(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
}

GLFWwindow *Window::handle() const
{
    return _window;
}

int Window::width() const
{
    return _width;
}

int Window::height() const
{
    return _height;
}

float Window::aspect() const
{
    if (_height <= 0)
        return 1.0f;
    return static_cast<float>(_width) / static_cast<float>(_height);
}
