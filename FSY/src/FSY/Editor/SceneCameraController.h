#include "../Input/Input.h"
#include "../Rendering/Camera.h"
#include "../Application/FSY_Time.h"
#include "../Core/Core.h"

#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/imgui_impl_glfw.h"
#include "../../vendor/imgui/imgui_impl_opengl3.h"
#include "../../vendor/imgui/imgui_stdlib.h"

#ifndef SCENECAMERACONTROLLER_H
#define SCENECAMERACONTROLLER_H

namespace FSY {

	class FSY_API SceneCameraController : public Component {

	public:

		float cameraSpeed = 10.0f;
		float sensitivity = 20.0f;

		bool inSceneWin = false;

		SceneCameraController() {
			
		}

		void DrawUI() override {
			ImGui::PushItemWidth(80.0f);
			ImGui::SliderFloat("Sensitivity", &sensitivity, 20.0f, 100.0f);
			ImGui::PopItemWidth();
			ImGui::PushItemWidth(80.0f);
			ImGui::SliderFloat("Speed", &cameraSpeed, 1.0f, 15.0f);
			ImGui::PopItemWidth();
		}

		void Start() override {

		}

		void Update() override {

			cameraPos.x = Camera::GetMain()->position.x;
			cameraPos.y = Camera::GetMain()->position.y;
			cameraPos.z = Camera::GetMain()->position.z;

			if (Input::GetMouse(Buttons::Button_RIGHT) && (inSceneWin || controlling)) {

				controlling = true;

				Input::SetCursorMode(CursorMode::Hidden);

				glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0, 1.0f, 0)));

				if (Input::GetKey(Keys::Key_W))
					cameraPos += Time::deltaTime() * cameraSpeed * cameraFront;
				if (Input::GetKey(Keys::Key_S))
					cameraPos -= Time::deltaTime() * cameraSpeed * cameraFront;
				if (Input::GetKey(Keys::Key_A))
					cameraPos -= cameraRight * cameraSpeed * Time::deltaTime();
				if (Input::GetKey(Keys::Key_D))
					cameraPos += cameraRight * cameraSpeed * Time::deltaTime();

				Camera::GetMain()->position.x = cameraPos.x;
				Camera::GetMain()->position.y = cameraPos.y;
				Camera::GetMain()->position.z = cameraPos.z;

				Vector3f front = Camera::GetMain()->front;
				cameraFront = glm::vec3(front.x, front.y, front.z);

				float xoffset = Input::MouseX() - lastX;
				float yoffset = lastY - Input::MouseY();

				xoffset *= sensitivity * Time::deltaTime();
				yoffset *= sensitivity * Time::deltaTime();

				yaw -= xoffset;
				//Camera::GetMain()->rotation.y = yaw;
				pitch += yoffset;
				//Camera::GetMain()->rotation.x = pitch;
				Vector3f rot(pitch, yaw, 0);
				Camera::GetMain()->rotation = rot;

				if (pitch > 89.0f)
					pitch = 89.0f;
				if (pitch < -89.0f)
					pitch = -89.0f;

				lastX = Input::MouseX();
				lastY = Input::MouseY();

				return;

			}

			controlling = false;

			lastX = Input::MouseX();
			lastY = Input::MouseY();

			Input::SetCursorMode(CursorMode::Shown);

		}

		const char* getName() {
			return "SceneCameraController";
		}

		glm::mat4 viewMatrix() {
			return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		}

	private:
		glm::vec3 cameraPos = glm::vec3(Camera::GetMain()->position.x, Camera::GetMain()->position.y,
			Camera::GetMain()->position.z);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		float yaw = 0.0f;
		float pitch = 0.0f;

		float lastX = Input::WinWidth() / 2.0f;
		float lastY = Input::WinHeight() / 2.0f;

		bool controlling = false;

	};

}
#endif