#include "scene/Lighting.h"

#include <string>

#include "gfx/Shader.h"

namespace {

void applyPointLight(const Shader& shader, const std::string& name, const PointLight& light) {
    shader.setVec3(name + ".position", light.position);
    shader.setVec3(name + ".color", light.color);
    shader.setFloat(name + ".intensity", light.intensity);
    shader.setFloat(name + ".constant", light.constant);
    shader.setFloat(name + ".linear", light.linear);
    shader.setFloat(name + ".quadratic", light.quadratic);
}

}

Lighting::Lighting()
    : ambientColor(0.18f, 0.16f, 0.14f) {
    windowLight.direction = glm::vec3(0.15f, -0.45f, 1.0f);
    windowLight.color = glm::vec3(0.55f, 0.72f, 0.95f);
    windowLight.intensity = 0.9f;

    deskLamp.position = glm::vec3(1.6f, 1.6f, -1.5f);
    deskLamp.color = glm::vec3(1.0f, 0.6f, 0.3f);
    deskLamp.intensity = 2.4f;
    deskLamp.constant = 1.0f;
    deskLamp.linear = 0.14f;
    deskLamp.quadratic = 0.07f;

    fillLight.position = glm::vec3(-1.5f, 2.8f, 1.5f);
    fillLight.color = glm::vec3(0.55f, 0.6f, 0.75f);
    fillLight.intensity = 0.7f;
    fillLight.constant = 1.0f;
    fillLight.linear = 0.09f;
    fillLight.quadratic = 0.032f;
}

void Lighting::apply(const Shader& shader, const Material& material) const {
    shader.setVec3("ambientColor", ambientColor);
    shader.setVec3("material.ambient", material.ambient);
    shader.setVec3("material.diffuse", material.diffuse);
    shader.setVec3("material.specular", material.specular);
    shader.setFloat("material.shininess", material.shininess);

    shader.setVec3("windowLight.direction", windowLight.direction);
    shader.setVec3("windowLight.color", windowLight.color);
    shader.setFloat("windowLight.intensity", windowLight.intensity);

    applyPointLight(shader, "deskLamp", deskLamp);
    applyPointLight(shader, "fillLight", fillLight);
}
