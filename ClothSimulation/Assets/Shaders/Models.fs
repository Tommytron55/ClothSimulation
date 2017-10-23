#version 430 core

in vec2 TexCoords;
out vec4 color;
uniform sampler2D texture_diffuse1;

//uniform sampler2D Texture;

void main(){
color = vec4(texture(texture_diffuse1, TexCoords)) * vec4(1.0f, 1.0f, 1.0f, 1.0f);
 // color = texture(Texture, TexCoords) * vec4(1.0f, 1.0f, 1.0f, 1.0f);
//color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}