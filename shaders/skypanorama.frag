#version 330 core

#define M_PI 3.14159265359

precision mediump float;

in vec2 uv_interp;

uniform highp mat4 pano_transform;
// uniform highp vec4 asym_proj;
uniform vec4 asym_proj;

uniform sampler2D texture0; //texunit:0
uniform float alpha;

vec4 texturePanorama(vec3 normal, sampler2D pano) {

	vec2 st = vec2(
			atan(normal.x, normal.z),
			acos(normal.y));

	if (st.x < 0.0)
		st.x += M_PI * 2.0;

	st /= vec2(M_PI * 2.0, M_PI);

//	return textureLod(pano, st, 0.0);
	return texture(pano, st);
//	return vec4(st, 0.0, 0.0);
}

layout(location = 0) out vec4 frag_color;

void main() {

	// When an asymmetrical projection matrix is used (applicable for stereoscopic rendering i.e. VR) we need to do this calculation per fragment to get a perspective correct result.
	// Asymmetrical projection means the center of projection is no longer in the center of the screen but shifted.
	// The Matrix[2][0] (= asym_proj.x) and Matrix[2][1] (= asym_proj.z) values are what provide the right shift in the image.

	vec3 cube_normal;
	cube_normal.z = -1.0;
	cube_normal.x = (cube_normal.z * (-uv_interp.x - asym_proj.x)) / asym_proj.y;
	cube_normal.y = (cube_normal.z * (-uv_interp.y - asym_proj.z)) / asym_proj.a;
	cube_normal = mat3(pano_transform) * cube_normal;
	cube_normal.z = -cube_normal.z;

	frag_color = vec4(texturePanorama(normalize(cube_normal.xyz), texture0).rgb, alpha);
}
