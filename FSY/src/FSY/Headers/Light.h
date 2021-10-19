#ifndef LIGHT_H
#define LIGHT_H

#include "Core.h"
#include "Camera.h"

namespace FSY {

	class FSY_API Light : public GameObject {

	public:
		Light();
		Light(Vector3f pos, Vector3f color);

		//Should only be used by the Editor, use ... instead.
		void __SetLightColorEngine(float col[3]);

		Vector3f GetLightColor();

	private:
		Vector3f m_color;

	};

}

#endif // !LIGHT_H
