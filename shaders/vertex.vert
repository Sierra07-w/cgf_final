#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 textureUV;
out vec3 vertexColor;

void main() {
    vec4 world = model * vec4(aPosition, 1.0);

    worldPosition = world.xyz;
    worldNormal = mat3(transpose(inverse(model))) * aNormal;
    textureUV = aUV;
    vertexColor = aColor;

    gl_Position = projection * view * world;
}
