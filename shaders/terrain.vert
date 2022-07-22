#version 330 core

layout(location = 0) in vec3 p;

out vec3 uv;
out vec3 pos_worldspace;

uniform mat4 VP;

const float terrainWidth = 512.0;

void main()
{
	vec3 pos = p;
	switch(gl_InstanceID) {
		case 0:
			pos.xz += vec2(-terrainWidth); break;
		case 1:
			pos.xz += vec2(0, -terrainWidth); break;
		case 2:
			pos.xz += vec2(terrainWidth, -terrainWidth); break;
		case 3:
			pos.xz += vec2(-terrainWidth, 0); break;
		case 4:
			pos.xz += vec2(0); break;
		case 5:
			pos.xz += vec2(terrainWidth, 0); break;
		case 6:
			pos.xz += vec2(-terrainWidth, terrainWidth); break;
		case 7:
			pos.xz += vec2(0, terrainWidth); break;
		case 8:
			pos.xz += vec2(terrainWidth); break;
	}

	gl_Position = VP * vec4(pos, 1.0);
	uv = pos / vec3(32.0);
	pos_worldspace = pos;
}
