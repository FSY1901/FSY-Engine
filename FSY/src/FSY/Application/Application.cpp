#include "Application.h"
#include "../Editor/SceneCameraController.h"
#include "../Editor/Console.h"
#include "Settings.h"
#include "../Editor/Editor.h"
#include "../Rendering/MeshRenderer.h"

#include "../../vendor/stb_image/stb_image.h"
#include <../imgui/imgui.h>
#include <../imgui/imgui_impl_glfw.h>
#include <../imgui/imgui_impl_opengl3.h>
#include <../imgui/imgui_stdlib.h>
#include "../../vendor/imguizmo/ImGuizmo.h"

#include <map>
#include <filesystem>

namespace FSY {

	Application* Application::app = nullptr;

	Application::Application() {

	}

	Application::Application(int width, int height, const char* title)
	{
		m_window.m_width = width;
		m_window.m_height = height;
#ifdef FSY_DEBUG
		inEditor = true;
#else
		inEditor = false;
#endif

		inEditor = true;

		if (this->inEditor)
			m_window.m_title = "FSY Engine";
		else
			m_window.m_title = title;
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
		glfwSetWindowShouldClose(m_window.m_win, 1);
	}

	void Application::ChangeScene(Scene* scene) {
		m_activeScene = scene;
		Scene::activeScene = m_activeScene;
	}

	void Application::SetIcon(const char* filename) {
		GLFWimage images[1];
		images[0].pixels = stbi_load(filename, &images[0].width, &images[0].height, 0, 4);
		glfwSetWindowIcon(m_window.m_win, 1, images);
		stbi_image_free(images[0].pixels);
	}

	void Application::ObjectTransform(GameObject* g) {
		glm::mat4 transform = glm::mat4(1.0f);
		glm::mat4 transMat = glm::translate(transform, glm::vec3(g->position.x, g->position.y, g->position.z));
		glm::quat _rot = glm::quat(g->rotation.w, g->rotation.x, g->rotation.y, g->rotation.z);
		glm::mat4 rotMat = glm::mat4_cast(_rot);
		glm::mat4 scaleMat = glm::scale(transform, glm::vec3(g->scale.x, g->scale.y, g->scale.z));
		transform = transMat * rotMat * scaleMat;
		g->m_transform = transform;
	}

	void Application::CreateNewGameObject(bool asChild, GameObject* parent) {
		GameObject* g;
		if (parent != nullptr)
			g = new GameObject(parent->position, Vector3f(0, 0, 0), Vector3f(1, 1, 1), "New Object");
		else
			g = new GameObject(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1), "New Object");

		if (asChild) {
			parent->AddChild(g);
		}

		m_activeScene->AddObject(g);
	}

	void Application::SetStylingEditScene() {
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
	}

	void Application::SetStylingPlayScene() {
		auto colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.2f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.3f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.4f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.3f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.3f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.4f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.3f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.3f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.4f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.3f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.3f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.5f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.4f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.3f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.3f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.3f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.3f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.3f, 0.1505f, 0.151f, 1.0f };
	}

	float Application::WinWidth() const { return m_window.m_width; }
	float Application::WinHeight() const { return m_window.m_height; }

	Scene* Application::GetActiveScene() {
		return m_activeScene;
	}

	GLFWwindow* Application::__GetWindow() {
		return m_window.m_win;
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
			m_window.m_width = mode->width;
			m_window.m_height = mode->height;
			m_window.m_win = glfwCreateWindow(m_window.m_width, m_window.m_height, m_window.m_title, monitor, NULL);
		}
		else {
			m_window.m_win = glfwCreateWindow(m_window.m_width, m_window.m_height, m_window.m_title, NULL, NULL);
		}

		if (m_window.m_win == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(m_window.m_win);

		Input::__SetWindow(m_window.m_win);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			glfwDestroyWindow(m_window.m_win);
			glfwTerminate();
			return;
		}

		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glViewport(0, 0, m_window.m_width, m_window.m_height);
		glfwSetFramebufferSizeCallback(m_window.m_win, s_framebuffer_size_callback);
		glfwSetCursorPosCallback(m_window.m_win, s_mouse_callback);
		glfwSwapInterval(0);

		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)m_window.m_width / (float)m_window.m_height, 0.01f, 100.0f);

		//Camera Setup
		view = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		m_sceneCamera.__SetViewMatrix(view);

		fbo.Generate(m_window.m_width, m_window.m_height);
		if (inEditor) {
			editorFBO.Generate(m_window.m_width, m_window.m_height);
			cb.LoadTextures();
			m_playButtonTexture = Texture(Settings::s_playButtonPath.c_str());
			m_stopTexture = Texture("./src/Data/Assets/Icons/Pause.png");
		}

		MainLoop();
	}

	void Application::MainLoop() {

		OnStart();

		if (inEditor) {

			Camera::SetAsMain(&m_sceneCamera);

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags = ImGuiConfigFlags_DockingEnable;
			ImGui::StyleColorsDark();
			ImGui_ImplGlfw_InitForOpenGL(m_window.m_win, true);
			ImGui_ImplOpenGL3_Init("#version 330");
			if (Settings::s_editorFontPath != "")
				io.FontDefault = io.Fonts->AddFontFromFileTTF(Settings::s_editorFontPath.c_str(), 16.0f);

			SetStylingEditScene();

		}

		if (inEditor) {
			m_activeScene->state = SceneState::Edit;
			m_sceneCamera.AddComponent<SceneCameraController>();
			scc = m_sceneCamera.GetComponent<SceneCameraController>();
			scc->Start();
		}
		else {
			m_activeScene->state = SceneState::Play;
			for (GameObject* g : m_activeScene->_GetObjects()) {
				for (auto& c : g->__GetComponents()) {
					c->Start();
				}
			}
		}

		fboShader = Shader(Settings::s_fboVertShaderPath.c_str(), Settings::s_fboFragShaderPath.c_str());
		fboShader.Use();
		glUniform1i(glGetUniformLocation(fboShader.ID, "screenTexture"), 0);
		m_sceneCamera.rotation = Vector3f(0, 0, 0);
		while (!glfwWindowShouldClose(m_window.m_win))
		{

			float currentFrame = glfwGetTime();
			Time::s_deltaTime = currentFrame - Time::s_lastframe;
			Time::s_lastframe = currentFrame;

			if (inEditor) {
				if ((m_PanelSize.x != m_texSize.x || m_PanelSize.y != m_texSize.y)) {
					fbo.Resize(m_PanelSize.x, m_PanelSize.y);
					editorFBO.Resize(m_PanelSize.x, m_PanelSize.y);

					m_texSize = m_PanelSize;
					glViewport(0, 0, m_PanelSize.x, m_PanelSize.y);
				}
			}
			else {
				if ((m_window.m_width != m_texSize.x || m_window.m_height != m_texSize.y)) {
					fbo.Resize(m_window.m_width, m_window.m_height);

					m_texSize = ImVec2(m_window.m_width, m_window.m_height);
					glViewport(0, 0, m_window.m_width, m_window.m_height);
				}
			}

			//Preventing a crash when minimizing
			if (m_window.m_width != 0 && m_window.m_height != 0) {

				if (inEditor) {
					ImGui_ImplOpenGL3_NewFrame();
					ImGui_ImplGlfw_NewFrame();
					ImGui::NewFrame();
					ImGuizmo::BeginFrame();
					if(m_activeScene->state == SceneState::Edit)
						scc->Update();
				}

				Camera* cam = Camera::GetMain();
				Quaternion q = cam->rotation;
				glm::vec3 direction(0, 0, -1);
				glm::quat quat = glm::quat(q.w, q.x, q.y, q.z);
				direction = quat * direction;
				glm::vec3 cameraFront = glm::normalize(direction);
				cam->front = { cameraFront.x, cameraFront.y, cameraFront.z };
				cam->right = Vector3f::Normalize(Vector3f::CrossProduct(cam->front, Vector3f(0, 1, 0)));
				cam->up = Vector3f::Normalize(Vector3f::CrossProduct(cam->right, cam->front));
				glm::vec3 cameraPos;
				cameraPos.x = cam->position.x;
				cameraPos.y = cam->position.y;
				cameraPos.z = cam->position.z;
				glm::vec3 Up = { 0.0f, 1.0f, 0.0f };
				view = glm::lookAt(cameraPos, cameraPos + cameraFront, Up);
				cam->__SetViewMatrix(view);
				if (inEditor) {
					float aspect = (float)m_PanelSize.x / m_PanelSize.y;
					projection = cam->mode == CameraMode::Perspective ? glm::perspective(glm::radians(cam->fov), aspect, cam->zNear, cam->zFar) 
						: glm::ortho(-aspect, aspect, -1.0f, 1.0f, cam->zNear, cam->zFar);
				}
				else {
					float aspect = (float)m_window.m_width / m_window.m_height;
					projection = cam->mode == CameraMode::Perspective ? glm::perspective(glm::radians(cam->fov), aspect, cam->zNear, cam->zFar)
						: glm::ortho(-aspect, aspect, -1.0f, 1.0f, cam->zNear, cam->zFar);
				}

				cam->__SetProjectionMatrix(projection);

				fbo.Bind();

				//GLenum attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
				//glDrawBuffers(2, attachments);

				glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glEnable(GL_DEPTH_TEST);

				s_processInput(m_window.m_win);

				if (!inEditor)
					OnUpdate();
				else
					OnEditorUpdate();


				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				//Render active Scene
				if (m_activeScene != nullptr) {

					const Frustum camFrustum = CreateFrustum(glm::vec3(cam->front.x, cam->front.y, cam->front.z),
						glm::vec3(cam->up.x, cam->up.y, cam->up.z),
						glm::vec3(cam->right.x, cam->right.y, cam->right.z),
						glm::vec3(cam->position.x, cam->position.y, cam->position.z),
						(float)m_window.m_width / (float)m_window.m_height, cam->fov, cam->zNear, cam->zFar);

					Camera::GetMain()->__SetFrustum(camFrustum);

					if (!inEditor || m_activeScene->state == SceneState::Play) {
						for (auto g : m_activeScene->_GetObjects()) {
							ObjectTransform(g);
							for (auto& c : g->__GetComponents()) {
								c->Update();
							}
							g->__UpdateChildren();
						}
					}
					else {
						for (auto g : m_activeScene->_GetObjects()) {
							ObjectTransform(g);
							auto renderer = g->GetComponent<MeshRenderer>();
							if (renderer != nullptr) {
								renderer->Update();
							}
						}
					}

					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					glClear(GL_COLOR_BUFFER_BIT);

					if(inEditor)
						editorFBO.Bind();
					
					fboShader.Use();
					fbo.Draw();
					
					if (inEditor) {
						glBindFramebuffer(GL_FRAMEBUFFER, 0);
						glClear(GL_COLOR_BUFFER_BIT);
						FBOTexture = editorFBO.GetTexture();
					}

				}

				if (inEditor) {
					RenderUI();
					ImGui::Render();
					ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
				}
			}

			glfwSwapBuffers(m_window.m_win);
			glfwPollEvents();

		}

		m_window.m_windowOpen = false;

		delete scc;
		//delete vao;
		//vbo.Delete();
		fbo.Delete();
		//Delete Sound
		//Sound::Quit();

		glfwDestroyWindow(m_window.m_win);

		glfwTerminate();
	}

	void Application::RenderUI() {


		//ImGui::ShowDemoWindow();

		static ImGuiWindowFlags winFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		winFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("DockSpace", NULL, winFlags);
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		ImGuiID id = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(id, ImVec2(0, 0));

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Project")) {
				if (ImGui::MenuItem("Save", "Ctrl+S")) {
					Console::Log("Still in work...");
				}
				if (ImGui::BeginMenu("New")) {
					if (ImGui::MenuItem("Folder")) {

					}
					if (ImGui::MenuItem("File")) {
						
					}
					if (ImGui::MenuItem("Scene")) {
						
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			Texture iconTex = m_activeScene->state == SceneState::Edit ? m_playButtonTexture : m_stopTexture;
			float size = ImGui::GetWindowHeight() - 4.0f;
			ImGui::SameLine((m_window.m_width * 0.5f) - (size * 0.5f));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::ImageButton((void*)(intptr_t)iconTex.GetTexture(), ImVec2(size, size))) {
				if (m_activeScene->state == SceneState::Edit) {
					m_activeScene->state = SceneState::Play;
					Camera::SetAsMain(m_activeScene->GetCamera());
					SetStylingPlayScene();
				}
				else if (m_activeScene->state == SceneState::Play) {
					m_activeScene->state = SceneState::Edit;
					Camera::SetAsMain(&m_sceneCamera);
					SetStylingEditScene();
				}
			}
			ImGui::PopStyleColor();
			ImGui::EndMainMenuBar();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Scene");
		ImGui::PopStyleVar();

		ImVec2 size = ImGui::GetContentRegionAvail();
		m_PanelSize = size;
		if (ImGui::IsWindowHovered())
			scc->inSceneWin = true;
		else
			scc->inSceneWin = false;
		ImGui::Image((void*)FBOTexture, size, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		static int op  = - 1;

		if (selectedObject != nullptr && m_activeScene->state == SceneState::Edit) {

			if (Input::GetKey(Keys::Key_G) && Input::GetKey(Keys::Key_LEFT_SHIFT)) {
				op = ImGuizmo::TRANSLATE;
			}
			else if (Input::GetKey(Keys::Key_S) && Input::GetKey(Keys::Key_LEFT_SHIFT)) {
				op = ImGuizmo::SCALE;
			}
			else if (Input::GetKey(Keys::Key_R) && Input::GetKey(Keys::Key_LEFT_SHIFT)) {
				op = ImGuizmo::ROTATE;
			}
			else if (Input::GetKey(Keys::Key_TAB)) {
				op = -1;
			}

			if (op != -1) {
				ImGuizmo::SetOrthographic(true);
				ImGuizmo::SetDrawlist();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

				glm::mat4 viewMat = glm::inverse(m_sceneCamera.GetTransformationMatrix());
				glm::mat4 transform = selectedObject->GetTransformationMatrix();
				
				ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection * viewMat), (ImGuizmo::OPERATION)op, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform));

				if (ImGuizmo::IsUsing()) {
					glm::vec3 pos, rot, scale;
					DecomposeTransform(transform, pos, rot, scale);
					selectedObject->position = Vector3f(pos.x, pos.y, pos.z);
					/*Vector3f deltaRotation = Vector3f(RadiansToDegrees(rot.x), RadiansToDegrees(rot.y), RadiansToDegrees(rot.z)) - selectedObject->rotation;
					selectedObject->rotation += deltaRotation;*/
					Vector3f deltaRotation = Vector3f(RadiansToDegrees(rot.x), RadiansToDegrees(rot.y), RadiansToDegrees(rot.z)) - selectedObject->rotation;
					Vector3f vec = (Quaternion::ToEulerAngles(selectedObject->rotation) + deltaRotation);
					selectedObject->rotation = vec;
					//Vector3f rot = selectedObject->rotation;
					//Console::Log(std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z));
					selectedObject->scale = Vector3f(scale.x, scale.y, scale.z);
				}
			}
		}

		ImGui::End();
	 	selectedObject = RenderInspector(m_activeScene, selectedObject, m_sceneCamera);
		RenderObjectPanel(selectedObject, m_activeScene);
		RenderContentBrowser(&cb);
		RenderConsolePanel(&m_console);
		RenderDebugPanel();
		ImGui::End();
	}

	void Application::RenderDebugPanel() {
		ImGui::Begin("Debug");
		ImGui::Text("FPS: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "%.1f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
		if (ImGui::TreeNodeEx("Settings", ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed)) {
			ImGui::PushItemWidth(170);
			ImGui::ColorEdit3("Scene Color", m_clearColor);
			ImGui::PopItemWidth();
			ImGui::TreePop();
		}
		ImGui::End();
	}

	void Application::__SetSelectedObject(GameObject* g) {
		selectedObject = g;
	}

	void Application::s_framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		app->m_window.m_width = width;
		app->m_window.m_height = height;
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