#include <cmath>
#include <exception>
#include <iostream>

#include "graphics/cube.hpp"
#include "graphics/shader.hpp"
#include "graphics/window.hpp"
#include "human/human.hpp"
#include "math/math.hpp"

namespace
{
    const float kPi = 3.14159265f;

    void drawFloor(const Shader &shader, const Cube &cube)
    {
        Mat4 model = Mat4::translation(Vec3(0.0f, -0.03f, 0.0f))
                     * Mat4::scale(Vec3(7.0f, 0.06f, 7.0f));
        shader.setMat4("uModel", model);
        shader.setVec3("uColor", Vec3(0.10f, 0.11f, 0.14f));
        cube.draw();
    }

    bool isPressed(GLFWwindow *window, int key)
    {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }
}

int main()
{
    try
    {
        Window window(1280, 720, "HumanGL");
        window.setClearColor(0.07f, 0.08f, 0.10f, 1.0f);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);

        Shader shader;
        Cube cube;
        Human human;

        shader.use();

        const Mat4 view = Mat4::lookAt(
            Vec3(3.2f, 2.3f, 5.4f),
            Vec3(0.0f, 1.5f, 0.0f),
            Vec3(0.0f, 1.0f, 0.0f));

        double lastTime = glfwGetTime();
        bool spaceWasDown = false;
        bool oneWasDown = false;
        bool twoWasDown = false;
        bool threeWasDown = false;
        bool fourWasDown = false;
        bool fiveWasDown = false;
        bool sixWasDown = false;

        while (!window.shouldClose())
        {
            window.pollEvents();

            const double now = glfwGetTime();
            float deltaTime = static_cast<float>(now - lastTime);
            lastTime = now;
            if (deltaTime > 0.1f)
                deltaTime = 0.1f;

            GLFWwindow *handle = window.handle();

            if (isPressed(handle, GLFW_KEY_ESCAPE))
                glfwSetWindowShouldClose(handle, GLFW_TRUE);

            if (isPressed(handle, GLFW_KEY_W) && human.motion() != Motion::Walk)
                human.setMotion(Motion::Walk);
            if (isPressed(handle, GLFW_KEY_S) && human.motion() != Motion::Idle)
                human.setMotion(Motion::Idle);

            const bool spaceDown = isPressed(handle, GLFW_KEY_SPACE);
            if (spaceDown && !spaceWasDown)
                human.setMotion(Motion::Jump);
            spaceWasDown = spaceDown;

            const bool oneDown = isPressed(handle, GLFW_KEY_1);
            const bool twoDown = isPressed(handle, GLFW_KEY_2);
            const bool threeDown = isPressed(handle, GLFW_KEY_3);
            const bool fourDown = isPressed(handle, GLFW_KEY_4);
            const bool fiveDown = isPressed(handle, GLFW_KEY_5);
            const bool sixDown = isPressed(handle, GLFW_KEY_6);

            if (oneDown && !oneWasDown)
                human.growArms(-0.08f);
            if (twoDown && !twoWasDown)
                human.growArms(0.08f);
            if (threeDown && !threeWasDown)
                human.growLegs(-0.08f);
            if (fourDown && !fourWasDown)
                human.growLegs(0.08f);
            if (fiveDown && !fiveWasDown)
                human.growTorso(-0.1f);
            if (sixDown && !sixWasDown)
                human.growTorso(0.1f);

            oneWasDown = oneDown;
            twoWasDown = twoDown;
            threeWasDown = threeDown;
            fourWasDown = fourDown;
            fiveWasDown = fiveDown;
            sixWasDown = sixDown;

            human.update(deltaTime);

            const Mat4 projection = Mat4::perspective(
                45.0f * kPi / 180.0f, window.aspect(), 0.1f, 100.0f);

            shader.use();
            shader.setMat4("uViewProjection", projection * view);

            window.clear();
            drawFloor(shader, cube);
            human.draw(shader, cube);
            window.swapBuffers();
        }
    }
    catch (const std::exception &error)
    {
        std::cerr << "HumanGL: " << error.what() << std::endl;
        return 1;
    }
    return 0;
}
