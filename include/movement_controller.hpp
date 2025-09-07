#pragma once

#include "game_object.hpp"
#include "window.hpp"

namespace hex
{
    class MovementController
    {
    public:
        struct KeyMappings
        {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_SPACE;
            int moveDown = GLFW_KEY_LEFT_SHIFT;

            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
        };

        MovementController(GLFWwindow *window, GameObject &gameObject, float moveSpeed = 3.0f, float lookSpeed = 1.5f, float sensitivity = 2.0f) : window(window), gameObject(gameObject), moveSpeed(moveSpeed), lookSpeed(lookSpeed), sensitivity(sensitivity) {};
        ~MovementController() = default;

        MovementController(const MovementController &) = delete;
        MovementController &operator=(const MovementController &) = delete;

        void moveInPlaneXZ(float dt);
        void lookAround(float dt);

    private:
        GLFWwindow *window;
        GameObject &gameObject;
        KeyMappings keys{};
        float moveSpeed;
        float lookSpeed;
        float sensitivity;
        bool firstLoad = true;
    };
}