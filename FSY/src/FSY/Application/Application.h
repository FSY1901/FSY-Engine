#pragma once

#include "../Core/Core.h"
#include "../../vendor/OpenGL/include/glad.h"
#include "../../vendor/OpenGL/include/glfw3.h"
#include "../../vendor/glm/glm.hpp"

#include "../Scene/Scene.h"
#include "../Rendering/VAO.h"
#include "../Rendering/Framebuffer.h"
#include "../Input/Input.h"
#include "FSY_Time.h"
#include "../Editor/ContentBrowser.h"
#include "../Editor/Console.h"

#include <iostream>

namespace FSY {

    extern class SceneCameraController;

    struct Window {
        GLFWwindow* m_win = nullptr;
        bool m_windowOpen;
        int m_width;
        int m_height;
        const char* m_title = "My Win";
    };

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

        float WinWidth() const;
        float WinHeight() const;

        Scene* GetActiveScene();

        bool inEditor = false;

        //DON'T USE
        GLFWwindow* __GetWindow();
        void __SetSelectedObject(GameObject* g);

	private:

        static Application* app;

#pragma region Win Vars
        Window m_window;
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

        void RenderObject(GameObject* g, Mesh* mesh, bool firstFrame, Frustum camFrustum, bool isEmpty = false);
#pragma endregion

#pragma region Scene View
        void SetStylingEditScene();
        void SetStylingPlayScene();

        void RenderUI();

        Console m_console;//Console for the Console panel
        void RenderDebugPanel();

        Texture m_playButtonTexture;
        Texture m_stopTexture;

        ContentBrowser cb;
        uint32_t FBOTexture;
        ImVec2 m_PanelSize = {1, 1};
        ImVec2 m_texSize = { 0,0 };
        bool isHovered = false;
        Camera m_renderCamera;
#pragma endregion

        void Init();

        void MainLoop();
        
        void CreateNewGameObject(bool asChild = false, GameObject* parent = nullptr);

    protected:
        Camera m_sceneCamera = Camera(Vector3f(), Vector3f(0, 0.0f, 0), Vector3f(1, 1, 1));
        GameObject* selectedObject = nullptr;

	};

	Application* CreateApplication();

}