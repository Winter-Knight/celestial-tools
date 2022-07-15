#version 330 core

const int numLights = 1;
const float PI = 3.141592653589793;

in vec3 uv;
in vec3 pos_worldspace;
in vec3 normal_worldspace;

out vec4 fragColor;

struct Light {
	vec4 pos;
	vec4 color;
};
uniform Light lights[numLights];
uniform sampler2D texture0;

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
			color += albedo * cosTheta * 4.0 * lights[i].color;
	}

	return color;
}

void main()
{
	vec3 n = normalize(uv);
	
	float vangle = asin(n.y);
	float th = cos(vangle);
	
	if (th == 0)
		th = 0.00001;
	
	float hangle = acos(clamp(n.x / th, -1, 1));
	
	if (n.z >= 0.0)
		hangle = 2 * PI - hangle;

	vec2 frag_uv = vec2(hangle / (2 * PI), (vangle + 0.5 * PI) / PI);
	
	vec4 albedo = texture(texture0, frag_uv);

	fragColor = lighting(vec4(albedo.r * 1.5, albedo.gb / 1.5, 1.0));
}
