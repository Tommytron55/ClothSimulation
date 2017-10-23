#version 430 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 outColor; 

void main(void)
{
	gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0);
        outColor = color;
}