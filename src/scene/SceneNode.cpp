#include "scene/SceneNode.h"

#include <utility>

SceneNode::SceneNode(std::shared_ptr<Mesh> mesh, int pattern, bool useTexture,
                     const glm::vec3& colorMultiplier)
    : mesh(std::move(mesh)),
      pattern(pattern),
      useTexture(useTexture),
      colorMultiplier(colorMultiplier) {
}

void SceneNode::addChild(const std::shared_ptr<SceneNode>& child) {
    children.push_back(child);
}

void SceneNode::setLocalMatrix(const glm::mat4& matrix) {
    localMatrix = matrix;
}

const glm::mat4& SceneNode::getLocalMatrix() const {
    return localMatrix;
}

const glm::mat4& SceneNode::getWorldMatrix() const {
    return worldMatrix;
}

void SceneNode::updateWorld(const glm::mat4& parentWorld) {
    worldMatrix = parentWorld * localMatrix;

    for (const auto& child : children) {
        child->updateWorld(worldMatrix);
    }
}

void SceneNode::draw(const Shader& shader, const Texture& texture) const {
    if (mesh != nullptr) {
        shader.setMat4("model", worldMatrix);
        shader.setInt("uPattern", pattern);
        shader.setBool("uUseTexture", useTexture);
        shader.setVec3("uColorMultiplier", colorMultiplier);
        texture.bind();
        mesh->draw();
    }

    for (const auto& child : children) {
        child->draw(shader, texture);
    }
}
