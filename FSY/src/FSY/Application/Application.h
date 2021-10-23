#pragma once

#include "../Core/Core.h"
#include "../../vendor/OpenGL/include/glad.h"
#include "../../vendor/OpenGL/include/glfw3.h"
#include "../../vendor/glm/glm.hpp"

#include "../Scene/Scene.h"
#include "../Rendering/VAO.h"
#include "../Input/Input.h"
#include "FSY_Time.h"
#include "../Editor/ContentBrowser.h"

#include <iostream>

namespace FSY {

    extern class SceneCameraController;

	class FSY_API Application
	{
	public:
		Application();
        Application(int width, int height, const char* title);
		virtual ~Application();

        static Application* GetInstance();

        virtual void OnStart() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnClose() = 0;
        virtual void OnEditorUpdate() = 0;

		void Run();
        void Close();
        void ChangeScene(Scene* scene);
        void SetIcon(const char* filename);

        float WinWidth();
        float WinHeight();

        Scene* GetActiveScene();

        bool inEditor = true;

        //DON'T USE
        GLFWwindow* __GetWindow();

	private:

        static Application* app;

#pragma region Win Vars
        GLFWwindow* m_win = nullptr;
        bool m_windowOpen = true;
        int m_width = 1000;
        int m_height = 800;
        const char* m_title = "My Win";
#pragma endregion

#pragma region Win Callbacks
        static void s_framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void s_mouse_callback(GLFWwindow* window, double xpos, double ypos);
        static void s_processInput(GLFWwindow* window);
#pragma endregion

#pragma region Render Vars
        VAO* vao = new VAO();
        glm::mat4 view = glm::mat4(1.0);
        glm::mat4 projection;
#pragma endregion

#pragma region Scene
        SceneCameraController* scc = nullptr;
        Scene* m_activeScene = nullptr;
        float m_clearColor[3] = { 0.2f, 0.2f, 0.2f };
#pragma endregion

#pragma region Scene View
        void RenderUI();
        void RenderChildren(GameObject* g);
        GameObject* selectedObject = nullptr;
        ContentBrowser cb;
#pragma endregion

#pragma region Audio
        
#pragma endregion

        void Init();

        void MainLoop();
        
	};

	Application* CreateApplication();

}