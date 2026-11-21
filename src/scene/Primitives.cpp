#include "scene/Primitives.h"

#include <cmath>

namespace {

void addFace(MeshData& data,
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

MeshData createCube(const glm::vec3& size, const glm::vec3& color) {
    MeshData data;
    const glm::vec3 h = size * 0.5f;

    addFace(data, {h.x, -h.y, -h.z}, {h.x, h.y, -h.z},
            {h.x, h.y, h.z}, {h.x, -h.y, h.z}, {1.0f, 0.0f, 0.0f}, color);

    addFace(data, {-h.x, -h.y, h.z}, {-h.x, h.y, h.z},
            {-h.x, h.y, -h.z}, {-h.x, -h.y, -h.z}, {-1.0f, 0.0f, 0.0f}, color);

    addFace(data, {-h.x, h.y, h.z}, {h.x, h.y, h.z},
            {h.x, h.y, -h.z}, {-h.x, h.y, -h.z}, {0.0f, 1.0f, 0.0f}, color);

    addFace(data, {-h.x, -h.y, -h.z}, {h.x, -h.y, -h.z},
            {h.x, -h.y, h.z}, {-h.x, -h.y, h.z}, {0.0f, -1.0f, 0.0f}, color);

    addFace(data, {-h.x, -h.y, h.z}, {h.x, -h.y, h.z},
            {h.x, h.y, h.z}, {-h.x, h.y, h.z}, {0.0f, 0.0f, 1.0f}, color);

    addFace(data, {h.x, -h.y, -h.z}, {-h.x, -h.y, -h.z},
            {-h.x, h.y, -h.z}, {h.x, h.y, -h.z}, {0.0f, 0.0f, -1.0f}, color);

    return data;
}

MeshData createSphere(float radius, int segments, int rings, const glm::vec3& color) {
    MeshData data;

    for (int ring = 0; ring <= rings; ++ring) {
        const float theta = 3.14159265f * static_cast<float>(ring) / static_cast<float>(rings);
        const float sinTheta = std::sin(theta);
        const float cosTheta = std::cos(theta);

        for (int segment = 0; segment <= segments; ++segment) {
            const float phi = 6.2831853f * static_cast<float>(segment) /
                              static_cast<float>(segments);
            const glm::vec3 normal(sinTheta * std::cos(phi), cosTheta,
                                   sinTheta * std::sin(phi));

            data.vertices.push_back({normal * radius, normal,
                                     {static_cast<float>(segment) / segments,
                                      static_cast<float>(ring) / rings},
                                     color});
        }
    }

    const int stride = segments + 1;
    for (int ring = 0; ring < rings; ++ring) {
        for (int segment = 0; segment < segments; ++segment) {
            const unsigned int a = static_cast<unsigned int>(ring * stride + segment);
            const unsigned int b = a + static_cast<unsigned int>(stride);
            data.indices.push_back(a);
            data.indices.push_back(b);
            data.indices.push_back(a + 1);
            data.indices.push_back(b);
            data.indices.push_back(b + 1);
            data.indices.push_back(a + 1);
        }
    }

    return data;
}

MeshData createPlane(float width, float depth, const glm::vec3& color) {
    MeshData data;
    const float w = width * 0.5f;
    const float d = depth * 0.5f;
    const glm::vec3 normal(0.0f, 1.0f, 0.0f);
    const unsigned int base = 0;

    data.vertices.push_back({{-w, 0.0f, d}, normal, {0.0f, 1.0f}, color});
    data.vertices.push_back({{w, 0.0f, d}, normal, {1.0f, 1.0f}, color});
    data.vertices.push_back({{w, 0.0f, -d}, normal, {1.0f, 0.0f}, color});
    data.vertices.push_back({{-w, 0.0f, -d}, normal, {0.0f, 0.0f}, color});

    data.indices.push_back(base);
    data.indices.push_back(base + 1);
    data.indices.push_back(base + 2);
    data.indices.push_back(base);
    data.indices.push_back(base + 2);
    data.indices.push_back(base + 3);

    return data;
}

MeshData createCylinder(float radius, float height, int segments, const glm::vec3& color) {
    MeshData data;
    const float halfHeight = height * 0.5f;

    for (int segment = 0; segment <= segments; ++segment) {
        const float phi = 6.2831853f * static_cast<float>(segment) / static_cast<float>(segments);
        const glm::vec3 normal(std::cos(phi), 0.0f, std::sin(phi));
        const glm::vec3 position = normal * radius;

        data.vertices.push_back({{position.x, -halfHeight, position.z}, normal,
                                 {static_cast<float>(segment) / segments, 0.0f}, color});
        data.vertices.push_back({{position.x, halfHeight, position.z}, normal,
                                 {static_cast<float>(segment) / segments, 1.0f}, color});
    }

    for (int segment = 0; segment < segments; ++segment) {
        const unsigned int bottom = static_cast<unsigned int>(segment * 2);
        data.indices.push_back(bottom);
        data.indices.push_back(bottom + 2);
        data.indices.push_back(bottom + 3);
        data.indices.push_back(bottom);
        data.indices.push_back(bottom + 3);
        data.indices.push_back(bottom + 1);
    }

    const unsigned int topCenter = static_cast<unsigned int>(data.vertices.size());
    data.vertices.push_back({{0.0f, halfHeight, 0.0f}, {0.0f, 1.0f, 0.0f},
                             {0.5f, 0.5f}, color});
    const unsigned int bottomCenter = static_cast<unsigned int>(data.vertices.size());
    data.vertices.push_back({{0.0f, -halfHeight, 0.0f}, {0.0f, -1.0f, 0.0f},
                             {0.5f, 0.5f}, color});

    for (int segment = 0; segment < segments; ++segment) {
        const unsigned int top = static_cast<unsigned int>(segment * 2 + 1);
        data.indices.push_back(topCenter);
        data.indices.push_back(top);
        data.indices.push_back(top + 2);

        const unsigned int bottom = static_cast<unsigned int>(segment * 2);
        data.indices.push_back(bottomCenter);
        data.indices.push_back(bottom + 2);
        data.indices.push_back(bottom);
    }

    return data;
}

MeshData createCone(float radius, float height, int segments, const glm::vec3& color) {
    MeshData data;
    const float halfHeight = height * 0.5f;
    const glm::vec3 apex(0.0f, halfHeight, 0.0f);

    for (int segment = 0; segment < segments; ++segment) {
        const float phi0 = 6.2831853f * static_cast<float>(segment) / static_cast<float>(segments);
        const float phi1 = 6.2831853f * static_cast<float>(segment + 1) / static_cast<float>(segments);
        const glm::vec3 normal0 = glm::normalize(glm::vec3(std::cos(phi0) * height,
                                                           radius,
                                                           std::sin(phi0) * height));
        const glm::vec3 normal1 = glm::normalize(glm::vec3(std::cos(phi1) * height,
                                                           radius,
                                                           std::sin(phi1) * height));
        const glm::vec3 base0(std::cos(phi0) * radius, -halfHeight, std::sin(phi0) * radius);
        const glm::vec3 base1(std::cos(phi1) * radius, -halfHeight, std::sin(phi1) * radius);

        const unsigned int first = static_cast<unsigned int>(data.vertices.size());
        data.vertices.push_back({apex, normal0, {0.5f, 1.0f}, color});
        data.vertices.push_back({base0, normal0, {0.0f, 0.0f}, color});
        data.vertices.push_back({base1, normal1, {1.0f, 0.0f}, color});

        data.indices.push_back(first);
        data.indices.push_back(first + 2);
        data.indices.push_back(first + 1);
    }

    const glm::vec3 down(0.0f, -1.0f, 0.0f);
    const unsigned int center = static_cast<unsigned int>(data.vertices.size());
    data.vertices.push_back({{0.0f, -halfHeight, 0.0f}, down, {0.5f, 0.5f}, color});

    for (int segment = 0; segment <= segments; ++segment) {
        const float phi = 6.2831853f * static_cast<float>(segment) / static_cast<float>(segments);
        data.vertices.push_back({{std::cos(phi) * radius, -halfHeight, std::sin(phi) * radius},
                                 down,
                                 {0.5f + 0.5f * std::cos(phi),
                                  0.5f + 0.5f * std::sin(phi)},
                                 color});
    }

    for (int segment = 0; segment < segments; ++segment) {
        data.indices.push_back(center);
        data.indices.push_back(center + segment + 1);
        data.indices.push_back(center + segment + 2);
    }

    return data;
}

MeshData createLightBand(const glm::vec3& color) {
    MeshData data;

    const float topHalfWidth = 0.35f;
    const float topHalfDepth = 0.12f;
    const float bottomHalfWidth = 0.50f;
    const float bottomHalfDepth = 0.18f;
    const glm::vec3 dim = color * 0.35f;
    const glm::vec3 frontNormal(0.0f, 0.0f, 1.0f);
    const glm::vec3 backNormal(0.0f, 0.0f, -1.0f);

    data.vertices.push_back({{-topHalfWidth, 0.5f, topHalfDepth}, frontNormal, {0.0f, 1.0f}, color});
    data.vertices.push_back({{ topHalfWidth, 0.5f, topHalfDepth}, frontNormal, {1.0f, 1.0f}, color});
    data.vertices.push_back({{ topHalfWidth, 0.5f, -topHalfDepth}, backNormal, {1.0f, 1.0f}, color});
    data.vertices.push_back({{-topHalfWidth, 0.5f, -topHalfDepth}, backNormal, {0.0f, 1.0f}, color});

    data.vertices.push_back({{-bottomHalfWidth, -0.5f, bottomHalfDepth}, frontNormal, {0.0f, 0.0f}, dim});
    data.vertices.push_back({{ bottomHalfWidth, -0.5f, bottomHalfDepth}, frontNormal, {1.0f, 0.0f}, dim});
    data.vertices.push_back({{ bottomHalfWidth, -0.5f, -bottomHalfDepth}, backNormal, {1.0f, 0.0f}, dim});
    data.vertices.push_back({{-bottomHalfWidth, -0.5f, -bottomHalfDepth}, backNormal, {0.0f, 0.0f}, dim});
    data.vertices.push_back({{0.0f, -0.5f, 0.0f}, backNormal, {0.5f, 0.5f}, color});

    data.indices.push_back(0); data.indices.push_back(1); data.indices.push_back(5);
    data.indices.push_back(0); data.indices.push_back(5); data.indices.push_back(4);

    data.indices.push_back(1); data.indices.push_back(2); data.indices.push_back(6);
    data.indices.push_back(1); data.indices.push_back(6); data.indices.push_back(5);

    data.indices.push_back(2); data.indices.push_back(3); data.indices.push_back(7);
    data.indices.push_back(2); data.indices.push_back(7); data.indices.push_back(6);

    data.indices.push_back(3); data.indices.push_back(0); data.indices.push_back(4);
    data.indices.push_back(3); data.indices.push_back(4); data.indices.push_back(7);

    data.indices.push_back(0); data.indices.push_back(1); data.indices.push_back(2);
    data.indices.push_back(0); data.indices.push_back(2); data.indices.push_back(3);

    data.indices.push_back(4); data.indices.push_back(5); data.indices.push_back(8);
    data.indices.push_back(5); data.indices.push_back(6); data.indices.push_back(8);
    data.indices.push_back(6); data.indices.push_back(7); data.indices.push_back(8);
    data.indices.push_back(7); data.indices.push_back(4); data.indices.push_back(8);

    return data;
}
