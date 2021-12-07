#include "../Core/Core.h"

#include "../../vendor/glm/glm.hpp"

#include <iostream>

#ifndef FRUSTUMCULLING_H
#define FRUSTUMCULLING_H

namespace FSY {

	struct Plan {
		glm::vec3 normal = {0, 1, 0};
		float distance = 0; // from origin

		Plan() = default;

		Plan(const glm::vec3& p1, const glm::vec3& norm) : normal(glm::normalize(norm)), distance(glm::dot(normal, p1)) {}

		float getSignedDistanceToPlan(const glm::vec3& point) const {
			return glm::dot(normal, point) - distance;
		}

	};

	struct Frustum {
		Plan top;
		Plan bottom;
		Plan right;
		Plan left;
		Plan _far;
		Plan _near;
	};

	struct BoundingVolume {
		virtual bool isOnFrustum(const Frustum& frustum, const glm::vec3& pos, const glm::vec3& scale) const = 0;
		virtual bool isOnOrForwardPlan(const Plan& plan) const = 0;
	};

	struct Sphere : public BoundingVolume {
		glm::vec3 center = { 0, 0, 0 };
		float radius = 0;

		Sphere(){}

		Sphere(const glm::vec3& inCenter, float inRadius) : BoundingVolume{}, center(inCenter), radius(inRadius) {}

		bool isOnOrForwardPlan(const Plan& plan) const final {
			return plan.getSignedDistanceToPlan(center) > -radius;
		}

		bool isOnFrustum(const Frustum& frustum, const glm::vec3& pos, const glm::vec3& scale) const final{
			
			float maxScale = std::max(std::max(scale.x, scale.y), scale.z);

			Sphere sphere(pos, radius * (maxScale * 0.5f));

			return (sphere.isOnOrForwardPlan(frustum.left) &&
					sphere.isOnOrForwardPlan(frustum.right) &&
					sphere.isOnOrForwardPlan(frustum.top) &&
					sphere.isOnOrForwardPlan(frustum.bottom) &&
					sphere.isOnOrForwardPlan(frustum._near) &&
					sphere.isOnOrForwardPlan(frustum._far));

		}
	};

	inline Frustum CreateFrustum(const glm::vec3 camFront, const glm::vec3 camUp, const glm::vec3 camRight, const glm::vec3 camPos, float aspect, float fovY, float near, float far) {

		Frustum frustum;

		const float halfVSide = far * tanf(fovY * .5f);
		const float halfHSide = halfVSide * aspect;
		const glm::vec3 frontMultFar = far * camFront;

		frustum._near = {camPos + near * camFront, camFront};
		frustum._far = { camPos + frontMultFar, -camFront };
		frustum.right = {camPos, glm::cross(camUp, frontMultFar + camRight * halfHSide)};
		frustum.left = { camPos, glm::cross(frontMultFar - camRight * halfHSide, camUp) };
		frustum.top = {camPos, glm::cross(camRight, frontMultFar - camUp * halfVSide)};
		frustum.bottom = { camPos, glm::cross(frontMultFar + camUp * halfVSide, camRight) };

		return frustum;

	}

}

#endif // !FRUSTUMCULLING_H
