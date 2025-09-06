#include "app.hpp"

#include <stdexcept>
#include <array>
#include <glm/gtc/constants.hpp>
#include "simple_render_system.hpp"

namespace hex
{
    App::App()
    {
        loadGameObjects();
    }

    App::~App() {}

    void App::run()
    {
        SimpleRenderSystem simpleRenderSystem{device, renderer.getSwapChainRenderPass()};

        while (!window.shouldClose())
        {
            glfwPollEvents();

            if (auto commandBuffer = renderer.beginFrame())
            {
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }

    void App::loadGameObjects()
    {
        std::vector<Model::Vertex> vertices{
            {{-0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.0f, -0.5f}, {0.0f, 0.0f, 1.0f}},
        };
        auto model = std::make_shared<Model>(device, vertices);

        auto triangle = GameObject::createGameObject();
        triangle.model = model;
        triangle.color = {0.1f, 0.8f, 0.1f};

        gameObjects.push_back(std::move(triangle));
    }
}