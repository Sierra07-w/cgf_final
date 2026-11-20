#pragma once

#include <glm/glm.hpp>

#include "gfx/Mesh.h"

MeshData createCube(const glm::vec3& size, const glm::vec3& color);
MeshData createSphere(float radius, int segments, int rings, const glm::vec3& color);
MeshData createPlane(float width, float depth, const glm::vec3& color);
MeshData createCylinder(float radius, float height, int segments, const glm::vec3& color);
MeshData createCone(float radius, float height, int segments, const glm::vec3& color);
MeshData createLightBand(const glm::vec3& color);
