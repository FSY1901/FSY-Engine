#include "Light.h"

namespace FSY {

	Light::Light() {

	}

	Light::Light(Vector3f pos, Vector3f color) : GameObject(pos, Vector3f(0, 0, 0), Vector3f(1, 1, 1), "Light Source"){
		m_color = color;
	}

	void Light::__SetLightColorEngine(float col[3]) {
		m_color.x = col[0];
		m_color.y = col[1];
		m_color.z = col[2];
	}

	Vector3f Light::GetLightColor() { return m_color; }

}