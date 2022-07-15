#version 330 core

layout(location = 0) in vec3 pos_modelspace;

out vec3 uv;
out vec3 pos_worldspace;
out vec3 normal_worldspace;

uniform mat4 VP;
uniform mat4 Rotations;
uniform mat4 Translation;

uniform vec3 centerPos_worldspace;
uniform float size;

const float PI = 3.14159;

void main()
{
	vec3 pos = pos_modelspace * size;
	vec4 rotated_pos = Rotations * vec4(pos, 1.0);
	normal_worldspace = rotated_pos.xyz;
	
	gl_Position = VP * Translation * rotated_pos;
//	gl_Position = VP * Translation * vec4(pos, 1.0);

	uv = vec3(pos_modelspace.x, -pos_modelspace.y, pos_modelspace.z);
//	uv = pos_modelspace;
	pos_worldspace = vec4(Translation * rotated_pos).xyz;
}
