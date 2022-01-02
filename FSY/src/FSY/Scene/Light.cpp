#include "Light.h"

namespace FSY {

	Light::Light() {

	}

	Light::Light(Vector3f pos, Vector3f color, Vector3f ambient, Vector3f specular) : GameObject(pos, Vector3f(0, 0, 0), Vector3f(1, 1, 1), "Light Source"){
		this->color = color;
		this->ambient = ambient;
		this->specular = specular;
	}

}