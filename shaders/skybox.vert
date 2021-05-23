#version 330

layout(location = 0) in vec3 pos_modelspace;

out vec3 pos;

uniform mat4 MVP;

void main()
{
	pos = pos_modelspace;
	gl_Position = (MVP * vec4(pos, 1.0)).xyww;
}
