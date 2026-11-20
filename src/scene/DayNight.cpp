#include "scene/DayNight.h"

#include <algorithm>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "scene/Lighting.h"
#include "scene/SceneNode.h"

namespace {

constexpr float twoPi = 6.2831853f;
const glm::vec3 windowOrigin(0.0f, 2.2f, -3.0f);

}

float updateDayNight(float& phase, float deltaTime, bool enabled, float speed,
                     const std::shared_ptr<SceneNode>& sun, Lighting& lighting,
                     glm::vec3& beamDirection, glm::vec3& rayColor,
                     float& rayStrength) {
    if (enabled) {
        phase += deltaTime * speed * twoPi;
    }

    const float elevation = std::sin(phase);
    const float daylight = std::clamp((elevation + 0.15f) / 0.7f, 0.0f, 1.0f);
    const float sunX = -std::cos(phase) * 2.2f;
    const float sunY = 2.6f + elevation * 1.0f;

    sun->setLocalMatrix(glm::translate(glm::mat4(1.0f),
                                       glm::vec3(sunX, sunY, -3.8f)));

    const glm::vec3 virtualSun(sunX * 0.25f, 3.0f + elevation, -4.2f);
    const glm::vec3 toSun = glm::normalize(virtualSun - windowOrigin);
    beamDirection = -toSun;

    rayStrength = std::clamp((elevation - 0.02f) / 0.5f, 0.0f, 1.0f);

    const float horizon = 1.0f - daylight;
    const float evening = std::clamp(sunX / 2.2f, 0.0f, 1.0f);
    const glm::vec3 noonColor(1.0f, 1.0f, 0.95f);
    const glm::vec3 morningColor(1.0f, 0.94f, 0.72f);
    const glm::vec3 eveningColor(1.0f, 0.52f, 0.22f);

    glm::vec3 color = glm::mix(noonColor, morningColor, horizon * (1.0f - evening) * 0.75f);
    rayColor = glm::mix(color, eveningColor, horizon * evening);

    if (enabled) {
        lighting.ambientColor = glm::mix(glm::vec3(0.12f, 0.13f, 0.2f),
                                         glm::vec3(0.34f, 0.32f, 0.29f), daylight);
        lighting.windowLight.color = glm::mix(glm::vec3(0.25f, 0.35f, 0.65f),
                                              glm::vec3(0.68f, 0.82f, 1.0f), daylight);
        lighting.windowLight.color = glm::mix(lighting.windowLight.color, rayColor,
                                              0.3f * horizon * evening);
        lighting.windowLight.intensity = 0.15f + 1.05f * daylight;
        lighting.windowLight.direction = beamDirection;
    }

    return daylight;
}
