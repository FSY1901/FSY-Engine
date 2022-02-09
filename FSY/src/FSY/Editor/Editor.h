#ifndef FSY_EDITOR_H
#define FSY_EDITOR_H

#include <../imgui/imgui.h>
#include <../imgui/imgui_impl_glfw.h>
#include <../imgui/imgui_impl_opengl3.h>
#include <../imgui/imgui_stdlib.h>
#include "../../vendor/imguizmo/ImGuizmo.h"

#include "../Scene/Scene.h"
#include "ContentBrowser.h"

namespace FSY {

	GameObject* RenderInspector(Scene* activeScene, GameObject* selectedObject, Camera& sceneCamera);

	GameObject* RenderChildrenInInspector(GameObject* g, GameObject* selectedObject);

	void RenderObjectPanel(GameObject* selectedObject, Scene* activeScene);

	void RenderContentBrowser(ContentBrowser* cb);

	void RenderConsolePanel(Console* console);

}

#endif