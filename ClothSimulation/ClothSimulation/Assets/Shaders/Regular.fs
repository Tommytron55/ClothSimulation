#version 430 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D Texture;

void main()
{
	color = texture(Texture, TexCoord) * vec4(vec3(1.0f, 1.0f, 1.0f), 1.0f);
}