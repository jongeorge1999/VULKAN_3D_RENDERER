#pragma once

#include "Model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <unordered_map>

struct TransformComponent {
    glm::vec3 translation{};
    glm::vec3 scale{1.f, 1.f, 1.f};
    glm::vec3 rotation{};

    // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
    // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
    glm::mat4 mat4();
    glm::mat3 normalMatrix();
};

class Object {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, Object>;

        static Object createObject() {
            static id_t currentId = 0;
            return Object(currentId++);
        }

        Object(const Object&) = delete;
        Object &operator = (const Object&) = delete;
        Object(Object&&) = default;
        Object &operator = (Object&&) = default;

        id_t getId() {return id;}

        std::shared_ptr<Model> model{};
        glm::vec3 color{};
        TransformComponent transform{};
        bool shouldRotateY{false};

    private:
        Object(id_t objId) : id{objId} {};

        id_t id;
};