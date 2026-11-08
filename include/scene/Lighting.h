#pragma once

#include <glm/glm.hpp>

class Shader;

struct Material {
    glm::vec3 ambient = glm::vec3(0.35f);
    glm::vec3 diffuse = glm::vec3(1.0f);
    glm::vec3 specular = glm::vec3(0.2f);
    float shininess = 24.0f;
};

struct DirectionalLight {
    glm::vec3 direction = glm::vec3(0.2f, -0.5f, 1.0f);
    glm::vec3 color = glm::vec3(0.55f, 0.72f, 0.95f);
    float intensity = 0.9f;
};

struct PointLight {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 1.0f;
    float constant = 1.0f;
    float linear = 0.14f;
    float quadratic = 0.07f;
};

class Lighting {
public:
    Lighting();

    void apply(const Shader& shader, const Material& material) const;

    glm::vec3 ambientColor;
    DirectionalLight windowLight;
    PointLight deskLamp;
    PointLight fillLight;
};
