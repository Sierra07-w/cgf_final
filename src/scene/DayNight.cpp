#include "scene/DayNight.h"

#include <algorithm>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "scene/Lighting.h"
#include "scene/SceneNode.h"

namespace {

constexpr float twoPi = 6.2831853f;

}

float updateDayNight(float& phase, float deltaTime, bool enabled, float speed,
                     const std::shared_ptr<SceneNode>& sun, Lighting& lighting) {
    if (enabled) {
        phase += deltaTime * speed * twoPi;
    }

    const float elevation = std::sin(phase);
    const float daylight = std::clamp((elevation + 0.15f) / 0.7f, 0.0f, 1.0f);
    const float sunX = -std::cos(phase) * 2.2f;

    sun->setLocalMatrix(glm::translate(
        glm::mat4(1.0f),
        glm::vec3(sunX, 1.4f + elevation * 1.6f, -9.0f)));

    if (enabled) {
        lighting.ambientColor = glm::mix(glm::vec3(0.12f, 0.13f, 0.2f),
                                         glm::vec3(0.34f, 0.32f, 0.29f), daylight);
        lighting.windowLight.color = glm::mix(glm::vec3(0.25f, 0.35f, 0.65f),
                                              glm::vec3(0.68f, 0.82f, 1.0f), daylight);
        lighting.windowLight.intensity = 0.15f + 1.05f * daylight;
        lighting.windowLight.direction =
            glm::normalize(glm::vec3(-sunX * 0.15f, -0.45f, 1.0f));
    }

    return enabled ? daylight : 1.0f;
}
