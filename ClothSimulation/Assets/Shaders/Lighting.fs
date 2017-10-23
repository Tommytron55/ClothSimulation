#version 430 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform sampler2D Texture;
uniform vec3 cameraPos;
uniform vec3 lightPos;

vec3 ObjectColour;



void main()
{
	vec3 LightColour =  vec3(1.0f, 1.0f, 0.9f);
	
	vec3 ambient = 0.3f * LightColour;	//Ambient colour of white light
	//vec3 lightPos = vec3(0.0f, 2.0f, -2.0f);			//Position of the light source
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * LightColour;	//Diffuse colour of white light
	
	vec3 viewDir = normalize(cameraPos - FragPos);
	vec3 reflectionDir = reflect(-lightDir, norm);
	
	vec3 H = normalize(lightDir + viewDir);
	vec3 specular = pow(max(dot(norm,H), 0.0), 96) * vec3(1.0f, 1.0f, 1.0f) * 0.5f;
	
//	float spec = pow(max(dot(viewDir, reflectionDir), 0.0f), 96);	//32 is used as "Shininess" power
	//vec3 specular = spec * LightColour * 0.5f;	//Colour of white light, strength of 0.3f
	
	ObjectColour = vec3(1.0f, 1.0f, 1.0f);
	vec3 totalColor = (ambient + diffuse + specular) * ObjectColour;
	
	color = texture(Texture, TexCoord) * vec4(totalColor, 1.0f);
}