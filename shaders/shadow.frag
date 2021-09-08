#version 330 core

//in vec2 uv;
in vec3 uv;
in vec3 antinormal;

out vec3 fragColor;

uniform sampler2D texture0;

void main()
{
	vec3 color = texture(texture0, uv.ts).rgb;
	color = pow(color, vec3(3)) * 2.0;

	float cosTheta = dot(antinormal, vec3(1.0, 0.0, 0.0));
	if (cosTheta < 0.0)
		fragColor = vec3(0.0);
	else
		fragColor = color;

}
