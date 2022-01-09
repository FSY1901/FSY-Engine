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
#include "../../vendor/imguizmo/ImGuizmo.h"

#include <map>
#include <filesystem>

namespace FSY {

	Application* Application::app = nullptr;

	Application::Application() {

	}

	Application::Application(int width, int height, const char* title, bool inEditor)
	{
		m_window.m_width = width;
		m_window.m_height = height;
		this->inEditor = inEditor;
		if (inEditor)
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
	}

	void Application::SetIcon(const char* filename) {
		GLFWimage images[1];
		images[0].pixels = stbi_load(filename, &images[0].width, &images[0].height, 0, 4);
		glfwSetWindowIcon(m_window.m_win, 1, images);
		stbi_image_free(images[0].pixels);
	}

	void Application::RenderObject(GameObject* g, Mesh* mesh, bool firstFrame, Frustum camFrustum, bool IsEmpty) {
		if (g->GetBoundingSphere().isOnFrustum(camFrustum,
			glm::vec3(g->position.x, g->position.y, g->position.z),
			glm::vec3(g->scale.x, g->scale.y, g->scale.z))) {
			if (!g->CompareLast() || firstFrame) {
				glm::mat4 transform = glm::mat4(1.0f);
				glm::mat4 transMat = glm::translate(transform, glm::vec3(g->position.x, g->position.y, g->position.z));
				glm::vec3 rot(DegreesToRadians(g->rotation.x), DegreesToRadians(g->rotation.y), DegreesToRadians(g->rotation.z));
				glm::quat _rot = glm::quat(rot);
				glm::mat4 rotMat = glm::mat4_cast(_rot);
				glm::mat4 scaleMat = glm::scale(transform, glm::vec3(g->scale.x, g->scale.y, g->scale.z));
				transform = transMat * rotMat * scaleMat;
				g->m_transform = transform;
				if (!IsEmpty) {
					glm::mat4 inverse = glm::inverse(transform);
					glm::mat4 transpose = glm::transpose(inverse);
					g->m_fixedNormal = transpose;
					mesh->GetShader()->setMat4("fixedNormal", g->m_fixedNormal);
					mesh->GetShader()->setMat4("transform", g->m_transform);
				}
			}
			if(!IsEmpty)
				glDrawArrays(GL_TRIANGLES, 0, sizeof(float) * mesh->GetVertexSize());
		}
	}

	void Application::CreateNewGameObject(bool asChild, GameObject* parent) {
		GameObject* g = new GameObject(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1), "New Object");
		if (asChild) {
			parent->AddChild(g);
			m_activeScene->AddObject(g);
		}
		m_activeScene->AddObject(g);
	}

	void Application::SetStyling() {
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

		cb.LoadTextures();

		//Sound Setup
		Sound::Init();

		MainLoop();
	}

	void Application::MainLoop() {

		OnStart();

		if (inEditor)
			Camera::SetAsMain(&m_sceneCamera);

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags = ImGuiConfigFlags_DockingEnable;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(m_window.m_win, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		if (Settings::s_editorFontPath != "")
			io.FontDefault = io.Fonts->AddFontFromFileTTF(Settings::s_editorFontPath.c_str(), 16.0f);

		SetStyling();

		vao->Generate();
		vao->Bind();
		VBO vbo;
		vbo.Bind();

		if (inEditor) {
			m_sceneCamera.AddComponent<SceneCameraController>();
			scc = m_sceneCamera.GetComponent<SceneCameraController>();
			scc->Start();
		}
		else {
			for (GameObject* g : m_activeScene->_GetObjects()) {
				for (Component* c : g->__GetComponents()) {
					c->Start();
				}
			}
		}

		//Framebuffer
#pragma region Framebuffer
		float rectangleVertices[] = //covers the whole screen
		{
			// Coords    // texCoords
			 1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f,

			 1.0f,  1.0f,  1.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f
		};

		// Prepare framebuffer rectangle VBO and VAO
		unsigned int rectVAO, rectVBO;
		glGenVertexArrays(1, &rectVAO);
		glGenBuffers(1, &rectVBO);
		glBindVertexArray(rectVAO);
		glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		unsigned int FBO;
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		unsigned int fboTex;
		glGenTextures(1, &fboTex);
		glBindTexture(GL_TEXTURE_2D, fboTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_window.m_width, m_window.m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTex, 0);

		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_window.m_width, m_window.m_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		//m_framebuffer.Generate(m_width, m_height);

		Shader fboShader = Shader(Settings::s_fboVertShaderPath.c_str(), Settings::s_fboFragShaderPath.c_str());
		fboShader.Use();
		glUniform1i(glGetUniformLocation(fboShader.ID, "screenTexture"), 0);
#pragma endregion

		while (!glfwWindowShouldClose(m_window.m_win))
		{

			bool firstFrame = true;

			//TODO: Make this a function
			if (inEditor) {
				if ((m_PanelSize.x != m_texSize.x || m_PanelSize.y != m_texSize.y)) {
					glDeleteTextures(1, &fboTex);
					glDeleteFramebuffers(1, &FBO);
					glDeleteTextures(1, &rbo);
					glDeleteRenderbuffers(1, &rbo);

					glGenFramebuffers(1, &FBO);
					glBindFramebuffer(GL_FRAMEBUFFER, FBO);

					glGenTextures(1, &fboTex);
					glBindTexture(GL_TEXTURE_2D, fboTex);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_PanelSize.x, m_PanelSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glBindTexture(GL_TEXTURE_2D, 0);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTex, 0);

					glGenRenderbuffers(1, &rbo);
					glBindRenderbuffer(GL_RENDERBUFFER, rbo);
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_PanelSize.x, m_PanelSize.y);
					glBindRenderbuffer(GL_RENDERBUFFER, 0);

					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

					m_texSize = m_PanelSize;
					glViewport(0, 0, m_PanelSize.x, m_PanelSize.y);
				}
			}
			else {
				if ((m_window.m_width != m_texSize.x || m_window.m_height != m_texSize.y)) {
					glDeleteTextures(1, &fboTex);
					glDeleteFramebuffers(1, &FBO);
					glDeleteTextures(1, &rbo);
					glDeleteRenderbuffers(1, &rbo);

					glGenFramebuffers(1, &FBO);
					glBindFramebuffer(GL_FRAMEBUFFER, FBO);

					glGenTextures(1, &fboTex);
					glBindTexture(GL_TEXTURE_2D, fboTex);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_window.m_width, m_window.m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glBindTexture(GL_TEXTURE_2D, 0);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTex, 0);

					glGenRenderbuffers(1, &rbo);
					glBindRenderbuffer(GL_RENDERBUFFER, rbo);
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_window.m_width, m_window.m_height);
					glBindRenderbuffer(GL_RENDERBUFFER, 0);

					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

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
					scc->Update();
				}

				glm::vec3 direction;
				direction.x = DegreesToRadians(Camera::GetMain()->rotation.x);
				direction.y = DegreesToRadians(Camera::GetMain()->rotation.y);
				direction.z = DegreesToRadians(Camera::GetMain()->rotation.z);
				glm::vec3 cameraFront = glm::normalize(direction);
				Camera::GetMain()->front = { cameraFront.x, cameraFront.y, cameraFront.z };
				Camera::GetMain()->right = Vector3f::Normalize(Vector3f::CrossProduct(Camera::GetMain()->front, Vector3f(0, 1, 0)));
				Camera::GetMain()->up = Vector3f::Normalize(Vector3f::CrossProduct(Camera::GetMain()->right, Camera::GetMain()->front));
				glm::vec3 cameraPos;
				cameraPos.x = Camera::GetMain()->position.x;
				cameraPos.y = Camera::GetMain()->position.y;
				cameraPos.z = Camera::GetMain()->position.z;
				glm::vec3 Up = { 0.0f, 1.0f, 0.0f };
				view = glm::lookAt(cameraPos, cameraPos + cameraFront, Up);
				Camera::GetMain()->__SetViewMatrix(view);
				if(inEditor)
					projection = glm::perspective(glm::radians(45.0f), (float)m_PanelSize.x / (float)m_PanelSize.y, 0.1f, 100.0f);
				else
					projection = glm::perspective(glm::radians(45.0f), (float)m_window.m_width / (float)m_window.m_height, 0.1f, 100.0f);

				Camera::GetMain()->__SetProjectionMatrix(projection);

				glBindFramebuffer(GL_FRAMEBUFFER, FBO);
				//m_framebuffer.Bind();

				glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glEnable(GL_DEPTH_TEST);

				s_processInput(m_window.m_win);

				if (!inEditor)
					OnUpdate();
				else
					OnEditorUpdate();

				//Render active Scene
				if (m_activeScene != nullptr) {

					const Frustum camFrustum = CreateFrustum(glm::vec3(Camera::GetMain()->front.x, Camera::GetMain()->front.y, Camera::GetMain()->front.z),
						glm::vec3(Camera::GetMain()->up.x, Camera::GetMain()->up.y, Camera::GetMain()->up.z),
						glm::vec3(Camera::GetMain()->right.x, Camera::GetMain()->right.y, Camera::GetMain()->right.z),
						glm::vec3(Camera::GetMain()->position.x, Camera::GetMain()->position.y, Camera::GetMain()->position.z),
						(float)m_window.m_width / (float)m_window.m_height, 90, 0.1f, 100.0f);

					for (auto mesh : m_activeScene->_GetMeshes()) {
						if (mesh->renderMode == RENDER_FRONT) {
							glEnable(GL_CULL_FACE);
							glCullFace(GL_BACK);
						}
						vbo.Bind();
						vbo.SetData(mesh->GetVertices(), sizeof(float) * mesh->GetVertexSize());
						vao->Bind();
						vao->Link(&vbo, 0, 1);
						vbo.Unbind();
						Shader* s = mesh->GetShader();
						s->Use();
						if (mesh->HasTexture())
							mesh->GetTexture()->Bind();
						s->setMat4("view", view);
						s->setMat4("projection", projection);
						s->setVec3("viewPos", Camera::GetMain()->position.x, Camera::GetMain()->position.y, Camera::GetMain()->position.z);
						s->setVec3("material.diffuse", s->diffuse.x, s->diffuse.y, s->diffuse.z);
						s->setVec3("material.specular", s->specular.x, s->specular.y, s->specular.z);
						s->setFloat("material.shininess", s->shininess);
						s->setVec3("lightPos", m_activeScene->GetLight()->position.x, m_activeScene->GetLight()->position.y, m_activeScene->GetLight()->position.z);
						s->setVec3("light.ambient", m_activeScene->GetLight()->ambient.x, m_activeScene->GetLight()->ambient.y, m_activeScene->GetLight()->ambient.z);
						s->setVec3("light.diffuse", m_activeScene->GetLight()->color.x, m_activeScene->GetLight()->color.y,
							m_activeScene->GetLight()->color.z);
						s->setVec3("light.specular", m_activeScene->GetLight()->specular.x, m_activeScene->GetLight()->specular.y, m_activeScene->GetLight()->specular.z);

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
								RenderObject(g, mesh, firstFrame, camFrustum);
								//vbo.Unbind();
								if (!inEditor) {
									for (Component* c : g->__GetComponents()) {
										c->Update();
									}
								}
							}
							glDisable(GL_BLEND);
							//vbo.Delete();
						}
						else {
							for (auto g : mesh->_GetGameObjects()) {
								g->__UpdateChildren();
								RenderObject(g, mesh, firstFrame, camFrustum);
								//vbo.Unbind();
								if (!inEditor) {
									for (Component* c : g->__GetComponents()) {
										c->Update();
									}
								}
							}
						}
						glDisable(GL_CULL_FACE);
						if (mesh->HasTexture())
							mesh->GetTexture()->Unbind();
						//vbo.Delete();
					}
					//vao->Unbind();
					//update all none mesh objects
					if (!inEditor) {
						for (auto g : m_activeScene->_GetObjects()) {
							if (!g->HasMesh()) {
								g->__UpdateChildren();
								for (Component* c : g->__GetComponents()) {
									c->Update();
								}
								RenderObject(g, nullptr, firstFrame, camFrustum, true);
							}
						}
					}
					else {
						for (auto g : m_activeScene->_GetObjects()) {
							if (!g->HasMesh()) {
								//Gizmos need the right Transformation Matrix, which is calculated with this:
								RenderObject(g, nullptr, firstFrame, camFrustum, true);
							}
						}

					}
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					FBOTexture = fboTex;
					glClear(GL_COLOR_BUFFER_BIT);

					//Update Sound Listener
					Sound::SetListener(Vector3f::Normalize(Camera::GetMain()->rotation), Camera::GetMain()->position);

					if (!inEditor) {
						fboShader.Use();
						glBindVertexArray(rectVAO);
						glDisable(GL_DEPTH_TEST);
						glBindTexture(GL_TEXTURE_2D, fboTex);
						FBOTexture = fboTex;
						glDisable(GL_CULL_FACE);
						glDrawArrays(GL_TRIANGLES, 0, 6);
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

			float currentFrame = glfwGetTime();
			Time::s_deltaTime = currentFrame - Time::s_lastframe;
			Time::s_lastframe = currentFrame;

			firstFrame = false;

		}

		m_window.m_windowOpen = false;

		delete scc;
		delete vao;
		vbo.Delete();
		glDeleteFramebuffers(1, &FBO);

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

		static int op;
		if (selectedObject != nullptr) {

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
					Vector3f deltaRotation = Vector3f(RadiansToDegrees(rot.x), RadiansToDegrees(rot.y), RadiansToDegrees(rot.z)) - selectedObject->rotation;
					selectedObject->rotation += deltaRotation;
					selectedObject->scale = Vector3f(scale.x, scale.y, scale.z);
				}
			}
		}

		ImGui::End();
		RenderInspector();
		RenderObjectPanel();
		RenderContentBrowser();
		RenderDebugPanel();
		RenderConsolePanel();
		ImGui::End();

	}

	void Application::RenderInspector() {
		ImGui::Begin("Inspector");
		for (auto g : m_activeScene->_GetObjects()) {
			if (!g->IsChild()) {
				bool node = ImGui::TreeNodeEx(g->name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);
				if (ImGui::IsItemClicked()) {
					selectedObject = g;
				}
				if (node) {
					RenderChildrenInInspector(g);
					ImGui::TreePop();
				}
			}
		}
		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::MenuItem("New Object")) {
				CreateNewGameObject();
			}
			ImGui::EndPopup();
		}
		ImGui::End();
	}

	void Application::RenderObjectPanel() {
		ImGui::Begin("Object");
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
					if (ImGui::TreeNodeEx(c->getName(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed)) {
						c->DrawUI();
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
			if (ImGui::BeginPopupContextWindow()) {
				if (ImGui::MenuItem("New Child Object")) {
					CreateNewGameObject(true, selectedObject);
				}
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}

	void Application::RenderContentBrowser() {
		ImGui::Begin("Content");
		cb.Draw();
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

	void Application::RenderConsolePanel() {
		m_console.Draw();
	}

	void Application::RenderChildrenInInspector(GameObject* g) {
		if (g->GetChildren().size() > 0) {
			for (auto g1 : g->GetChildren()) {
				bool node = ImGui::TreeNodeEx(g1->name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow);
				if (ImGui::IsItemClicked()) {
					selectedObject = g1;
				}
				if (node) {
					RenderChildrenInInspector(g1);
					ImGui::TreePop();
				}
			}
		}
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