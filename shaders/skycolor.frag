#version 330 core

in vec2 uv_interp;

out vec4 color;

uniform vec3 skyColor;
uniform float alpha;

void main()
{
	color = vec4(skyColor, alpha);
}
