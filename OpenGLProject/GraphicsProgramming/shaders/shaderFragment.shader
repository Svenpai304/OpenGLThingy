#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 uv;
in vec3 worldPos;
in mat3 tbn;

uniform sampler2D mainTex;
uniform sampler2D normalTex;
uniform vec4 lights[64];
uniform int lightsCount;
uniform vec3 cameraPosition;

void main() {

	// Normal map
	vec3 normal = texture(normalTex, uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = tbn * normal;

	vec3 viewDir = normalize(worldPos - cameraPosition);

	float lightValue = 0;
	float specular = 0;

	for (int i = 0; i < lightsCount; i++) {

		// Specular data
		vec3 lightDir = normalize(worldPos - lights[i].xyz);
		vec3 reflDir = normalize(reflect(lightDir, normal));

		// Lighting
		lightValue += max(-dot(normal, lightDir), 0.0) * lights[i].w;
		specular += pow(max(dot(reflDir, viewDir), 0.0), 64);
	}

	vec4 clrOutput = vec4(color, 1.0f) * texture(mainTex, uv);
	clrOutput.rgb = clrOutput.rgb * min(lightValue + 0.1, 1.0) + specular * clrOutput.rgb;

	FragColor = clrOutput;
}