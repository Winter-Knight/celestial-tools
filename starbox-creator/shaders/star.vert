#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;

out vec4 fragColor;

uniform mat4 VP;

void main()
{
	gl_Position = VP * vec4(pos, 1.0);
	fragColor = color;
}
