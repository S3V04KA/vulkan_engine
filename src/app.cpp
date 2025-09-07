#include "app.hpp"

#include <stdexcept>
#include <array>
#include <chrono>
#include <glm/gtc/constants.hpp>
#include "simple_render_system.hpp"
#include "camera.hpp"
#include "movement_controller.hpp"

#define MAX_FRAME_TIME 1.0f / 60.0f

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
        Camera camera{};

        auto viewerObject = GameObject::createGameObject();
        MovementController cameraController{window.getGLFWwindow(), viewerObject};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!window.shouldClose())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;
            frameTime = glm::min(frameTime, MAX_FRAME_TIME);

            cameraController.moveInPlaneXZ(frameTime);
            cameraController.lookAround(frameTime);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = renderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(60.f), aspect, 0.1f, 100.0f);

            if (auto commandBuffer = renderer.beginFrame())
            {
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }

    std::unique_ptr<Model> createTestCubeModel(Device &device, glm::vec3 offset)
    {
        Model::Builder modelBuilder{};
        modelBuilder.vertices = {
            {{-.5f, -.5f, -.5f}, {0.0f, 0.0f, 0.0f}},
            {{-.5f, .5f, -.5f}, {1.0f, 0.0f, 0.0f}},
            {{.5f, .5f, -.5f}, {0.0f, 1.0f, 0.0f}},
            {{.5f, -.5f, -.5f}, {1.0f, 1.0f, 0.0f}},

            {{-.5f, -.5f, .5f}, {0.0f, 0.0f, 1.0f}},
            {{-.5f, .5f, .5f}, {1.0f, 0.0f, 1.0f}},
            {{.5f, .5f, .5f}, {0.0f, 1.0f, 1.0f}},
            {{.5f, -.5f, .5f}, {1.0f, 1.0f, 1.0f}},
        };

        modelBuilder.indices = {
            0, 1, 2, 0, 2, 3,
            4, 5, 6, 4, 6, 7,
            0, 3, 7, 0, 7, 4,
            1, 5, 6, 1, 6, 2,
            4, 0, 5, 1, 5, 0,
            3, 2, 7, 6, 2, 7};

        return std::make_unique<Model>(device, modelBuilder);
    }

    void App::loadGameObjects()
    {
        // std::shared_ptr<Model> model = createTestCubeModel(device, {0.0f, 0.0f, 0.0f});
        std::shared_ptr<Model> model = Model::createModelFromFile(device, "colored_cube");

        auto cube = GameObject::createGameObject();
        cube.model = model;
        cube.transform.translation = {0.0f, 0.0f, 2.5f};
        cube.transform.scale = {1.0f, 1.0f, 1.0f};

        gameObjects.push_back(std::move(cube));
    }
}