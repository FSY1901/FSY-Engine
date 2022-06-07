#include "Maths.h"
#include "../Scene/GameObject.h"

namespace FSY {

#pragma region Angle Conversion
	double RadiansToDegrees(double radians) {
		return radians * 180.0 / PI;
	}

	double DegreesToRadians(double degrees) {
		return degrees * PI / 180.0;
	}
#pragma endregion

	float Clamp(float a, float b, float value) {
		if (value < a)
			return a;
		else if (value > b)
			return b;
		else
			return value;
	}

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

	Vector3f::Vector3(const Quaternion& q) {
		*this = Quaternion::ToEulerAngles(q);
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

	Vector3f Vector3f::Lerp(Vector3 v1, Vector3 v2, float t) {
		t = Clamp(0.0f, 1.0f, t);

		Vector3f res;

		res.x = v1.x + (v2.x - v1.x) * t;
		res.y = v1.y + (v2.y - v1.y) * t;
		res.z = v1.z + (v2.z - v1.z) * t;

		return res;
	}

	Vector3f Vector3f::up = {0, 1, 0};
	Vector3f Vector3f::forward = {0, 0, 1};
	Vector3f Vector3f::right = {1, 0, 0};
	Vector3f Vector3f::down = {0, -1, 0};
	Vector3f Vector3f::back = { 0, 0, -1 };
	Vector3f Vector3f::left = {-1, 0, 0};

	Vector3f& Vector3f::operator+=(Vector3 vec) {
		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;
		return *this;
	}

	Vector3f& Vector3f::operator*=(Vector3 vec) {
		this->x *= vec.x;
		this->y *= vec.y;
		this->z *= vec.z;
		return *this;
	}

	Vector3f& Vector3f::operator+(Vector3 vec) {
		Vector3f res;
		res.x = this->x + vec.x;
		res.y = this->y + vec.y;
		res.z = this->z + vec.z;
		return res;
	}

	Vector3f& Vector3f::operator-(Vector3 vec) {
		Vector3f res;
		res.x = this->x - vec.x;
		res.y = this->y - vec.y;
		res.z = this->z - vec.z;
		return res;
	}

	Vector3f& Vector3f::operator*(Vector3 vec) {
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

	bool Vector3f::operator!=(Vector3 other) {
		return this->x != other.x || this->y != other.y || this->z != other.z;
	}

	bool Vector3f::operator==(Vector3 other) {
		return this->x == other.x && this->y == other.y && this->z == other.z;
	}
#pragma endregion

#pragma region Quaternions

	Quaternion::Quaternion() {

	}

	Quaternion::Quaternion(double x, double y, double z, double w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Quaternion::Quaternion(const Vector3f& vec) {
		Quaternion q = Quaternion::ToQuaternion(vec);
		*this = q;
	}

	Quaternion Quaternion::ToQuaternion(Vector3f vec) {

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
	
	Quaternion Quaternion::LookAt(Vector3f direction, Vector3f forward, Vector3f up) {

		Quaternion rot1 = RotationBetweenVectors(forward, direction);
		Vector3f right = Vector3f::CrossProduct(direction, up);
		up = Vector3f::CrossProduct(right, direction);
		Vector3f realUp(0, 1, 0);
		Vector3f newUp = rot1 * realUp;
		Quaternion rot2 = RotationBetweenVectors(newUp, up);
		Quaternion res = rot2 * rot1;

		return Quaternion(res.x, res.y, res.z, res.w);

	}

	Vector3f Quaternion::EulerAngles(glm::quat q) {
		glm::vec3 res = glm::eulerAngles(q);
		return Vector3f(RadiansToDegrees(res.x), RadiansToDegrees(res.y), RadiansToDegrees(res.z));
	}

	Quaternion Quaternion::RotationBetweenVectors(Vector3f forward, Vector3f direction) {
		forward = Vector3f::Normalize(forward);
		direction = Vector3f::Normalize(direction);

		float cosTheta = Vector3f::DotProduct(forward, direction);
		Vector3f axis;

		if (cosTheta < -1 + 0.001f) {
			// special case when vectors in opposite directions:
			// there is no "ideal" rotation axis
			// So guess one; any will do as long as it's perpendicular to start
			axis = Vector3f::CrossProduct(Vector3f(0.0f, 0.0f, 1.0f), forward);

			if (axis.Length() * axis.Length() < 0.01)
				axis = Vector3f::CrossProduct(Vector3f(1.0f, 0.0f, 0.0f), forward);

			axis = Vector3f::Normalize(axis);
			return Quaternion(axis.x, axis.y, axis.z, DegreesToRadians(0));
		}

		axis = Vector3f::CrossProduct(forward, direction);
		float s = sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;

		return Quaternion(
			axis.x * invs,
			axis.y * invs,
			axis.z * invs,
			s * 0.5f
		);
	}

	float Quaternion::Dot(Quaternion q1, Quaternion q2) {
		float x = q1.x * q2.x;
		float y = q1.y * q2.y;
		float z = q1.z * q2.z;
		float w = q1.w + q2.w;
		float total = x + y + z + w;
		return total;
	}

	Quaternion Quaternion::Slerp(Quaternion q1, Quaternion q2, float delta) {
		delta = Clamp(0.0f, 1.0f, delta);
		glm::quat res = glm::slerp(glm::quat(q1.w, q1.x, q1.y, q1.z), glm::quat(q2.w, q2.x, q2.y, q2.z), delta);
		return Quaternion(res.x, res.y, res.z, res.w);
	}

	Vector3f Quaternion::ToEulerAngles(Quaternion q) {
		Vector3f angles;

		// roll (x-axis rotation)
		double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
		double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
		angles.x = RadiansToDegrees(std::atan2(sinr_cosp, cosr_cosp));

		// pitch (y-axis rotation)
		double sinp = 2 * (q.w * q.y - q.z * q.x);
		if (std::abs(sinp) >= 1)
			angles.y = RadiansToDegrees(std::copysign(PI / 2, sinp)); // use 90 degrees if out of range
		else
			angles.y = RadiansToDegrees(std::asin(sinp));

		// yaw (z-axis rotation)
		double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
		double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
		angles.z = RadiansToDegrees(std::atan2(siny_cosp, cosy_cosp));

		return angles;
	}

	Quaternion& Quaternion::operator*(Quaternion& q) {
		Quaternion res;

		glm::quat q1(this->w, this->x, this->y, this->z);
		glm::quat q2(q.w, q.x, q.y, q.z);

		glm::quat r = q1 * q2;

		res = Quaternion(r.x, r.y, r.z, r.w);

		/*res.x = this->x * q.w + this->y * q.z - this->z * q.y + this->w * q.x;
		res.y = -this->x * q.z + this->y * q.w + this->z * q.x + this->w * q.y;
		res.z = this->x * q.y - this->y * q.x + this->z * q.w + this->w * q.z;
		res.w = -this->x * q.x - this->y * q.y - this->z * q.z + this->w * q.w;*/

		return res;
	}

	Quaternion& Quaternion::operator*(float f) {
		Quaternion res;
		res.x = f * this->x;
		res.y = f * this->y;
		res.z = f * this->z;
		res.w = f * this->w;
		return res;
	}

	Quaternion& Quaternion::operator+(Quaternion& q) {
		Quaternion res;

		res.x = this->x + q.x;
		res.y = this->y + q.y;
		res.z = this->z + q.z;
		res.w = this->w + q.w;

		return res;
	}

	Vector3f& Quaternion::operator*(Vector3f& vec) {
		Vector3f res;
		glm::vec3 v = glm::quat(this->w, this->x, this->y, this->z) * glm::vec3(vec.x, vec.y, vec.z);
		res = Vector3f(v.x, v.y, v.z);
		return res;
	}

	bool Quaternion::operator!=(Quaternion& other) {
		return this->x != other.x || this->y != other.y || this->z != other.z || this->w != other.w;
	}

	bool Quaternion::operator==(Quaternion& other) {
		return this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w;
	}
#pragma endregion

}