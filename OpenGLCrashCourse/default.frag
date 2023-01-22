#version 330 core
out vec4 FragColor;


in vec3 color;
in vec2 texCoord;
in vec3 Normal;
in vec3 crntPos;

uniform sampler2D tex0;
uniform sampler2D tex1;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight()
{
	vec3 lightVec = lightPos - crntPos;
	float distance = length(lightVec);
	float a = 3.0;
	float b= 0.7;
	float intensity = 1.0/ (a * distance * distance + b * distance + 1.0);

	float ambient = 0.2;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.5;

	vec3 viewDirection = normalize(camPos-crntPos);
	vec3 reflectionDirection = reflect(-lightDirection,normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;
	
	return (texture(tex0, texCoord)  * (diffuse + ambient) + texture(tex1,texCoord).r * specular * intensity) * lightColor;
}

vec4 directLight()
{
	float ambient = 0.2;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0,1.0,0.0));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.5;

	vec3 viewDirection = normalize(camPos-crntPos);
	vec3 reflectionDirection = reflect(-lightDirection,normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;
	
	return (texture(tex0, texCoord)  * (diffuse + ambient) + texture(tex1,texCoord).r * specular) * lightColor;
}


vec4 spotLight()
{
	// in terms of cos and radiants;
	float outerCone = 0.9;
	float innerCone = 0.95;

	float ambient = 0.2;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.5;

	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection,normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;
	
	float angle = dot(vec3(0.0f,-1.0f,0.0f),-lightDirection);
    float intensity = clamp((angle - outerCone) / (innerCone - outerCone),0.0f,1.0f);

	return (texture(tex0, texCoord) * (diffuse * intensity + ambient) + texture(tex1, texCoord).r * specular * intensity) * lightColor;
}

void main()
{
	
	FragColor = spotLight() ;
}