#version 430 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D Text;
uniform vec3 textColor;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(Text, TexCoord).r);
	color = vec4(textColor, 1.0) * sampled;
}