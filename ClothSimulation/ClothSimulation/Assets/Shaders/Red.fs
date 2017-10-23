#version 430 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D Texture;

void main()
{
	color = vec4(vec3(0.8f, 0.0f, 0.0f), 1.0f);
}