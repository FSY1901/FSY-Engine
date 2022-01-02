#ifndef LIGHT_H
#define LIGHT_H

#include "../Core/Core.h"
#include "../Rendering/Camera.h"

namespace FSY {

	class FSY_API Light : public GameObject {

	public:
		Light();
		Light(Vector3f pos, Vector3f color, Vector3f ambient, Vector3f specular);;

		Vector3f color;
		Vector3f ambient;
		Vector3f specular;

	};

}

#endif // !LIGHT_H
