#version 330 core

layout(location = 0) in vec3 centerpos_worldspace;
layout(location = 1) in float vSize;
layout(location = 2) in vec4 vColor;

out vec3 pos;
out float size;
out vec4 color;

void main()
{
	pos = centerpos_worldspace;
	size = vSize;
	color = vColor;
}
