#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 fixedNormal;

out vec3 Normal;
out vec3 FragPos;

out vec2 TexCoord;

void main()
{
    FragPos = vec3(transform * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
    Normal = mat3(fixedNormal) * aNormal;
    TexCoord = aTexCoord;
}