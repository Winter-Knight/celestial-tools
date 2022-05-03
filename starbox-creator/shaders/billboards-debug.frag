#version 410 core

in vec2 uv;
in vec4 color;

in vec3 output1;
in vec3 output2;
in vec3 output3;

layout(location = 0) out vec3 pos;
layout(location = 1) out vec3 UV;
layout(location = 2) out vec3 fragColor;


void main()
{
	if (uv.x * uv.x + uv.y * uv.y > 1.0)
		discard;

	pos = output1;
	UV = output2;
	fragColor = output3;
}
