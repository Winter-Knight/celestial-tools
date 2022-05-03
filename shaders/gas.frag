#version 400 core

in vec3 uv;

out vec3 fragColor;

uniform sampler2D texture0;

const float PI = 3.141592653589793;

void main()
{
/*	vec3 color = texture(texture0, uv).rgb;

	float cosTheta = dot(antinormal, vec3(1.0, 0.0, 0.0));
	if (cosTheta < 0.0)
		fragColor = vec3(0.0);
	else
		fragColor = color;
*/
	vec3 n = normalize(uv);
//	vec3 n = uv;
	
	float vangle = asin(n.y);
	float th = cos(vangle);
	
	if (th == 0)
		th = 0.00001;
	
	float hangle = acos(clamp(n.x / th, -1, 1));
	
	if (n.z >= 0.0)
		hangle = 2 * PI - hangle;

//	hangle = atan(n.z, n.x);


	vec2 frag_uv = vec2(hangle / (2 * PI), (vangle + 0.5 * PI) / PI);
	
	if (textureQueryLod(texture0, frag_uv).x >= 9.9)
		// Work around weird bug
		if (frag_uv.x < 0.5)
			fragColor = texture(texture0, frag_uv + vec2(0.0001)).rgb;
		else
			fragColor = texture(texture0, frag_uv - vec2(0.0001)).rgb;
	else
		fragColor = texture(texture0, frag_uv).rgb;
}
