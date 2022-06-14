#version 330 core

layout(location = 0) in vec3 pos_modelspace; // includes 2d modelspace (-1.0, 1.0) and size
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec3 center_worldspace;

out vec4 color;
out vec2 uv;

uniform mat4 VP;
uniform vec3 cameraRight;
uniform vec3 cameraUp;

void main()
{
	vec3 pos_worldspace = center_worldspace +
		cameraRight * pos_modelspace.x * pos_modelspace.z +
		cameraUp * pos_modelspace.y * pos_modelspace.z;
	gl_Position = VP * vec4(pos_worldspace, 1.0);
	
	uv = pos_modelspace.xy;
	color = vColor;
}
