#version 330 core

in vec2 uv;

out vec4 color;

void main()
{
	color = vec4(normalize(vec3(0.0, uv)), 0.8);
}
