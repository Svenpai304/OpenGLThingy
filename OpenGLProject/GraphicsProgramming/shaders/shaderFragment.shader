#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 uv;
in vec3 worldPos;
in mat3 tbn;

uniform sampler2D mainTex;
uniform sampler2D normalTex;
uniform vec3 lightPosition;

void main() {

	// Normal map
	vec3 normal = texture(normalTex, uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = tbn * normal;

	// Lighting
	vec3 lightDir = normalize(worldPos - lightPosition);

	float lightValue = max(-dot(normal, lightDir), 0.0f);

	FragColor = vec4(color, 1.0f) * texture(mainTex, uv) * min(lightValue + 0.1, 1.0);
}