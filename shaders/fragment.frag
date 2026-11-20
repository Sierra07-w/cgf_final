#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 textureUV;
in vec3 vertexColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform DirectionalLight windowLight;
uniform PointLight deskLamp;
uniform PointLight fillLight;
uniform vec3 ambientColor;
uniform vec3 viewPosition;
uniform sampler2D uTexture;
uniform bool uUseTexture;
uniform int uPattern;
uniform vec3 uColorMultiplier;
uniform float uAlpha;

out vec4 fragmentColor;

vec3 applyLight(vec3 normal, vec3 viewDirection, vec3 lightDirection,
                vec3 lightColor, vec3 albedo) {
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);
    vec3 halfway = normalize(lightDirection + viewDirection);
    float specularFactor = pow(max(dot(normal, halfway), 0.0), material.shininess);

    return (diffuseFactor * albedo + specularFactor * material.specular) * lightColor;
}

vec3 applyPointLight(PointLight light, vec3 normal, vec3 viewDirection, vec3 albedo) {
    vec3 toLight = light.position - worldPosition;
    float distanceToLight = length(toLight);
    vec3 lightDirection = toLight / max(distanceToLight, 0.0001);
    float attenuation = 1.0 / (light.constant + light.linear * distanceToLight +
                               light.quadratic * distanceToLight * distanceToLight);
    vec3 lightColor = light.color * light.intensity * attenuation;

    return applyLight(normal, viewDirection, lightDirection, lightColor, albedo);
}

vec3 applyPattern(vec3 base, vec3 position, vec2 uv) {
    if (uPattern == 1) {
        float plankLine = smoothstep(0.96, 1.0, fract(position.z * 2.2));
        float seam = smoothstep(0.985, 1.0, fract(position.x * 0.75));
        return base * (1.0 - 0.22f * plankLine - 0.12f * seam);
    }

    if (uPattern == 2) {
        float stripe = step(0.55, fract(position.x * 2.5));
        return base * mix(0.93, 1.04, stripe);
    }

    if (uPattern == 3) {
        return base * (0.82 + 0.18 * uv.y);
    }

    if (uPattern == 5) {
        vec2 cell = fract(uv * 8.0) - 0.5;
        float dotMask = 1.0 - smoothstep(0.17, 0.21, length(cell));
        return mix(base, vec3(1.0), dotMask);
    }

    return base;
}

void main() {
    vec3 normal = normalize(worldNormal);
    vec3 viewDirection = normalize(viewPosition - worldPosition);
    vec3 albedo = material.diffuse * vertexColor * uColorMultiplier;

    if (uUseTexture) {
        albedo *= texture(uTexture, textureUV * 2.0).rgb;
    }

    albedo = applyPattern(albedo, worldPosition, textureUV);

    vec3 color = ambientColor * material.ambient * albedo;
    color += applyLight(normal, viewDirection, normalize(-windowLight.direction),
                        windowLight.color * windowLight.intensity, albedo);
    color += applyPointLight(deskLamp, normal, viewDirection, albedo);
    color += applyPointLight(fillLight, normal, viewDirection, albedo);

    fragmentColor = vec4(color, uAlpha);
}
