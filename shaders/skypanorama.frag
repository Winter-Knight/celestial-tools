#version 330 core

/*************************************************************************/
/* Copyright (c) 2022 Brandon Barnes <winterknight@email.com>            */
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#define M_PI 3.14159265359

in vec2 uv;

uniform mat4 pano_transform;
uniform vec4 asym_proj;

uniform sampler2D texture0;
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
	cube_normal.x = (cube_normal.z * (-uv.x - asym_proj.x)) / asym_proj.y;
	cube_normal.y = (cube_normal.z * (-uv.y - asym_proj.z)) / asym_proj.a;
	cube_normal = mat3(pano_transform) * cube_normal;
	cube_normal.z = -cube_normal.z;

	frag_color = vec4(texturePanorama(normalize(cube_normal.xyz), texture0).rgb, alpha);
}
