#version 330 core

const int numLights = 2;

in vec3 uv;
in vec3 pos_worldspace;
in vec3 normal_worldspace;

out vec3 fragColor;

struct Light {
	vec4 pos;
	vec4 color;
};
uniform Light lights[numLights];
uniform sampler2D texture0;

vec3 lighting(vec3 albedo)
{
	vec3 color = vec3(0.0);

	float cosTheta;
	for (int i = 0; i < numLights; i++) {
		vec3 lightDir_worldspace = lights[i].pos.xyz - pos_worldspace;
		cosTheta = dot(normalize(normal_worldspace), normalize(lightDir_worldspace));

		if (cosTheta > 0.25)
			color += albedo * lights[i].color.rgb;
		else if (cosTheta > 0.0)
			color += albedo * cosTheta * 4.0 * lights[i].color.rgb;
	}

	return color;
}

void main()
{
	fragColor = lighting(texture(texture0, uv.ts).rgb);
}
