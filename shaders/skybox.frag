#version 330 core

in vec3 pos;

out vec3 fragColor;

uniform samplerCube skyboxTexture;

void main()
{
	fragColor = texture(skyboxTexture, pos).rgb;
}
