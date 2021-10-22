#include "ContentBrowser.h"

namespace FSY {

	ContentBrowser::ContentBrowser() { m_CurrentPath = m_AssetPath; }

	void ContentBrowser::Draw() {

		if (m_CurrentPath != std::filesystem::path(m_AssetPath)) {
			if (ImGui::Button("BACK")) {
				m_CurrentPath = m_CurrentPath.parent_path();
			}
		}

		for (auto& p : std::filesystem::directory_iterator(m_CurrentPath)) {
			auto relative = std::filesystem::relative(p.path(), m_AssetPath);
			std::string file = relative.filename().string();
			if (p.is_directory()) {
				if (ImGui::Button(file.c_str())) {
					m_CurrentPath /= p.path().filename();
				}
			}
			else {
				ImGui::Text(file.c_str());
			}
		}

		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::MenuItem("Click me!")) {

			}
			ImGui::EndPopup();
		}

	}

}