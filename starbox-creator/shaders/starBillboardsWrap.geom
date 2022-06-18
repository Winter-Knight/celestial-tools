#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec3 pos[];
in float size[];
in vec4 color[];

out vec2 uv;
out vec4 fragColor;

const float PI = 3.14159;

void PutCorner(vec2 corner)
{
	vec3 look = normalize(pos[0]);
	vec3 right = normalize(cross(look, vec3(0.0, 1.0, 0.0)));
	vec3 up = cross(right, look);

	vec3 pos_worldspace = pos[0] +
		right * corner.x * size[0] +
		up * corner.y * size[0];

	float x, y;

	x = acos(normalize(pos_worldspace.xz).x);
	if (pos_worldspace.z < 0.0)
		x = 2.0 * PI - x;
	x = (x - PI) / PI;

	y = asin(normalize(pos_worldspace).y);
	y = y * 2.0 / PI;
	
	gl_Position = vec4(x, y, 0.0, 1.0);
	
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
