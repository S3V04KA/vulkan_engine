#include "movement_controller.hpp"

namespace hex
{
    void MovementController::moveInPlaneXZ(float dt)
    {
        glm::vec3 rotate{0};
        if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS)
            rotate.y += 1.0f;
        if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS)
            rotate.y -= 1.0f;
        if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS)
            rotate.x += 1.0f;
        if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS)
            rotate.x -= 1.0f;

        if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
            gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);

        gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
        gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

        float yaw = gameObject.transform.rotation.y;
        const glm::vec3 forwardDir{sin(yaw), 0.0f, cos(yaw)};
        const glm::vec3 rightDir{forwardDir.z, 0.0f, -forwardDir.x};
        const glm::vec3 upDir{0.0f, 1.0f, 0.0f};

        glm::vec3 moveDir{0.0f};
        if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS)
            moveDir += forwardDir;
        if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)
            moveDir -= forwardDir;
        if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)
            moveDir += rightDir;
        if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS)
            moveDir -= rightDir;
        if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS)
            moveDir += upDir;
        if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS)
            moveDir -= upDir;

        if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
            gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
    }

    void MovementController::lookAround(float dt)
    {
        if (firstLoad)
        {
            glfwSetCursorPos(window, 0.0, 0.0);
            firstLoad = false;
        }

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        glfwSetCursorPos(window, 0.0, 0.0);

        if (xpos != 0.0 || ypos != 0.0)
        {
            gameObject.transform.rotation.y += sensitivity * 0.1f * static_cast<float>(xpos) * dt;
            gameObject.transform.rotation.x -= sensitivity * 0.1f * static_cast<float>(ypos) * dt;
        }

        gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
        gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());
    }
}