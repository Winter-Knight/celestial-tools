#version 330 core

in vec3 uv;
in vec3 antinormal;

out vec3 fragColor;

uniform sampler2D texture0;

const float PI = 3.14159;

void main()
{
/*	vec3 color = texture(texture0, uv).rgb;

	float cosTheta = dot(antinormal, vec3(1.0, 0.0, 0.0));
	if (cosTheta < 0.0)
		fragColor = vec3(0.0);
	else
		fragColor = color;
*/
	float vangle = asin(uv.y);
	float th = cos(vangle);
	float hangle = acos(clamp(uv.x / th, -1, 1));
	
//	fragColor = vec3(hangle / (2 * PI), (vangle + 0.5 * PI) / PI, 0.0);
//	fragColor = vec3(hangle / (2 * PI), 0.0, 0.0);

	float cosTheta = dot(antinormal, vec3(1.0, 0.0, 0.0));
	if (cosTheta < 0.0)
		fragColor = vec3(0.0);
	else
		fragColor = texture(texture0, vec2(hangle / (2 * PI), (vangle + 0.5 * PI) / PI)).rgb;
}
