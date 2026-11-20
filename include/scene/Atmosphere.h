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

void updateZParticles(const std::vector<std::shared_ptr<SceneNode>>& particles,
                      const std::vector<float>& phases, std::vector<float>& alphas,
                      float elapsedTime, float calm);

void updateSunbeams(const std::vector<std::shared_ptr<SceneNode>>& shafts,
                    std::vector<float>& alphas, const glm::vec3& beamDirection,
                    const glm::vec3& rayColor, float rayStrength, float curtainOpen);

void updateCurtains(float& open, bool closed, float deltaTime,
                    const std::shared_ptr<SceneNode>& left,
                    const std::shared_ptr<SceneNode>& right);

void drawWithAlpha(const Shader& shader, const Texture& texture,
                   const std::vector<std::shared_ptr<SceneNode>>& nodes,
                   const std::vector<float>& alphas);
