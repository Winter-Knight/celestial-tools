#version 330 core

in vec2 uv;
in vec4 fragColor;

layout(location = 0) out vec4 color;

void main()
{
	if (uv.x * uv.x + uv.y * uv.y > 1.0)
		discard;

	color = fragColor;
}
