#ifndef BLUEBOXMATHS_H
#define BLUEBOXMATHS_H

#include <iostream>
#include <cmath>
#include "../Core/Core.h"
#include "../../vendor/glm/glm.hpp"
#include "../../vendor/glm/gtc/matrix_transform.hpp"
#include "../../vendor/glm/gtc/type_ptr.hpp"

namespace FSY {

	const double PI = 3.1415926;

	double FSY_API RadiansToDegrees(double radians);

	double FSY_API DegreesToRadians(double degrees);

	template<typename T>
	class FSY_API Vector3 {

	public:
		Vector3();
		Vector3(T X, T Y, T Z);

		T x, y, z;

		T Length();

		//Multiplies 2 Vectors and returns the dot product (a scalar result).
		static T DotProduct(Vector3 v1, Vector3 v2);

		//Calculates the angle between to Vector3s and returns the result in radians.
		static T AngleBetween(Vector3 v1, Vector3 v2);

		static T Distance(Vector3 v1, Vector3 v2);

		//Produces a Vector that is orthogonal to both given Vectors.
		static Vector3 CrossProduct(Vector3 a, Vector3 b);

		//Returns a Vector that has the same direction, but a length of one.
		static Vector3 Normalize(Vector3 v);

#pragma region Operators
		Vector3& operator*=(Vector3& vec);

		Vector3& operator+=(Vector3& vec);

		Vector3& operator+(Vector3& vec);

		Vector3& operator-(Vector3& vec);

		Vector3& operator*(Vector3& vec);

		Vector3& operator*(float f);

		Vector3& operator/(float f);

		bool operator!=(Vector3& other);

		bool operator==(Vector3& other);
#pragma endregion

	};

	FSY_API typedef Vector3<float> Vector3f;

	struct FSY_API Quaternion {
		double w = 1, x = 0, y = 0, z = 0;

		static Quaternion ToQuaternion(Vector3f& vec);
	};

}

#endif // !MATHS_H