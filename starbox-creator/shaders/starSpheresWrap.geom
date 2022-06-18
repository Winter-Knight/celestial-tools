#version 330 core

const int numIndices = 240;
const int numVertices = 42;

layout(points) in;
layout(triangle_strip, max_vertices = 240) out;

in vec3 pos[];
in float size[];
in vec4 color[];

//out vec4 fragColor;

// Texture buffers?
uniform vec3 vertices[numVertices];
uniform uint indices[numIndices];

bool lessthan01 = false;
bool greaterthan09 = false;

const float PI = 3.14159;

vec2 PutVertex(vec3 pos_modelspace)
{
	vec3 pos_worldspace = pos[0] + pos_modelspace * size[0];

	float x, y, z;

	x = acos(normalize(pos_worldspace.xz).x);
	if (pos_worldspace.z < 0.0)
		x = 2.0 * PI - x;
	x = (x - PI) / PI;

	y = asin(normalize(pos_worldspace).y);
	y = y * 2.0 / PI;

	if (x < 0.1)
		lessthan01 = true;
	if (x > 0.9)
		greaterthan09 = true;

	return vec2(x, y);
}

void main()
{
	vec2 outputVertices[numIndices];
	for (int i = 0; i < numIndices; i++) {
		outputVertices[i] = PutVertex(vertices[indices[i]]);
	}

	if (!(lessthan01 && greaterthan09))
		for (int i = 0; i < numIndices; i++) {
			gl_Position = vec4(outputVertices[i], 0.0, 1.0);
//			fragColor = color[0];
			EmitVertex();
			if (i % 3 == 2)
				EndPrimitive();
		}
}
