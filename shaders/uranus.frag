#version 330 core

const int numLights = 1;

in vec3 pos_worldspace;
in vec3 normal_worldspace;

out vec4 fragColor;

struct Light {
	vec4 pos;
	vec4 color;
};
uniform Light lights[numLights];

vec4 lighting(vec4 albedo)
{
	vec4 color = vec4(vec3(0.0), 1.0);

	float cosTheta;
	for (int i = 0; i < numLights; i++) {
		vec3 lightDir_worldspace = lights[i].pos.xyz - pos_worldspace;
		cosTheta = dot(normalize(normal_worldspace), normalize(lightDir_worldspace));

		if (cosTheta > 0.25)
			color += albedo * lights[i].color;
		else if (cosTheta > 0.0)
			color += albedo * cosTheta * lights[i].color;
	}

	return color;
}

void main()
{
	fragColor = lighting(vec4(0.0, 0.53, 0.84, 1.0));
}
