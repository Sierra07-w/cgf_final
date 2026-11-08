#include "scene/Room.h"

#include <cstddef>
#include <vector>

#include <glad/gl.h>
#include <glm/glm.hpp>

namespace {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};

void addTriangle(std::vector<Vertex>& vertices, const glm::vec3& a, const glm::vec3& b,
                 const glm::vec3& c, const glm::vec3& normal, const glm::vec3& color) {
    vertices.push_back({a, normal, color});
    vertices.push_back({b, normal, color});
    vertices.push_back({c, normal, color});
}

void addQuad(std::vector<Vertex>& vertices, const glm::vec3& a, const glm::vec3& b,
             const glm::vec3& c, const glm::vec3& d, const glm::vec3& normal,
             const glm::vec3& color) {
    addTriangle(vertices, a, b, c, normal, color);
    addTriangle(vertices, a, c, d, normal, color);
}

}

Room::Room() {
    const glm::vec3 floorColor(0.43f, 0.31f, 0.22f);
    const glm::vec3 wallColor(0.76f, 0.68f, 0.58f);
    const glm::vec3 frameColor(0.32f, 0.21f, 0.14f);
    const glm::vec3 upNormal(0.0f, 1.0f, 0.0f);
    const glm::vec3 leftWallNormal(1.0f, 0.0f, 0.0f);
    const glm::vec3 backWallNormal(0.0f, 0.0f, 1.0f);

    std::vector<Vertex> vertices;

    addQuad(vertices,
            {-3.0f, 0.0f, -3.0f}, {-3.0f, 0.0f, 3.0f},
            {3.0f, 0.0f, 3.0f}, {3.0f, 0.0f, -3.0f},
            upNormal, floorColor);

    addQuad(vertices,
            {-3.0f, 0.0f, 3.0f}, {-3.0f, 0.0f, -3.0f},
            {-3.0f, 4.0f, -3.0f}, {-3.0f, 4.0f, 3.0f},
            leftWallNormal, wallColor);

    addQuad(vertices,
            {-3.0f, 0.0f, -3.0f}, {3.0f, 0.0f, -3.0f},
            {3.0f, 1.3f, -3.0f}, {-3.0f, 1.3f, -3.0f},
            backWallNormal, wallColor);

    addQuad(vertices,
            {-3.0f, 3.1f, -3.0f}, {3.0f, 3.1f, -3.0f},
            {3.0f, 4.0f, -3.0f}, {-3.0f, 4.0f, -3.0f},
            backWallNormal, wallColor);

    addQuad(vertices,
            {-3.0f, 1.3f, -3.0f}, {-0.9f, 1.3f, -3.0f},
            {-0.9f, 3.1f, -3.0f}, {-3.0f, 3.1f, -3.0f},
            backWallNormal, wallColor);

    addQuad(vertices,
            {0.9f, 1.3f, -3.0f}, {3.0f, 1.3f, -3.0f},
            {3.0f, 3.1f, -3.0f}, {0.9f, 3.1f, -3.0f},
            backWallNormal, wallColor);

    const float frameZ = -2.985f;

    addQuad(vertices,
            {-1.02f, 1.18f, frameZ}, {1.02f, 1.18f, frameZ},
            {1.02f, 1.3f, frameZ}, {-1.02f, 1.3f, frameZ},
            backWallNormal, frameColor);

    addQuad(vertices,
            {-1.02f, 3.1f, frameZ}, {1.02f, 3.1f, frameZ},
            {1.02f, 3.22f, frameZ}, {-1.02f, 3.22f, frameZ},
            backWallNormal, frameColor);

    addQuad(vertices,
            {-1.02f, 1.3f, frameZ}, {-0.9f, 1.3f, frameZ},
            {-0.9f, 3.1f, frameZ}, {-1.02f, 3.1f, frameZ},
            backWallNormal, frameColor);

    addQuad(vertices,
            {0.9f, 1.3f, frameZ}, {1.02f, 1.3f, frameZ},
            {1.02f, 3.1f, frameZ}, {0.9f, 3.1f, frameZ},
            backWallNormal, frameColor);

    vertexCount = static_cast<int>(vertices.size());

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
                 vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, normal)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, color)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Room::~Room() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Room::draw() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}
