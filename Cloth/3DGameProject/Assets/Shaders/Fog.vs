#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoord;

uniform vec3 colour;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec4 mWorldPos;

void main(void)
{
	vec4 mworldPos = model * vec4(position, 1.0);
	gl_Position = projection * view * mworldPos;
		TexCoord = texCoord;
		FragPos = vec3(model * vec4(position, 1.0f));
		Normal = normal;
		mWorldPos = mworldPos;
}