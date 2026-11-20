#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "gfx/Mesh.h"
#include "gfx/Shader.h"
#include "gfx/Texture.h"

class SceneNode {
public:
    explicit SceneNode(std::shared_ptr<Mesh> mesh = nullptr,
                       int pattern = 0,
                       bool useTexture = false,
                       const glm::vec3& colorMultiplier = glm::vec3(1.0f));

    void addChild(const std::shared_ptr<SceneNode>& child);

    void setLocalMatrix(const glm::mat4& matrix);
    void setColorMultiplier(const glm::vec3& color);
    const glm::mat4& getLocalMatrix() const;
    const glm::mat4& getWorldMatrix() const;

    void updateWorld(const glm::mat4& parentWorld);
    void draw(const Shader& shader, const Texture& texture) const;

private:
    std::shared_ptr<Mesh> mesh;
    std::vector<std::shared_ptr<SceneNode>> children;
    glm::mat4 localMatrix = glm::mat4(1.0f);
    glm::mat4 worldMatrix = glm::mat4(1.0f);
    int pattern = 0;
    bool useTexture = false;
    glm::vec3 colorMultiplier = glm::vec3(1.0f);
};
