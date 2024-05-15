#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 uv;
in vec3 worldPos;
in mat3 tbn;

uniform sampler2D mainTex;
uniform sampler2D normalTex;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main() {

	// Normal map
	vec3 normal = texture(normalTex, uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = tbn * normal;


	// Specular data
	vec3 lightDir = normalize(worldPos - lightPosition);
	vec3 viewDir = normalize(worldPos - cameraPosition);
	vec3 reflDir = normalize(reflect(lightDir, normal));

	// Lighting

	float lightValue = max(-dot(normal, lightDir), 0.0);
	float specular = pow(max(-dot(reflDir, viewDir), 0.0), 60);

	vec4 clrOutput = vec4(color, 1.0f) * texture(mainTex, uv);
	clrOutput.rgb = clrOutput.rgb * min(lightValue + 0.1, 1.0) + specular;

	FragColor = clrOutput;
}