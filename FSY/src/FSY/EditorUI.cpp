#include "EditorUI.h"

namespace FSY {

	void EditorUI::Text(const char* text) {
		ImGui::PushItemWidth(80.0f);
		ImGui::TextWrapped(text);
		ImGui::PopItemWidth();
	}

	void EditorUI::InputText(const char* label, std::string* text) {
		ImGui::PushItemWidth(80.0f);
		ImGui::InputText(label, text);
		ImGui::PopItemWidth();
	}

	void EditorUI::SliderFloat(const char* label, float* f, float min, float max) {
		ImGui::PushItemWidth(80.0f);
		ImGui::SliderFloat(label, f, min, max);
		ImGui::PopItemWidth();
	}

	void EditorUI::InputFloat(const char* label, float* f) {
		ImGui::PushItemWidth(80.0f);
		ImGui::InputFloat(label, f);
		ImGui::PopItemWidth();
	}

}