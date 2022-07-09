#version 330 core

layout(location = 0) in vec2 pos;

out vec2 uv;

uniform mat4 VP;

void main()
{
	gl_Position = VP * vec4(vec3(pos.x, pos.y, 0.0) * 10000, 1.0);
	uv = pos;
}
