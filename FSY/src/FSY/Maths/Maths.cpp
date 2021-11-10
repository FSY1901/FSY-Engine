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

	Vector3f& Vector3f::operator*(float& f) {
		Vector3f res;
		res.x = f * this->x;
		res.y = f * this->y;
		res.z = f * this->z;
		return res;
	}

	bool Vector3f::operator!=(Vector3& other) {
		return this->x != other.x || this->y != other.y || this->z != other.z;
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