#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 uv;
in vec3 normal;
in vec3 worldPos;

uniform sampler2D mainTex;
uniform vec3 lightPosition;

void main() {
	// Lighting
	vec3 lightDir = normalize(worldPos - lightPosition);

	float lightValue = max(-dot(normal, lightDir), 0.0f);

	FragColor = vec4(color, 1.0f) * texture(mainTex, uv) * min(lightValue + 0.1, 1.0);
}