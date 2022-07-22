#version 330 core

in vec3 uv;
in vec3 pos_worldspace;

out vec4 color;

uniform sampler2D texture0; // Rocky texture
uniform sampler2D texture1; // Normal texture

uniform vec4 lightDir;
uniform vec4 lightColor;

const float terrainWidth = 512.0;

void main()
{
	vec3 normalDir = normalize(texture(texture1, pos_worldspace.sp / terrainWidth).xzy * 2.0 - 1.0);
	
	float cosTheta = clamp(dot(lightDir.xyz, normalDir), 0.2, 1.0);

	color = texture(texture0, uv.sp) * lightColor * vec4(vec3(cosTheta), 1.0);
}
