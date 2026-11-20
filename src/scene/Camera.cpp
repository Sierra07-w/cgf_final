#include "scene/Camera.h"

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
    : mode(Mode::Orbit),
      eye(2.0f, 4.0f, 8.0f),
      target(0.0f, 1.5f, 0.0f),
      yaw(-105.0f),
      pitch(-18.0f),
      distance(8.5f),
      moveSpeed(4.0f),
      orbitSensitivity(0.25f),
      freeSensitivity(0.1f),
      zoomStep(0.9f),
      fov(45.0f),
      nearPlane(0.1f),
      farPlane(100.0f) {
}

void Camera::toggleMode() {
    if (mode == Mode::Orbit) {
        eye = getPosition();
        mode = Mode::Free;
    } else {
        target = eye + getFront() * distance;
        mode = Mode::Orbit;
    }
}

void Camera::move(bool forward, bool backward, bool left, bool right,
                  bool up, bool down, float deltaTime) {
    const glm::vec3 front = getFront();
    const glm::vec3 groundFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    const glm::vec3 rightDirection =
        glm::normalize(glm::cross(groundFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    const float step = moveSpeed * deltaTime;

    if (mode == Mode::Orbit) {
        if (forward) {
            target += groundFront * step;
        }
        if (backward) {
            target -= groundFront * step;
        }
        if (right) {
            target += rightDirection * step;
        }
        if (left) {
            target -= rightDirection * step;
        }
        if (up) {
            target.y += step;
        }
        if (down) {
            target.y -= step;
        }
        return;
    }

    if (forward) {
        eye += front * step;
    }
    if (backward) {
        eye -= front * step;
    }
    if (right) {
        eye += rightDirection * step;
    }
    if (left) {
        eye -= rightDirection * step;
    }
    if (up) {
        eye.y += step;
    }
    if (down) {
        eye.y -= step;
    }
}

void Camera::rotate(float deltaX, float deltaY) {
    const float sensitivity = mode == Mode::Free ? freeSensitivity : orbitSensitivity;

    yaw += deltaX * sensitivity;
    pitch += deltaY * sensitivity;
    pitch = std::clamp(pitch, -89.0f, 89.0f);
}

void Camera::zoom(float offset) {
    if (mode != Mode::Orbit) {
        return;
    }

    distance = std::clamp(distance * std::pow(zoomStep, offset), 2.0f, 25.0f);
}

glm::mat4 Camera::getViewMatrix() const {
    const glm::vec3 position = getPosition();
    return glm::lookAt(position, position + getFront(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix(float aspect) const {
    return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}

glm::vec3 Camera::getPosition() const {
    if (mode == Mode::Free) {
        return eye;
    }

    return target - getFront() * distance;
}

Camera::Mode Camera::getMode() const {
    return mode;
}

glm::vec3 Camera::getFront() const {
    const float yawRadians = glm::radians(yaw);
    const float pitchRadians = glm::radians(pitch);
    const float cosPitch = std::cos(pitchRadians);

    return glm::vec3(cosPitch * std::cos(yawRadians),
                     std::sin(pitchRadians),
                     cosPitch * std::sin(yawRadians));
}

void Camera::getRay(float ndcX, float ndcY, float aspect,
                    glm::vec3& origin, glm::vec3& direction) const {
    const glm::mat4 inverseProjection = glm::inverse(getProjectionMatrix(aspect));
    const glm::mat4 inverseView = glm::inverse(getViewMatrix());

    glm::vec4 nearPoint = inverseProjection * glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
    glm::vec4 farPoint = inverseProjection * glm::vec4(ndcX, ndcY, 1.0f, 1.0f);
    nearPoint /= nearPoint.w;
    farPoint /= farPoint.w;

    const glm::vec4 nearWorld = inverseView * nearPoint;
    const glm::vec4 farWorld = inverseView * farPoint;

    origin = glm::vec3(nearWorld);
    direction = glm::normalize(glm::vec3(farWorld) - origin);
}
