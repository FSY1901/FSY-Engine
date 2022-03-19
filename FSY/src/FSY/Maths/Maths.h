#ifndef FSYMATHS_H
#define FSYMATHS_H

#include <iostream>
#include <cmath>

#include "../Core/Core.h"

#include "../../vendor/glm/glm.hpp"
#include "../../vendor/glm/gtc/matrix_transform.hpp"
#include "../../vendor/glm/gtc/type_ptr.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "../../vendor/glm/gtx/matrix_decompose.hpp"

namespace FSY {

	extern class GameObject;

	struct FSY_API Quaternion;

	const double PI = 3.1415926;

	double FSY_API RadiansToDegrees(double radians);

	double FSY_API DegreesToRadians(double degrees);

	float FSY_API Clamp(float a, float b, float value);

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

	template<typename T>
	class FSY_API Vector3 {

	public:
		Vector3();
		Vector3(T X, T Y, T Z);
		Vector3(const Quaternion& q);

		T x, y, z;

		T Length();

		//Multiplies 2 Vectors and returns the dot product (a scalar result).
		static T DotProduct(Vector3 v1, Vector3 v2);

		//Calculates the angle between to Vector3s and returns the result in RADIANS.
		static T AngleBetween(Vector3 v1, Vector3 v2);

		static T Distance(Vector3 v1, Vector3 v2);

		//Produces a Vector that is orthogonal to both given Vectors.
		static Vector3 CrossProduct(Vector3 a, Vector3 b);

		//Returns a Vector that has the same direction, but a length of one.
		static Vector3 Normalize(Vector3 v);

#pragma region Operators

		Vector3& operator*=(Vector3 vec);

		Vector3& operator+=(Vector3 vec);

		Vector3& operator+(Vector3 vec);

		Vector3& operator-(Vector3 vec);

		Vector3& operator*(Vector3 vec);

		Vector3& operator*(float f);

		Vector3& operator/(float f);

		bool operator!=(Vector3 other);

		bool operator==(Vector3 other);
#pragma endregion

		static Vector3 up;
		static Vector3 forward;
		static Vector3 right;
		static Vector3 down;
		static Vector3 back;
		static Vector3 left;

	};

	FSY_API typedef Vector3<float> Vector3f;

	struct FSY_API Quaternion {
		double w = 1, x = 0, y = 0, z = 0;

		Quaternion();
		Quaternion(double x, double y, double z, double w);

		static Quaternion ToQuaternion(Vector3f vec);
		static Quaternion LookAt(Vector3f direction, Vector3f forward = Vector3f::forward, Vector3f up = Vector3f::up);
		static Quaternion RotationBetweenVectors(Vector3f forward, Vector3f direction);

		static Vector3f ToEulerAngles(Quaternion q);
		static Vector3f EulerAngles(glm::quat q);

		Quaternion& operator*(Quaternion& q);

		Vector3f& operator*(Vector3f& vec);
	};

}

#endif // !MATHS_H