#version 400 core

in vec3 uv;

out vec3 fragColor;

uniform sampler2D texture0;

const float PI = 3.141592653589793;

void main()
{
	fragColor = texture(texture0, uv.st).xyz;
//	fragColor = uv;
}
