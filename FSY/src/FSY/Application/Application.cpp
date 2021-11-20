#include "Application.h"
#include "../Editor/SceneCameraController.h"
#include "../Editor/Console.h"
#include "../Sound/Sound.h"
#include "Settings.h"

#include "../../vendor/stb_image/stb_image.h"
#include <../imgui/imgui.h>
#include <../imgui/imgui_impl_glfw.h>
#include <../imgui/imgui_impl_opengl3.h>
#include <../imgui/imgui_stdlib.h>

#include <map>
#include <filesystem>

namespace FSY {

	Application* Application::app = nullptr;

	Application::Application() {

	}

	Application::Application(int width, int height, const char* title)
		: m_width(width), m_height(height), m_title(title)
	{
		app = this;
	}

	Application::~Application() {

	}

	Application* Application::GetInstance() {
		return app;
	}

	void Application::Run() {
		Init();
	}

	void Application::Close() {
		glfwSetWindowShouldClose(m_win, 1);
	}

	void Application::ChangeScene(Scene* scene) {
		m_activeScene = scene;
	}

	void Application::SetIcon(const char* filename) {
		GLFWimage images[1];
		images[0].pixels = stbi_load(filename, &images[0].width, &images[0].height, 0, 4);
		glfwSetWindowIcon(m_win, 1, images);
		stbi_image_free(images[0].pixels);
	}

	float Application::WinWidth() { return m_width; }
	float Application::WinHeight() { return m_height; }

	Scene* Application::GetActiveScene() {
		return m_activeScene;
	}

	GLFWwindow* Application::__GetWindow() {
		return m_win;
	}

	void Application::Init() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		if (Settings::s_fullscreen) {
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			m_width = mode->width;
			m_height = mode->height;
			m_win = glfwCreateWindow(m_width, m_height, m_title, monitor, NULL);
		}
		else {
			m_win = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
		}

		if (m_win == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(m_win);

		Input::__SetWindow(m_win);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			glfwDestroyWindow(m_win);
			glfwTerminate();
			return;
		}

		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glViewport(0, 0, m_width, m_height);
		glfwSetFramebufferSizeCallback(m_win, s_framebuffer_size_callback);
		glfwSetCursorPosCallback(m_win, s_mouse_callback);
		glfwSwapInterval(0);

		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)m_width / (float)m_height, 0.01f, 100.0f);

		//Camera Setup
		view = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		cb.LoadTextures();

		//Sound Setup
		Sound::CreateSoundEngine();

		MainLoop();
	}

	void Application::MainLoop() {

		OnStart();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(m_win, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		if(Settings::s_editorFontPath != "")
			io.FontDefault = io.Fonts->AddFontFromFileTTF(Settings::s_editorFontPath.c_str(), 16.0f);

#pragma region Styling
		auto colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
#pragma endregion

		vao->Generate();
		vao->Bind();

		if (inEditor) {
			m_activeScene->GetCamera()->AddComponent<SceneCameraController>();
			scc = m_activeScene->GetCamera()->GetComponent<SceneCameraController>();
			scc->Start();
		}
		else {
			for (GameObject* g : m_activeScene->_GetObjects()) {
				for (Component* c : g->__GetComponents()) {
					c->Start();
				}
			}
		}

		while (!glfwWindowShouldClose(m_win))
		{

			//Preventing a crash when minimizing
			if (m_width != 0 && m_height != 0) {

				if (inEditor) {
					ImGui_ImplOpenGL3_NewFrame();
					ImGui_ImplGlfw_NewFrame();
					ImGui::NewFrame();
					scc->Update();
				}

				glm::vec3 direction;
				direction.x = DegreesToRadians(m_activeScene->GetCamera()->rotation.x);
				direction.y = DegreesToRadians(m_activeScene->GetCamera()->rotation.y);
				direction.z = DegreesToRadians(m_activeScene->GetCamera()->rotation.z);
				glm::vec3 cameraFront = glm::normalize(direction);
				glm::vec3 cameraPos;
				cameraPos.x = m_activeScene->GetCamera()->position.x;
				cameraPos.y = m_activeScene->GetCamera()->position.y;
				cameraPos.z = m_activeScene->GetCamera()->position.z;
				glm::vec3 Up = { 0.0f, 1.0f, 0.0f };
				view = glm::lookAt(cameraPos, cameraPos + cameraFront, Up);

				projection = glm::perspective(glm::radians(45.0f), (float)m_width / (float)m_height, 0.1f, 100.0f);

				glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				s_processInput(m_win);

				if (!inEditor)
					OnUpdate();
				else
					OnEditorUpdate();

				//Render active Scene
				if (m_activeScene != nullptr) {

					Camera* cam = Camera::GetMain();
					Vector3f vec1 = { cam->position.x, cam->position.y, cam->position.z };
					float inView = cos(DegreesToRadians(90 * 0.8));
					Sound::GetSoundEngine()->setListenerPosition(irrklang::vec3df(vec1.x, vec1.y, vec1.z), irrklang::vec3df(-cameraFront.x, cameraFront.y, -cameraFront.z));
					Sound::listenerPos = cam->position;
					for (auto mesh : m_activeScene->_GetMeshes()) {
						if (mesh->renderMode == RENDER_FRONT) {
							glEnable(GL_CULL_FACE);
							glCullFace(GL_BACK);
						}
						VBO vbo;
						vbo.Bind();
						vbo.SetData(mesh->GetVertices(), sizeof(float) * mesh->GetVertexSize());
						vao->Link(&vbo, 0, 1);
						vbo.Unbind();
						mesh->GetShader()->Use();
						if (mesh->HasTexture())
							mesh->GetTexture()->Bind();
						mesh->GetShader()->setMat4("view", view);
						mesh->GetShader()->setMat4("projection", projection);
						mesh->GetShader()->setColorValues3("lightColor", m_activeScene->GetLight()->GetLightColor().x, m_activeScene->GetLight()->GetLightColor().y,
							m_activeScene->GetLight()->GetLightColor().z);
						mesh->GetShader()->setVec3("lightPos", m_activeScene->GetLight()->position.x, m_activeScene->GetLight()->position.y, m_activeScene->GetLight()->position.z);
						mesh->GetShader()->setVec3("viewPos", Camera::GetMain()->position.x, Camera::GetMain()->position.y, Camera::GetMain()->position.z);

						if (mesh->isTransparent) {
							glEnable(GL_BLEND);
							std::map<float, glm::vec3> sorted;
							std::map<float, GameObject*> sorted1;
							for (unsigned int i = 0; i < mesh->_GetGameObjects().size(); i++)
							{
								glm::vec3 pos(mesh->_GetGameObjects()[i]->position.x, mesh->_GetGameObjects()[i]->position.y, mesh->_GetGameObjects()[i]->position.z);
								float distance = glm::length(cameraPos - pos);
								if (sorted1[distance] == nullptr) {
									sorted[distance] = pos;
									sorted1[distance] = mesh->_GetGameObjects()[i];
								}
								else {
									//This step is required because otherwise Meshes that have the same position won't be rendered
									distance += 0.00001f;
									sorted[distance] = pos;
									sorted1[distance] = mesh->_GetGameObjects()[i];
								}
							}
							for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
							{
								GameObject* g = sorted1[it->first];
								g->__UpdateChildren();
								Vector3f vec2 = g->position;
								Vector3f res = vec2 - vec1;
								res = Vector3f::Normalize(res);
								Vector3f dir = { cameraFront.x, cameraFront.y, cameraFront.z };
								float f_dp = Vector3f::DotProduct(dir, res);
								if (f_dp >= inView) {
									glm::mat4 transform = glm::mat4(1.0f);
									glm::mat4 transMat = glm::translate(transform, it->second);
									glm::vec3 rot(DegreesToRadians(g->rotation.x), DegreesToRadians(g->rotation.y), DegreesToRadians(g->rotation.z));
									glm::quat _rot = glm::quat(rot);
									glm::mat4 rotMat = glm::mat4_cast(_rot);
									//glm::sin(rot);
									glm::mat4 scaleMat = glm::scale(transform, glm::vec3(g->scale.x, g->scale.y, g->scale.z));
									transform = transMat * rotMat * scaleMat;
									g->transform = transform;
									glm::mat4 inverse = glm::inverse(transform);
									glm::mat4 transpose = glm::transpose(inverse);
									g->fixedNormal = transpose;
									mesh->GetShader()->setMat4("fixedNormal", g->fixedNormal);
									mesh->GetShader()->setMat4("transform", g->transform);
									glDrawArrays(GL_TRIANGLES, 0, sizeof(float) * mesh->GetVertexSize());
									vbo.Unbind();
									if (!inEditor) {
										for (Component* c : g->__GetComponents()) {
											c->Update();
										}
									}
								}
							}
							glDisable(GL_BLEND);
							vbo.Delete();
						}
						else {
							for (auto g : mesh->_GetGameObjects()) {
								g->__UpdateChildren();
								Vector3f vec2 = g->position;
								Vector3f res = vec2 - vec1;
								res = Vector3f::Normalize(res);
								Vector3f dir = { cameraFront.x, cameraFront.y, cameraFront.z };
								float f_dp = Vector3f::DotProduct(dir, res);
								if (f_dp >= inView) {
									glm::mat4 transform = glm::mat4(1.0f);
									glm::vec3 rot(DegreesToRadians(g->rotation.x), DegreesToRadians(g->rotation.y), DegreesToRadians(g->rotation.z));
									glm::quat _rot = glm::quat(rot);
									glm::mat4 rotMat = glm::mat4_cast(_rot);
									//glm::sin(rot);
									glm::mat4 transMat = glm::translate(transform, glm::vec3(g->position.x, g->position.y, g->position.z));
									glm::mat4 scaleMat = glm::scale(transform, glm::vec3(g->scale.x, g->scale.y, g->scale.z));
									transform = transMat * rotMat * scaleMat;
									g->transform = transform;
									glm::mat4 inverse = glm::inverse(transform);
									glm::mat4 transpose = glm::transpose(inverse);
									g->fixedNormal = transpose;
									mesh->GetShader()->setMat4("fixedNormal", g->fixedNormal);
									mesh->GetShader()->setMat4("transform", g->transform);
									glDrawArrays(GL_TRIANGLES, 0, sizeof(float) * mesh->GetVertexSize());
									vbo.Unbind();
									if (!inEditor) {
										for (Component* c : g->__GetComponents()) {
											c->Update();
										}
									}
								}
							}
						}
						glDisable(GL_CULL_FACE);
						vbo.Delete();
					}
					//update all none mesh objects
					if (!inEditor) {
						for (auto g : m_activeScene->_GetObjects()) {
							if (!g->HasMesh()) {
								g->__UpdateChildren();
								for (Component* c : g->__GetComponents()) {
									c->Update();
								}
							}
						}
					}

				}

				if (inEditor) {
					RenderUI();
					ImGui::Render();
					ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
				}
			}

			glfwSwapBuffers(m_win);
			glfwPollEvents();

			float currentFrame = glfwGetTime();
			Time::s_deltaTime = currentFrame - Time::s_lastframe;
			Time::s_lastframe = currentFrame;

		}

		m_windowOpen = false;

		delete scc;
		delete vao;

		Sound::engine->drop();

		glfwDestroyWindow(m_win);

		glfwTerminate();
	}

	void Application::RenderUI() {

		ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(280, m_height));
		ImGui::BeginTabBar("Inspector");
		if (ImGui::BeginTabItem("Scene")) {
			for (auto g : m_activeScene->_GetObjects()) {
				if (!g->IsChild()) {
					bool node = ImGui::TreeNodeEx(g->name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);
					if (ImGui::IsItemClicked()) {
						selectedObject = g;
					}
					if (node) {
						RenderChildren(g);
						ImGui::TreePop();
					}
				}
			}
			if(ImGui::BeginPopupContextWindow()){
				if (ImGui::MenuItem("Click me!")) {
					std::cout << "CLICK";
				}
				ImGui::EndPopup();
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Object")) {
			if (selectedObject != nullptr) {
				std::string s = selectedObject->name;
				ImGui::InputText("Name", &s);
				if (selectedObject->name != s)
					selectedObject->name = m_activeScene->__CheckName(s, selectedObject);
				if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed)) {
					if (ImGui::TreeNodeEx("Position", ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed)) {
						ImGui::AlignTextToFramePadding();
						ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "X");
						ImGui::SameLine();
						ImGui::PushItemWidth(50);
						ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 0, 0, 255));
						std::string x = std::to_string(selectedObject->position.x);
						ImGui::InputFloat(" ", &selectedObject->position.x, -1000000.0f, 1000000.0f, x.c_str());
						ImGui::PopStyleColor();
						ImGui::AlignTextToFramePadding();
						ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Y");
						ImGui::SameLine();
						ImGui::PushItemWidth(50);
						ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 255, 0, 255));
						std::string y = std::to_string(selectedObject->position.y);
						ImGui::InputFloat("  ", &selectedObject->position.y, -1000000.0f, 1000000.0f, y.c_str());
						ImGui::PopStyleColor();
						ImGui::AlignTextToFramePadding();
						ImGui::TextColored(ImVec4(0.2f, 0.2f, 1.0f, 1.0f), "Z");
						ImGui::SameLine();
						ImGui::PushItemWidth(50);
						ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 255, 255));
						std::string z = std::to_string(selectedObject->position.z);
						ImGui::InputFloat("   ", &selectedObject->position.z, -1000000.0f, 1000000.0f, z.c_str());
						ImGui::PopStyleColor();
						ImGui::TreePop();
					}
					if (ImGui::TreeNodeEx("Rotation", ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed)) {
						ImGui::AlignTextToFramePadding();
						ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "X");
						ImGui::SameLine();
						ImGui::PushItemWidth(50);
						ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 0, 0, 255));
						std::string x = std::to_string(selectedObject->rotation.x);
						ImGui::InputFloat(" ", &selectedObject->rotation.x, 0, 360, x.c_str());
						ImGui::PopStyleColor();
						ImGui::AlignTextToFramePadding();
						ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Y");
						ImGui::SameLine();
						ImGui::PushItemWidth(50);
						ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 255, 0, 255));
						std::string y = std::to_string(selectedObject->rotation.y);
						ImGui::InputFloat("  ", &selectedObject->rotation.y, 0, 360, y.c_str());
						ImGui::PopStyleColor();
						ImGui::AlignTextToFramePadding();
						ImGui::TextColored(ImVec4(0.2f, 0.2f, 1.0f, 1.0f), "Z");
						ImGui::SameLine();
						ImGui::PushItemWidth(50);
						ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 255, 255));
						std::string z = std::to_string(selectedObject->rotation.z);
						ImGui::InputFloat("   ", &selectedObject->rotation.z, -1000000.0f, 1000000.0f, z.c_str());
						ImGui::PopStyleColor();
						ImGui::TreePop();
					}
					if (ImGui::TreeNodeEx("Scale", ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed)) {
						ImGui::AlignTextToFramePadding();
						ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "X");
						ImGui::SameLine();
						ImGui::PushItemWidth(50);
						ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 0, 0, 255));
						std::string x = std::to_string(selectedObject->scale.x);
						ImGui::InputFloat(" ", &selectedObject->scale.x, -1000000.0f, 1000000.0f, x.c_str());
						ImGui::PopStyleColor();
						ImGui::AlignTextToFramePadding();
						ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Y");
						ImGui::SameLine();
						ImGui::PushItemWidth(50);
						ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 255, 0, 255));
						std::string y = std::to_string(selectedObject->scale.y);
						ImGui::InputFloat("  ", &selectedObject->scale.y, -1000000.0f, 1000000.0f, y.c_str());
						ImGui::PopStyleColor();
						ImGui::AlignTextToFramePadding();
						ImGui::TextColored(ImVec4(0.2f, 0.2f, 1.0f, 1.0f), "Z");
						ImGui::SameLine();
						ImGui::PushItemWidth(50);
						ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 255, 255));
						std::string z = std::to_string(selectedObject->scale.z);
						ImGui::InputFloat("   ", &selectedObject->scale.z, -1000000.0f, 1000000.0f, z.c_str());
						ImGui::PopStyleColor();
						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNodeEx("Components", ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed)) {
					for (auto c : selectedObject->__GetComponents()) {
						if(ImGui::TreeNodeEx(c->getName(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed)){
							c->DrawUI();
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Content")) {

			cb.Draw();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Debug")) {
			ImGui::Text("FPS: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "%.1f FPS", ImGui::GetIO().Framerate);
			ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "%.1f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
			if (ImGui::TreeNodeEx("Settings", ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed)) {
				ImGui::PushItemWidth(170);
				ImGui::ColorEdit3("Scene Color", m_clearColor);
				ImGui::PopItemWidth();
				float col[3];
				col[0] = m_activeScene->GetLight()->GetLightColor().x;
				col[1] = m_activeScene->GetLight()->GetLightColor().y;
				col[2] = m_activeScene->GetLight()->GetLightColor().z;
				ImGui::PushItemWidth(170);
				ImGui::ColorEdit3("Light Color", col);
				ImGui::PopItemWidth();
				m_activeScene->GetLight()->__SetLightColorEngine(col);
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("Console", ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed)) {
				if(Console::_GetLatestMessage() != "")
					ImGui::Text(Console::_GetLatestMessage().c_str());
				ImGui::TreePop();
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
		ImGui::End();

	}

	void Application::RenderChildren(GameObject* g) {
		if (g->GetChildren().size() > 0) {
			for (auto g1 : g->GetChildren()) {
				bool node = ImGui::TreeNodeEx(g1->name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow);
				if (ImGui::IsItemClicked()) {
					selectedObject = g1;
				}
				if (node) {
					RenderChildren(g1);
					ImGui::TreePop();
				}
			}
		}
	}

	void Application::s_framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		app->m_width = width;
		app->m_height = height;
	}

	void Application::s_mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		Input::__mx = xpos;
		Input::__my = ypos;
	}

	void Application::s_processInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

}