#pragma once

#include <model.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

struct TransformComponent
{
    glm::vec3 translation{};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
    glm::vec3 rotation{};

    glm::mat4 mat4()
    {
        auto transform = glm::translate(glm::mat4{1.0f}, translation);
        transform = glm::rotate(transform, rotation.y, {0.0f, 1.0f, 0.0f});
        transform = glm::rotate(transform, rotation.x, {1.0f, 0.0f, 0.0f});
        transform = glm::rotate(transform, rotation.z, {0.0f, 0.0f, 1.0f});
        transform = glm::scale(transform, scale);
        return transform;
        return transform;
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
        TransformComponent transform{};

    private:
        GameObject(id_t objId) : id{objId} {};
        id_t id;
    };
} // namespace hex
