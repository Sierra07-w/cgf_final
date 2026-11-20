#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

class SceneNode;
class Shader;
class Texture;

void updateRain(const std::vector<std::shared_ptr<SceneNode>>& drops,
                std::vector<glm::vec3>& state, bool enabled, float deltaTime);

void updateSteam(const std::vector<std::shared_ptr<SceneNode>>& puffs,
                 const std::vector<glm::vec3>& state, float elapsedTime);

void updateDust(const std::vector<std::shared_ptr<SceneNode>>& motes,
                const std::vector<glm::vec3>& state, std::vector<float>& alphas,
                float elapsedTime, float daylight, float curtainOpen);

void updateCurtains(float& open, bool closed, float deltaTime,
                    const std::shared_ptr<SceneNode>& left,
                    const std::shared_ptr<SceneNode>& right);

void drawDust(const Shader& shader, const Texture& texture,
              const std::vector<std::shared_ptr<SceneNode>>& motes,
              const std::vector<float>& alphas);
