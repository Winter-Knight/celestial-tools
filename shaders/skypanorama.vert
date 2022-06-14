#version 330 core

layout(location = 0) in highp vec4 vertex_attrib;
layout(location = 4) in vec2 uv_in;

out vec2 uv_interp;

void main() {
	uv_interp = vertex_attrib.xy;
	gl_Position = vertex_attrib;
}
