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

uniform vec3 lightPos = vec3(0.0, 0.0, 0.0);
uniform vec3 viewPos = vec3(0.0, 0.0, 0.0); //camera pos

in vec3 Normal;
in vec3 FragPos; 
  
in vec2 TexCoord;
uniform sampler2D Texture;

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

    if(texture(Texture, TexCoord).a < 0.1)
        discard;

    FragColor = texture(Texture, TexCoord) * vec4(result, 1.0);
}