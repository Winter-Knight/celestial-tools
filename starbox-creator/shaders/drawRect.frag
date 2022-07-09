#version 330 core

in vec2 uv;

out vec3 color;

uniform sampler2D texture0;

void main()
{
	color = texture(texture0, uv).rgb;
}
