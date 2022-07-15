#version 330 core

const float PI = 3.141592653589793;

in vec3 uv;

out vec4 fragColor;

uniform sampler2D texture0;

void main()
{
	vec3 n = normalize(uv);
	
	float vangle = asin(n.y);
	float th = cos(vangle);
	
	if (th == 0)
		th = 0.00001;
	
	float hangle = acos(clamp(n.x / th, -1, 1));
	
	if (n.z >= 0.0)
		hangle = 2 * PI - hangle;

	vec2 frag_uv = vec2(hangle / (2 * PI), (vangle + 0.5 * PI) / PI);
	
	fragColor = texture(texture0, frag_uv);
}
