#version 330 core

layout(location = 0) in vec3 Pos_worldspace;
layout(location = 1) in float Size;
layout(location = 2) in float Tilt;

out float size;
out float tilt;

void main()
{
	size = Size;
	tilt = Tilt;
}
