#pragma once

#include <memory>

class Lighting;
class SceneNode;

float updateDayNight(float& phase, float deltaTime, bool enabled, float speed,
                     const std::shared_ptr<SceneNode>& sun, Lighting& lighting);
