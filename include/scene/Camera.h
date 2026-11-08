#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    enum class Mode {
        Orbit,
        Free
    };

    Camera();

    void toggleMode();
    void move(bool forward, bool backward, bool left, bool right,
              bool up, bool down, float deltaTime);
    void rotate(float deltaX, float deltaY);
    void zoom(float offset);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspect) const;
    glm::vec3 getPosition() const;
    Mode getMode() const;

private:
    glm::vec3 getFront() const;

    Mode mode;
    glm::vec3 eye;
    glm::vec3 target;
    float yaw;
    float pitch;
    float distance;
    float moveSpeed;
    float orbitSensitivity;
    float freeSensitivity;
    float zoomStep;
    float fov;
    float nearPlane;
    float farPlane;
};
