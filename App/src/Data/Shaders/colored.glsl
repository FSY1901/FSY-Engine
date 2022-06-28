#type vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord; // Isn't used, but keeps everything standarized

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 fixedNormal;

out vec3 Normal;
out vec3 FragPos;

void main()
{
    FragPos = vec3(transform * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
    Normal = mat3(fixedNormal) * aNormal;
}

#type fragment

#version 330 core
struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

struct Light {  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light; 

out vec4 FragColor;

uniform float Transparency = 1.0;

uniform vec3 lightPos = vec3(0.0, 0.0, 0.0);
uniform vec3 viewPos = vec3(0.0, 0.0, 0.0); //camera pos

in vec3 Normal;
in vec3 FragPos; 

void main()
{
    vec3 ambient = light.ambient * material.diffuse;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
            
    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, Transparency);
}