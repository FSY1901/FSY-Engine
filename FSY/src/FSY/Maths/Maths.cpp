#include "Maths.h"

namespace FSY {

#pragma region Angle Conversion
	double RadiansToDegrees(double radians) {
		return radians * 180.0 / PI;
	}

	double DegreesToRadians(double degrees) {
		return degrees * PI / 180.0;
	}
#pragma endregion

#pragma region Decompose Matrix
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
	{
		// From glm::decompose in matrix_decompose.inl & https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Math/Math.cpp

		using namespace glm;
		using T = float;

		mat4 LocalMatrix(transform);

		// Normalize the matrix.
		if (LocalMatrix[3][3] == static_cast<T>(0))
			return false;

		// First, isolate perspective.  This is the messiest.
		if (LocalMatrix[0][3] != static_cast<T>(0) || LocalMatrix[1][3] != static_cast<T>(0) || LocalMatrix[2][3] != static_cast<T>(0))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}

		// Next take care of translation (easy).
		translation = vec3(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

		vec3 Row[3], Pdum3;

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X scale factor and normalize first row.
		scale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<T>(1));
		scale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<T>(1));
		scale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], static_cast<T>(1));

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
#if 0
		Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				scale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}
#endif

		rotation.y = asin(-Row[0][2]);
		if (cos(rotation.y) != 0) {
			rotation.x = atan2(Row[1][2], Row[2][2]);
			rotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else {
			rotation.x = atan2(-Row[2][0], Row[1][1]);
			rotation.z = 0;
		}

		return true;
	}
#pragma endregion


#pragma region Vector(float)
	Vector3f::Vector3() {
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3f::Vector3(float X, float Y, float Z) {
		x = X;
		y = Y;
		z = Z;
	}

	float Vector3f::Length() {
		return sqrtf(x * x + y * y + z * z);
	}

	float Vector3f::DotProduct(Vector3f v1, Vector3f v2) {
		float x = v1.x * v2.x;
		float y = v1.y * v2.y;
		float z = v1.z * v2.z;
		float total = x + y + z;
		return total;
	}

	float Vector3f::AngleBetween(Vector3f v1, Vector3f v2) {
		float x = v1.x * v2.x;
		float y = v1.y * v2.y;
		float z = v1.z * v2.z;
		float total = x + y + z;
		float res = total / (v1.Length() * v2.Length());
		float radians = cosf(total);
		return radians;
	}

	float Vector3f::Distance(Vector3f v1, Vector3f v2) {
		float a = v2.x - v1.x;
		float b = v2.y - v1.y;
		float c = v2.z - v1.z;

		return sqrtf((a * a + b * b + c * c));
	}

	Vector3f Vector3f::Normalize(Vector3f v) {
		Vector3f ret;
		ret.x = v.x / v.Length();
		ret.y = v.y / v.Length();
		ret.z = v.z / v.Length();
		return ret;
	}

	Vector3f Vector3f::CrossProduct(Vector3f a, Vector3f b) {
		Vector3 res;
		res.x = a.y * b.z - a.z * b.y;
		res.y = a.z * b.x - a.x * b.z;
		res.z = a.x * b.y - a.y * b.x;
		return res;
	}

	Vector3f& Vector3f::operator+=(Vector3& vec) {
		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;
		return *this;
	}

	Vector3f& Vector3f::operator*=(Vector3& vec) {
		this->x *= vec.x;
		this->y *= vec.y;
		this->z *= vec.z;
		return *this;
	}

	Vector3f& Vector3f::operator+(Vector3& vec) {
		Vector3f res;
		res.x = this->x + vec.x;
		res.y = this->y + vec.y;
		res.z = this->z + vec.z;
		return res;
	}

	Vector3f& Vector3f::operator-(Vector3& vec) {
		Vector3f res;
		res.x = this->x - vec.x;
		res.y = this->y - vec.y;
		res.z = this->z - vec.z;
		return res;
	}

	Vector3f& Vector3f::operator*(Vector3& vec) {
		Vector3f res;
		res.x = this->x * vec.x;
		res.y = this->y * vec.y;
		res.z = this->z * vec.z;
		return res;
	}

	Vector3f& Vector3f::operator/(float f) {
		Vector3f res;
		res.x = this->x / f;
		res.y = this->y / f;
		res.z = this->z / f;
		return res;
	}

	Vector3f& Vector3f::operator*(float f) {
		Vector3f res;
		res.x = f * this->x;
		res.y = f * this->y;
		res.z = f * this->z;
		return res;
	}

	bool Vector3f::operator!=(Vector3& other) {
		return this->x != other.x || this->y != other.y || this->z != other.z;
	}

	bool Vector3f::operator==(Vector3& other) {
		return this->x == other.x && this->y == other.y && this->z == other.z;
	}
#pragma endregion

#pragma region Quaternions
	Quaternion Quaternion::ToQuaternion(Vector3f& vec) {

		double yaw = DegreesToRadians((double)vec.y);
		double pitch = DegreesToRadians((double)vec.x);
		double roll = DegreesToRadians((double)vec.z);

		double cy = cos(yaw * 0.5);
		double sy = sin(yaw * 0.5);
		double cx = cos(pitch * 0.5);
		double sx = sin(pitch * 0.5);
		double cz = cos(roll * 0.5);
		double sz = sin(roll * 0.5);

		Quaternion q;

		q.w = cz * cx * cy + sx * sy * sz;
		q.x = cz * sx * cy - cx * sz * sy;
		q.y = cz * cx * sy + sz * sx * cy;
		q.z = sz * cx * cy - cz * sx * sy;

		return q;

	}
#pragma endregion

}