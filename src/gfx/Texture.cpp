#include "gfx/Texture.h"

#include <array>
#include <cmath>
#include <cstdint>

#include <glad/gl.h>

namespace {

constexpr int textureSize = 128;

std::array<unsigned char, textureSize * textureSize * 4> makeCheckerData() {
    std::array<unsigned char, textureSize * textureSize * 4> data{};

    for (int y = 0; y < textureSize; ++y) {
        for (int x = 0; x < textureSize; ++x) {
            const int cellX = x / 16;
            const int cellY = y / 16;
            const bool dark = (cellX + cellY) % 2 == 0;
            const float edgeX = static_cast<float>(x % 16) / 16.0f;
            const float edgeY = static_cast<float>(y % 16) / 16.0f;
            const float border = (edgeX < 0.06f || edgeX > 0.94f ||
                                  edgeY < 0.06f || edgeY > 0.94f) ? 0.82f : 1.0f;

            const float base = dark ? 0.55f : 0.92f;
            const float value = base * border;

            const size_t offset = (static_cast<size_t>(y) * textureSize + x) * 4;
            data[offset] = static_cast<unsigned char>(value * 255.0f);
            data[offset + 1] = static_cast<unsigned char>(value * 210.0f);
            data[offset + 2] = static_cast<unsigned char>(value * 225.0f);
            data[offset + 3] = 255;
        }
    }

    return data;
}

}

Texture::Texture() {
    const auto data = makeCheckerData();

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize, textureSize, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}
