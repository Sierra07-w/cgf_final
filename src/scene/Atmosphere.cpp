#include "scene/Atmosphere.h"

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

#include "gfx/Shader.h"
#include "gfx/Texture.h"
#include "scene/SceneNode.h"

namespace {

constexpr float twoPi = 6.2831853f;

glm::mat4 translation(float x, float y, float z) {
    return glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
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

void updateDust(const std::vector<std::shared_ptr<SceneNode>>& motes,
                const std::vector<glm::vec3>& state, std::vector<float>& alphas,
                float elapsedTime, float daylight, float curtainOpen) {
    for (size_t i = 0; i < motes.size(); ++i) {
        const float phase = std::fmod(elapsedTime * 0.12f + state[i].y, 1.0f);
        const float sway = std::sin(phase * twoPi + state[i].y * 9.0f) * 0.08f;
        const float x = state[i].x + sway;
        const float y = 2.85f - phase * 2.55f;
        const float z = -2.75f + phase * 4.0f;
        const float scale = state[i].z * (0.7f + 0.3f * std::sin(phase * 3.14159265f));

        motes[i]->setLocalMatrix(translation(x, y, z) *
                                 glm::scale(glm::mat4(1.0f), glm::vec3(scale)));
        motes[i]->updateWorld(glm::mat4(1.0f));

        alphas[i] = 0.55f * std::sin(phase * 3.14159265f) * daylight * curtainOpen;
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

void drawDust(const Shader& shader, const Texture& texture,
              const std::vector<std::shared_ptr<SceneNode>>& motes,
              const std::vector<float>& alphas) {
    for (size_t i = 0; i < motes.size(); ++i) {
        shader.setFloat("uAlpha", alphas[i]);
        motes[i]->draw(shader, texture);
    }
}
