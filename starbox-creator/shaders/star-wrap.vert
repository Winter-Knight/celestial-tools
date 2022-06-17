#version 330 core

layout(location = 0) in vec3 pos_modelspace;
layout(location = 1) in vec3 centerpos_worldspace;
layout(location = 2) in float size;
layout(location = 3) in vec4 color;

out vec4 fragColor;

const float PI = 3.14159;

void main()
{
	vec3 pos_worldspace = centerpos_worldspace + pos_modelspace * size;

	float x, y, z;

	x = acos(normalize(pos_worldspace.xz).x);
	if (pos_worldspace.z < 0.0)
		x = 2.0 * PI - x;
	x = (x - PI) / PI;

	y = asin(normalize(pos_worldspace).y);
	y = y * 2.0 / PI;

	gl_Position = vec4(x, y, 0.0, 1.0);

	fragColor = color;
}
