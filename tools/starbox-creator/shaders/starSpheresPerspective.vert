#version 330 core

layout(location = 0) in vec3 pos_modelspace;
layout(location = 1) in vec3 centerpos_worldspace;
layout(location = 2) in float size;
layout(location = 3) in vec4 color;

out vec4 fragColor;

uniform mat4 VP;

void main()
{
	vec3 pos_worldspace = centerpos_worldspace + pos_modelspace * size;
	gl_Position = VP * vec4(pos_worldspace, 1.0);
	fragColor = color;
}
