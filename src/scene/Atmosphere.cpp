#include "scene/Atmosphere.h"

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

#include "gfx/Shader.h"
#include "gfx/Texture.h"
#include "scene/SceneNode.h"

namespace {

constexpr float twoPi = 6.2831853f;
constexpr float pi = 3.14159265f;
const glm::vec3 windowOrigin(0.0f, 2.2f, -3.0f);

glm::mat4 translation(float x, float y, float z) {
    return glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

float beamLength(const glm::vec3& direction) {
    if (direction.y >= -0.01f) {
        return 6.0f;
    }
    return std::min(6.0f, 2.2f / -direction.y);
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

void updateDust(const std::vector<std::shared_ptr<SceneNode>>& motes,
                const std::vector<glm::vec3>& state, std::vector<float>& alphas,
                float elapsedTime, const glm::vec3& beamDirection,
                float daylight, float curtainOpen) {
    const float length = beamLength(beamDirection);

    glm::vec3 side = glm::cross(beamDirection, glm::vec3(0.0f, 1.0f, 0.0f));
    if (glm::length(side) < 0.001f) {
        side = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    side = glm::normalize(side);
    const glm::vec3 other = glm::normalize(glm::cross(side, beamDirection));

    for (size_t i = 0; i < motes.size(); ++i) {
        const float phase = std::fmod(elapsedTime * 0.12f + state[i].y, 1.0f);
        const float sway = std::sin(phase * twoPi + state[i].y * 9.0f) * 0.08f;
        const float lift = std::cos(phase * twoPi + state[i].y * 7.0f) * 0.05f;

        const glm::vec3 position = windowOrigin + beamDirection * (phase * length) +
                                   side * (state[i].x + sway) + other * lift;
        const float scale = state[i].z * (0.7f + 0.3f * std::sin(phase * pi));

        motes[i]->setLocalMatrix(glm::translate(glm::mat4(1.0f), position) *
                                 glm::scale(glm::mat4(1.0f), glm::vec3(scale)));
        motes[i]->updateWorld(glm::mat4(1.0f));

        alphas[i] = 0.55f * std::sin(phase * pi) * daylight * curtainOpen;
    }
}

void updateSunbeams(const std::vector<std::shared_ptr<SceneNode>>& shafts,
                    std::vector<float>& alphas, const glm::vec3& beamDirection,
                    float daylight, float curtainOpen) {
    const float length = beamLength(beamDirection);
    const glm::mat4 rotation = beamRotation(beamDirection);
    const glm::vec3 center = windowOrigin + beamDirection * (length * 0.5f);

    for (size_t i = 0; i < shafts.size(); ++i) {
        shafts[i]->setLocalMatrix(glm::translate(glm::mat4(1.0f), center) * rotation *
                                  glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, length, 1.0f)));
        alphas[i] = (0.075f - static_cast<float>(i) * 0.02f) * daylight * curtainOpen;
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
