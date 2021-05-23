#version 330 core

in vec2 uv;

out vec3 fragColor;

uniform sampler2D texture0;

void main()
{
	fragColor = vec3(0.0, 0.0, 1.0);
}
