#version 330 core

in vec2 uv;
in vec4 color;

out vec4 fragColor;

void main()
{
	if (uv.x * uv.x + uv.y * uv.y > 1.0)
		discard;

	fragColor = color;
}
