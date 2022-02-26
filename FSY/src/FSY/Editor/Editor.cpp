#include "Editor.h"
#include "../Application/Application.h"

namespace FSY {

	GameObject* RenderInspector(Scene* activeScene, GameObject* selectedObject, Camera& sceneCamera) {
		ImGui::Begin("Inspector");
		for (auto g : activeScene->_GetObjects()) {
			if (!g->IsChild()) {
				bool node = ImGui::TreeNodeEx(g->name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);
				if (ImGui::IsItemClicked()) {
					selectedObject = g;
				}
				if (node) {
					selectedObject = RenderChildrenInInspector(g, selectedObject);
					ImGui::TreePop();
				}
			}
		}
		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::MenuItem("New Object")) {
				GameObject::CreateGameObject(false, &sceneCamera, activeScene);
			}
			ImGui::EndPopup();
		}
		ImGui::End();

		return selectedObject;
	}

	GameObject* RenderChildrenInInspector(GameObject* g, GameObject* selectedObject) {
		if (g->GetChildren().size() > 0) {
			for (auto g1 : g->GetChildren()) {
				bool node = ImGui::TreeNodeEx(g1->name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow);
				if (ImGui::IsItemClicked()) {
					selectedObject = g1;
				}
				if (node) {
					selectedObject = RenderChildrenInInspector(g1, selectedObject);
					ImGui::TreePop();
				}
			}
		}

		return selectedObject;
	}

	void RenderObjectPanel(GameObject* selectedObject, Scene* activeScene) {
		ImGui::Begin("Object");
		if (selectedObject != nullptr) {
			std::string s = selectedObject->name;
			ImGui::InputText("Name", &s);
			if (selectedObject->name != s)
				selectedObject->name = activeScene->__CheckName(s, selectedObject);
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
			if (ImGui::BeginPopupContextWindow("Actions On Object")) {
				if (ImGui::MenuItem("New Child Object")) {
					GameObject::CreateGameObject(true, selectedObject, activeScene);
				}

				if (ImGui::MenuItem("Remove From Scene")) {
					activeScene->DeleteObject(selectedObject);
					Application::GetInstance()->__SetSelectedObject(nullptr);
				}

				if (selectedObject->HasMesh()) {
					if (ImGui::Button("Remove Mesh")) {
						selectedObject->GetMesh()->RemoveGameObject(selectedObject);
					}
				}
				else {
					if (ImGui::Button("Add Mesh"))
						ImGui::OpenPopup("Meshes");
					if (ImGui::BeginPopup("Meshes")) {
						for (auto m : activeScene->_GetMeshes()) {
							if (ImGui::MenuItem(m->GetName().c_str())) {
								m->AddGameObject(selectedObject);
							}
						}
						ImGui::EndPopup();
					}
				}
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}

	void RenderContentBrowser(ContentBrowser* cb) {
		ImGui::Begin("Content");
		cb->Draw();
		ImGui::End();
	}

	void RenderConsolePanel(Console* console) {
		console->Draw();
	}

}