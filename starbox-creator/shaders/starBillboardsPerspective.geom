#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec3 pos[];
in float size[];
in vec4 color[];

out vec2 uv;
out vec4 fragColor;

uniform vec3 cameraRight;
uniform vec3 cameraUp;

uniform mat4 VP;

void PutCorner(vec2 corner)
{
	vec3 pos_worldspace = pos[0] +
		cameraRight * size[0] * corner.x +
		cameraUp * size[0] * corner.y;
	gl_Position = VP * vec4(pos_worldspace, 1.0);
	
	fragColor = color[0];
	uv = corner;
	EmitVertex();
}

void main()
{
	PutCorner(vec2(-1.0, -1.0));
	PutCorner(vec2( 1.0, -1.0));
	PutCorner(vec2(-1.0,  1.0));
	PutCorner(vec2( 1.0,  1.0));
	
	EndPrimitive();
}
