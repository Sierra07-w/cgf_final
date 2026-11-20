#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "gfx/Shader.h"
#include "gfx/Texture.h"
#include "scene/Lighting.h"
#include "scene/SceneNode.h"

struct SceneOptions {
    bool lampOn = true;
    bool rainOn = true;
    bool flicker = false;
    bool ballRolling = false;
    float ballSpeed = 1.6f;
    bool dayNightOn = true;
    float dayCycleSpeed = 0.04f;
    glm::vec3 lampOffset = glm::vec3(0.0f);
};

class Scene {
public:
    Scene();

    void update(float deltaTime, float elapsedTime, Lighting& lighting,
                const SceneOptions& options);
    void draw(const Shader& shader, const Texture& texture) const;
    void drawParticles(const Shader& shader, const Texture& texture) const;

    bool pickCat(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const;
    void pokeCat();
    bool isCatGrowling() const;

private:
    std::shared_ptr<SceneNode> root;
    std::shared_ptr<SceneNode> ball;
    float ballPhase = 0.0f;
    std::shared_ptr<SceneNode> lampShade;
    std::shared_ptr<SceneNode> sun;
    float sunPhase = 0.0f;
    std::shared_ptr<SceneNode> plantLeaf;
    std::shared_ptr<SceneNode> rainRoot;
    std::shared_ptr<SceneNode> steamRoot;
    std::vector<std::shared_ptr<SceneNode>> rainDrops;
    std::vector<std::shared_ptr<SceneNode>> steamPuffs;
    std::vector<glm::vec3> rainState;
    std::vector<glm::vec3> steamState;

    std::shared_ptr<SceneNode> sleepingCat;
    std::shared_ptr<SceneNode> sleepingCatBody;
    std::shared_ptr<SceneNode> sleepingCatHead;
    std::shared_ptr<SceneNode> catMouth;
    std::vector<std::shared_ptr<SceneNode>> catEars;
    std::vector<glm::mat4> catEarBase;
    std::vector<std::shared_ptr<SceneNode>> zParticles;
    std::vector<float> zPhases;
    std::vector<float> zAlphas;
    float catReactionTime = 0.0f;
};
