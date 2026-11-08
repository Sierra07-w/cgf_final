#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;
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

void main() {
    vec3 normal = normalize(worldNormal);
    vec3 viewDirection = normalize(viewPosition - worldPosition);
    vec3 albedo = material.diffuse * vertexColor;

    vec3 color = ambientColor * material.ambient * albedo;
    color += applyLight(normal, viewDirection, normalize(-windowLight.direction),
                        windowLight.color * windowLight.intensity, albedo);
    color += applyPointLight(deskLamp, normal, viewDirection, albedo);
    color += applyPointLight(fillLight, normal, viewDirection, albedo);

    fragmentColor = vec4(color, 1.0);
}
