#ifndef APP_H_
#define APP_H_

#include "Window.h"
#include "Renderer.h"
#include "Pipeline.h"
#include "Device.h"
#include "GameObject.h"
#include "Global.h"
#include <ctime>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

class App {
    public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    App();
    ~App();

    App(const App &) = delete;
    App &operator=(const App &) = delete;

    int run();
    private:
    void loadGameObjects();
    void createPipelineLayout();
    void createPipeline();
    void renderGameObjects(VkCommandBuffer commandBuffer);

    clock_t deltaTime = 0;
    int frames = 0;
    int frame = 0;

    Window WINDOW{WIDTH, HEIGHT, std::string("York Engine v") + YORK_ENGINE_VER};
    VKDevice DEVICE{WINDOW};
    Renderer RENDERER{WINDOW, DEVICE};

    std::unique_ptr<VKPipeline> PIPELINE;
    VkPipelineLayout pipelineLayout;
    std::vector<GameObject> gameObjects;
};

#endif // APP_H_