#version 330 core

layout(location = 0) in vec3 pos;

out vec3 color;

uniform mat4 MVP;

uniform float totalVertices;

void main()
{
	gl_Position = MVP * vec4(pos, 1.0);
	color = vec3(gl_VertexID / totalVertices, 0.0, 1.0);
}
