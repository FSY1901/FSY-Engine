#version 330 core
out vec4 FragColor;

uniform vec3 Color = vec3(1.0, 1.0, 1.0);
uniform float Transparency = 10;
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);

uniform vec3 lightPos = vec3(0.0, 0.0, 0.0);
uniform vec3 viewPos = vec3(0.0, 0.0, 0.0); //camera pos

in vec3 Normal;
in vec3 FragPos; 

void main()
{
    // ambient
    float ambientStrength = 0.3;
    float specularStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 
            
    vec3 result = (ambient + diffuse + specular) * Color;
    FragColor = vec4(result, Transparency);
}