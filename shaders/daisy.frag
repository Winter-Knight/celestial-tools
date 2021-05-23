#version 330 core

in vec2 uv;
in vec3 normal_worldspace;
in vec3 sundir_worldspace;

out vec3 fragColor;

uniform sampler2D texture0;

void main()
{
	vec3 color = texture(texture0, uv.ts).rgb;

	float cosTheta = dot(normalize(normal_worldspace), normalize(sundir_worldspace));

	if (cosTheta > 0.25)
		fragColor = color;
//	else if (cosTheta > -0.25)
//		fragColor = color * (cosTheta + 0.25) * 2.0;
	else if (cosTheta > 0.0)
		fragColor = color * cosTheta * 4.0;
	else
		fragColor = vec3(0.0);
}
