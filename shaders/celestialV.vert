#version 330 core

layout(location = 0) in vec3 pos_modelspace;

out vec3 antinormal;

out vec3 uv;
out vec3 normal_worldspace;
out vec3 sundir_worldspace;

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
	sundir_worldspace = -vec3(Translation[3][0], Translation[3][1], Translation[3][2]);
	
//	gl_Position = VP * Translation * rotated_pos;
	gl_Position = VP * Translation * vec4(pos, 1.0);

	uv = pos_modelspace;
}
