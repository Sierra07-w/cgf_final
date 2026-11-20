#include "scene/Room.h"

namespace {

void addQuad(MeshData& data,
             const glm::vec3& a, const glm::vec3& b,
             const glm::vec3& c, const glm::vec3& d,
             const glm::vec3& normal, const glm::vec3& color) {
    const unsigned int base = static_cast<unsigned int>(data.vertices.size());

    data.vertices.push_back({a, normal, {0.0f, 0.0f}, color});
    data.vertices.push_back({b, normal, {1.0f, 0.0f}, color});
    data.vertices.push_back({c, normal, {1.0f, 1.0f}, color});
    data.vertices.push_back({d, normal, {0.0f, 1.0f}, color});

    data.indices.push_back(base);
    data.indices.push_back(base + 1);
    data.indices.push_back(base + 2);
    data.indices.push_back(base);
    data.indices.push_back(base + 2);
    data.indices.push_back(base + 3);
}

}

MeshData Room::createFloor() {
    MeshData data;
    const glm::vec3 floorColor(0.72f, 0.58f, 0.45f);
    const glm::vec3 normal(0.0f, 1.0f, 0.0f);

    addQuad(data,
            {-3.0f, 0.0f, 3.0f}, {3.0f, 0.0f, 3.0f},
            {3.0f, 0.0f, -3.0f}, {-3.0f, 0.0f, -3.0f},
            normal, floorColor);

    return data;
}

MeshData Room::createWalls() {
    MeshData data;
    const glm::vec3 wallColor(0.93f, 0.91f, 0.86f);
    const glm::vec3 leftNormal(1.0f, 0.0f, 0.0f);
    const glm::vec3 backNormal(0.0f, 0.0f, 1.0f);

    addQuad(data,
            {-3.0f, 0.0f, 3.0f}, {-3.0f, 0.0f, -3.0f},
            {-3.0f, 4.0f, -3.0f}, {-3.0f, 4.0f, 3.0f},
            leftNormal, wallColor);

    addQuad(data,
            {-3.0f, 0.0f, -3.0f}, {3.0f, 0.0f, -3.0f},
            {3.0f, 1.3f, -3.0f}, {-3.0f, 1.3f, -3.0f},
            backNormal, wallColor);

    addQuad(data,
            {-3.0f, 3.1f, -3.0f}, {3.0f, 3.1f, -3.0f},
            {3.0f, 4.0f, -3.0f}, {-3.0f, 4.0f, -3.0f},
            backNormal, wallColor);

    addQuad(data,
            {-3.0f, 1.3f, -3.0f}, {-0.9f, 1.3f, -3.0f},
            {-0.9f, 3.1f, -3.0f}, {-3.0f, 3.1f, -3.0f},
            backNormal, wallColor);

    addQuad(data,
            {0.9f, 1.3f, -3.0f}, {3.0f, 1.3f, -3.0f},
            {3.0f, 3.1f, -3.0f}, {0.9f, 3.1f, -3.0f},
            backNormal, wallColor);

    return data;
}

MeshData Room::createWindowFrame() {
    MeshData data;
    const glm::vec3 frameColor(0.86f, 0.89f, 0.92f);
    const glm::vec3 normal(0.0f, 0.0f, 1.0f);
    const float frameZ = -2.985f;

    addQuad(data,
            {-1.02f, 1.18f, frameZ}, {1.02f, 1.18f, frameZ},
            {1.02f, 1.3f, frameZ}, {-1.02f, 1.3f, frameZ},
            normal, frameColor);

    addQuad(data,
            {-1.02f, 3.1f, frameZ}, {1.02f, 3.1f, frameZ},
            {1.02f, 3.22f, frameZ}, {-1.02f, 3.22f, frameZ},
            normal, frameColor);

    addQuad(data,
            {-1.02f, 1.3f, frameZ}, {-0.9f, 1.3f, frameZ},
            {-0.9f, 3.1f, frameZ}, {-1.02f, 3.1f, frameZ},
            normal, frameColor);

    addQuad(data,
            {0.9f, 1.3f, frameZ}, {1.02f, 1.3f, frameZ},
            {1.02f, 3.1f, frameZ}, {0.9f, 3.1f, frameZ},
            normal, frameColor);

    return data;
}
