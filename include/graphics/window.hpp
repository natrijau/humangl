#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>

#include "graphics/opengl.hpp"

class Window
{
public:
    Window(int width, int height, const std::string &title);
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    bool shouldClose() const;
    void pollEvents();
    void swapBuffers();
    void clear();

    void setClearColor(float red, float green, float blue, float alpha);

    GLFWwindow *handle() const;
    int width() const;
    int height() const;
    float aspect() const;

private:
    GLFWwindow *_window;
    int _width;
    int _height;

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    static void errorCallback(int code, const char *description);
};

#endif
