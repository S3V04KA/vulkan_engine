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

    // temporary helper function, creates a 1x1x1 cube centered at offset
    // temporary helper function, creates a 1x1x1 cube centered at offset with an index buffer
    std::unique_ptr<Model> createCubeModel(Device &device, glm::vec3 offset)
    {
        Model::Builder modelBuilder{};
        modelBuilder.vertices = {
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        };
        for (auto &v : modelBuilder.vertices)
        {
            v.position += offset;
        }

        modelBuilder.indices = {0, 1, 2, 0, 3, 1, 4, 5, 6, 4, 7, 5, 8, 9, 10, 8, 11, 9,
                                12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};

        return std::make_unique<Model>(device, modelBuilder);
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
        std::shared_ptr<Model> model = createTestCubeModel(device, {0.0f, 0.0f, 0.0f});

        auto cube = GameObject::createGameObject();
        cube.model = model;
        cube.transform.scale = {1.0f, 1.0f, 1.0f};

        gameObjects.push_back(std::move(cube));
    }
}