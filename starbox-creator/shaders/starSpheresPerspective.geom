#version 330 core

//const int numIndices = 240;
//const int numVertices = 42;
//const int numIndices = 60;
//const int numVertices = 12;
const int numIndices = 96;
const int numVertices = 18;

layout(points) in;
layout(triangle_strip, max_vertices = 96) out;

in vec3 pos[];
in float size[];
in vec4 color[];

out vec4 fragColor;

// Texture buffers?
uniform vec3 vertices[numVertices];
uniform uint indices[numIndices];

uniform mat4 VP;

const float PI = 3.14159;

void PutVertex(vec3 pos_modelspace)
{
	vec3 pos_worldspace = pos[0] + pos_modelspace * size[0];

	gl_Position = VP * vec4(pos_worldspace, 1.0);
	fragColor = color[0];
	EmitVertex();
}

void main()
{
	for (int i = 0; i < numIndices; i++) {
		PutVertex(vertices[indices[i]]);
		if (i % 3 == 2)
			EndPrimitive();
	}
}
