#version 400 core

in vec3 uv;

layout(location = 0) out vec3 pos;
layout(location = 1) out vec3 frag_uv;
layout(location = 2) out vec3 color;

uniform sampler2D texture0;

const float PI = 3.141592653589793;

float mip_map_level(in vec2 texture_coordinate) // in texel units
{
    vec2  dx_vtc        = dFdx(texture_coordinate);
    vec2  dy_vtc        = dFdy(texture_coordinate);
    float delta_max_sqr = max(dot(dx_vtc, dx_vtc), dot(dy_vtc, dy_vtc));
    float mml = 0.5 * log2(delta_max_sqr);
    return max( 0, mml ); // Thanks @Nims
}

void main()
{
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


	pos = n;
	frag_uv = vec3(hangle / (2 * PI), (vangle + 0.5 * PI) / PI, 0.0);
//	color = texture(texture0, frag_uv.xy).rgb;
	color.xy = textureQueryLod(texture0, frag_uv.xy);
	color.z = mip_map_level(frag_uv.xy * textureSize(texture0, 0));
}
