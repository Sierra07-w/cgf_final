#include "scene/Atmosphere.h"

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

#include "gfx/Shader.h"
#include "gfx/Texture.h"
#include "scene/SceneNode.h"

namespace {

constexpr float twoPi = 6.2831853f;
const glm::vec3 windowOrigin(0.0f, 2.2f, -2.92f);

glm::mat4 translation(float x, float y, float z) {
    return glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

float beamLength(const glm::vec3& direction, float originY) {
    const float floorReach = direction.y < -0.01f ? originY / -direction.y : 6.0f;
    const float wallReach = direction.z > 0.01f
        ? (2.85f - windowOrigin.z) / direction.z
        : 6.0f;
    return std::min(9.0f, std::min(floorReach, wallReach));
}

glm::mat4 beamRotation(const glm::vec3& direction) {
    const glm::vec3 axisY = glm::normalize(direction);
    glm::vec3 axisX = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), axisY);
    if (glm::length(axisX) < 0.001f) {
        axisX = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    axisX = glm::normalize(axisX);
    const glm::vec3 axisZ = glm::normalize(glm::cross(axisX, axisY));

    return glm::mat4(axisX.x, axisX.y, axisX.z, 0.0f,
                     axisY.x, axisY.y, axisY.z, 0.0f,
                     axisZ.x, axisZ.y, axisZ.z, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f);
}

}

void updateRain(const std::vector<std::shared_ptr<SceneNode>>& drops,
                std::vector<glm::vec3>& state, bool enabled, float deltaTime) {
    for (size_t i = 0; i < drops.size(); ++i) {
        if (!enabled) {
            drops[i]->setLocalMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(0.0f)));
            continue;
        }

        float y = state[i].y - state[i].z * deltaTime;
        if (y < 1.25f) {
            y = 3.25f;
        }
        state[i].y = y;
        drops[i]->setLocalMatrix(translation(state[i].x, y, -3.12f));
    }
}

void updateSteam(const std::vector<std::shared_ptr<SceneNode>>& puffs,
                 const std::vector<glm::vec3>& state, float elapsedTime) {
    for (size_t i = 0; i < puffs.size(); ++i) {
        const float phase = std::fmod(elapsedTime * 0.35f + state[i].y, 0.7f);
        const float y = 1.28f + phase;
        const float scale = 1.0f - phase * 0.55f;
        puffs[i]->setLocalMatrix(translation(state[i].x, y, 0.0f) *
                                 glm::scale(glm::mat4(1.0f), glm::vec3(scale)));
    }
}

void updateZParticles(const std::vector<std::shared_ptr<SceneNode>>& particles,
                      const std::vector<float>& phases, std::vector<float>& alphas,
                      float elapsedTime, float calm) {
    for (size_t i = 0; i < particles.size(); ++i) {
        const float phase = std::fmod(elapsedTime * 0.32f + phases[i], 1.0f);
        const float x = -1.35f + std::sin(phase * twoPi + static_cast<float>(i)) * 0.06f;
        const float y = 1.32f + phase * 0.95f;
        const float scale = 0.95f - phase * 0.35f;
        particles[i]->setLocalMatrix(translation(x, y, 1.43f) *
                                     glm::scale(glm::mat4(1.0f), glm::vec3(scale)));
        particles[i]->updateWorld(glm::mat4(1.0f));
        alphas[i] = 0.85f * (1.0f - phase) * calm;
    }
}

void updateSunbeams(const std::vector<std::shared_ptr<SceneNode>>& shafts,
                    std::vector<float>& alphas, const glm::vec3& beamDirection,
                    const glm::vec3& rayColor, float rayStrength, float curtainOpen) {
    for (size_t i = 0; i < shafts.size(); ++i) {
        const float across = static_cast<float>(static_cast<int>(i % 3) - 1);
        const float height = static_cast<float>(static_cast<int>(i / 3) - 1);
        const float middle = 1.0f - std::abs(across);

        const glm::vec3 direction = glm::normalize(
            beamDirection + glm::vec3(across * 0.055f, height * 0.015f, 0.0f));
        const glm::vec3 origin = windowOrigin +
                                 glm::vec3(across * 0.72f, height * 0.65f, 0.0f);
        const float reach = beamLength(direction, origin.y);
        const glm::vec3 center = origin + direction * (reach * 0.5f);

        const float width = (0.78f + 0.28f * middle) * (0.9f + 0.1f * rayStrength);
        const float depth = 0.30f + 0.10f * middle;

        shafts[i]->setLocalMatrix(glm::translate(glm::mat4(1.0f), center) *
                                  beamRotation(-direction) *
                                  glm::scale(glm::mat4(1.0f), glm::vec3(width, reach, depth)));
        shafts[i]->setColorMultiplier(rayColor);
        shafts[i]->updateWorld(glm::mat4(1.0f));

        alphas[i] = 0.09f * (0.8f + 0.2f * middle) * rayStrength * curtainOpen;
    }
}

void updateCurtains(float& open, bool closed, float deltaTime,
                    const std::shared_ptr<SceneNode>& left,
                    const std::shared_ptr<SceneNode>& right) {
    const float target = closed ? 0.0f : 1.0f;
    open += (target - open) * std::min(1.0f, deltaTime * 3.5f);

    left->setLocalMatrix(translation(glm::mix(-0.45f, -1.25f, open), 2.2f, -2.9f));
    right->setLocalMatrix(translation(glm::mix(0.45f, 1.25f, open), 2.2f, -2.9f));
}

void drawWithAlpha(const Shader& shader, const Texture& texture,
                   const std::vector<std::shared_ptr<SceneNode>>& nodes,
                   const std::vector<float>& alphas) {
    for (size_t i = 0; i < nodes.size(); ++i) {
        shader.setFloat("uAlpha", alphas[i]);
        nodes[i]->draw(shader, texture);
    }
}
