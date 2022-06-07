#version 330 core

layout (location = 0) out vec4 color;

in vec2 texCoords;

uniform sampler2D screenTexture;

void main(){


	//no effect
	//FragColor = texture(screenTexture, texCoords);
	color = texture(screenTexture, texCoords);

	//inverse effect
	//color = vec4(vec3(1.0 - texture(screenTexture, texCoords)), 1.0);

	//Grayscale
	//color = texture(screenTexture, texCoords);
    //float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    //color = vec4(average, average, average, 1.0);
}