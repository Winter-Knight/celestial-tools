#version 330 core
#extension GL_ARB_gpu_shader_int64 : enable

const int numLights = 2;
const float PI = 3.141592653589793;

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
	
	float cosTheta;
	vec3 color = vec3(0.0);
	vec3 albedo = texture(texture0, frag_uv).rgb;

	vec3 lightDir_worldspace = lights[0].pos.xyz - pos_worldspace;
	cosTheta = dot(normalize(normal_worldspace), normalize(lightDir_worldspace));

	if (cosTheta > 0.25)
		color = albedo * lights[0].color.rgb;
	else if (cosTheta > 0.0)
		color = albedo * cosTheta * 4.0 * lights[0].color.rgb;
	
	if (cosTheta > 0.25)
		color += albedo * (1.25 - cosTheta) * lights[1].color.rgb;
	if (cosTheta < 0.25)
		color += albedo * lights[1].color.rgb;

	fragColor = color;

	uint64_t t;
}
