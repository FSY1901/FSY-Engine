#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;

void main(){


	//no effect
	FragColor = texture(screenTexture, texCoords);


	//inverse effect
	//FragColor = vec4(vec3(1.0 - texture(screenTexture, texCoords)), 1.0);

	//Grayscale
	//FragColor = texture(screenTexture, texCoords);
    //float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    //FragColor = vec4(average, average, average, 1.0);
}