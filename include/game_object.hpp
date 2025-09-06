#pragma once

#include <model.hpp>

#include <memory>

struct Transform2DComponent
{
    glm::vec2 translation{};
    glm::vec2 scale{1.0f, 1.0f};
    float rotation{0.0f};

    glm::mat2 mat2()
    {
        glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};
        glm::mat2 rotMat{{cos(rotation), sin(rotation)}, {-sin(rotation), cos(rotation)}};
        return rotMat * scaleMat;
    }
};

namespace hex
{
    class GameObject
    {
    public:
        using id_t = unsigned int;

        static GameObject createGameObject()
        {
            static id_t currentId = 0;
            return GameObject(currentId++);
        }

        GameObject(const GameObject &) = delete;
        GameObject &operator=(const GameObject &) = delete;
        GameObject(GameObject &&) = default;
        GameObject &operator=(GameObject &&) = default;

        id_t getId() { return id; }

        std::shared_ptr<Model> model{};
        glm::vec3 color{};
        Transform2DComponent transform2D{};

    private:
        GameObject(id_t objId) : id{objId} {};
        id_t id;
    };
} // namespace hex
