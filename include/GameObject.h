#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "Model.h"
#include <memory>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

struct Transform2dComponent {
    glm::vec2 transform{};
    glm::vec2 scale{1.f, 1.f};
    float rotation;

    glm::mat2 mat2() {
        const float s = glm::sin(rotation);
        const float c = glm::cos(rotation);
        glm::mat2 rotMat = {
            {c, s}, {-s, c}
        };

        glm::mat2 scaleMat{
            {scale.x, .0f},
            {.0f, scale.y}
        };

        return rotMat * scaleMat;
    };
};

class GameObject {
    public:
    using id_t = unsigned int;

    // GameObject(GameObject &&) = delete;
    // GameObject &operator=(GameObject &&) = delete;

    static GameObject New() {
        static id_t current = 0;
        return GameObject{current++};
    };

    inline id_t getId() { return id; };

    std::shared_ptr<Model> model{};
    glm::vec3 color;
    Transform2dComponent transform2d{};

    private:
    GameObject(id_t id) : id(id) {};
    id_t id;
};

#endif // GAMEOBJECT_H_