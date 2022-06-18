#version 330 core

in vec4 fragColor;
out vec4 color;

void main()
{
	color = fragColor;
//	color = vec4(1.0);
//	color = vec4(vec2(float(fragColor)), 1.0, 1.0);
}
