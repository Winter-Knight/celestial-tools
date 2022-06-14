#version 330 core

layout(location = 0) in vec3 pos_modelspace; // includes 2d modelspace (-1.0, 1.0) and size
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec3 center_worldspace;

out vec4 color;
out vec2 uv;

out vec3 output1;
out vec3 output2;
out vec3 output3;

uniform mat4 VP;
uniform vec3 cameraRight;
uniform vec3 cameraUp;

const float PI = 3.14159;

void main()
{
	vec3 look = normalize(center_worldspace);
	vec3 right = normalize(cross(look, vec3(0.0, 1.0, 0.0)));
	vec3 up = cross(right, look);

	vec3 pos_worldspace = center_worldspace +
		right * pos_modelspace.x * pos_modelspace.z +
		up * pos_modelspace.y * pos_modelspace.z;

	float x, y, z;

	x = acos(normalize(pos_worldspace.xz).x);
	if (pos_worldspace.z < 0.0)
		x = 2.0 * PI - x;
	x = (x - PI) / PI;

	y = asin(normalize(pos_worldspace).y);
	y = y * 2.0 / PI;
//	y = normalize(pos_worldspace).y;
	
	gl_Position = vec4(x, y, 0.0, 1.0);
	
	uv = pos_modelspace.xy;
	color = vColor;
//	float l = length(right);
//	color = vec4(pow(l, 5), 1.0 - pow(l, 5), 0.0, 1.0);
	color = vec4(x, 0.0, -x, 1.0);

	output1 = pos_modelspace;
	output2 = center_worldspace;
	output3 = vec3(x, y, 0.0);
}
