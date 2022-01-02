#include "ContentBrowser.h"
#include "../Application/Settings.h"

#include <../imgui/imgui_stdlib.h>
#include <iostream>

namespace FSY {

	ContentBrowser::ContentBrowser() { m_CurrentPath = m_AssetPath; }

	void ContentBrowser::LoadTextures() {
		m_buggyTexture = { Settings::s_folderIconPath.c_str(), false };
		m_FolderIcon = { Settings::s_folderIconPath.c_str(), false };
		m_FontIcon = { Settings::s_ttfIcon.c_str(), false };
		m_PNGIcon = { Settings::s_pngIcon.c_str(), false };
		m_TXTIcon = { Settings::s_txtIcon.c_str(), false };
	}

	void ContentBrowser::Draw() {

		if (m_CurrentPath != std::filesystem::path(m_AssetPath)) {
			if (ImGui::Button("BACK")) {
				m_CurrentPath = m_CurrentPath.parent_path();
			}
		}

		ImGui::Text(m_CurrentPath.string().c_str());
		float cellSize = 85;
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columns = (int)(panelWidth / cellSize);
		if (columns < 1)
			columns = 1;
		ImGui::Columns(columns, 0, false);

		for (auto& p : std::filesystem::directory_iterator(m_CurrentPath)) {
			auto relative = std::filesystem::relative(p.path(), m_AssetPath);
			std::string file = relative.filename().string();

			if (p.is_directory()) {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
				ImGui::ImageButton((void*)(intptr_t)m_FolderIcon.GetTexture(), ImVec2(64, 50));
				ImGui::PopStyleColor();
				//ImGui::Button(file.c_str());
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					m_CurrentPath /= p.path().filename();
				}
				else if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right) && !__creatingNewFolder) {
					__wantsRename = true;
					__originalName = __directoryToRename = file;
				}
				ImGui::TextWrapped(file.c_str());
			}
			else {
				if(relative.filename().extension().string() == ".ttf") {
					ImGui::Image((void*)(intptr_t)m_FontIcon.GetTexture(), ImVec2(64, 64));
				}
				else if (relative.filename().extension().string() == ".png") {
					ImGui::Image((void*)(intptr_t)m_PNGIcon.GetTexture(), ImVec2(56.4f, 64));
				}
				else if (relative.filename().extension().string() == ".txt") {
					ImGui::Image((void*)(intptr_t)m_TXTIcon.GetTexture(), ImVec2(62.7f, 70.4f));
				}
				ImGui::TextWrapped(file.c_str());
			}

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::MenuItem("New...") && !__creatingNewFolder) {
				__creatingNewFolder = true;
			}
			ImGui::EndPopup();
		}

		if (__wantsRename) {
			if (ImGui::BeginPopupContextWindow()) {
				if (ImGui::MenuItem("Rename")) {
					__isRenaming = true;
				}
				ImGui::EndPopup();
			}
		}

		//////////////////////////
		//CONTENT BROWSER WINDOWS
		//////////////////////////

		if(__creatingNewFolder) {
			ImGui::Begin("New Folder", &__creatingNewFolder, ImGuiWindowFlags_NoResize);
			ImGui::SetWindowSize(ImVec2(400, 100));
			std::string s = __newFolderName;
			ImGui::InputText("Name", &s);
			if (__newFolderName != s)
				__newFolderName = s;
			if (ImGui::Button("Create")) {
				std::filesystem::path newPath = m_CurrentPath;
				newPath /= s;
				std::filesystem::create_directory(newPath);
				__creatingNewFolder = false;
			}
			ImGui::End();
		}

		if (__isRenaming) {
			ImGui::Begin("Rename Folder", &__isRenaming, ImGuiWindowFlags_NoResize);
			ImGui::SetWindowSize(ImVec2(400, 100));
			std::string s = __directoryToRename;
			ImGui::InputText("Name", &s);
			if (__directoryToRename != s)
				__directoryToRename = s;
			if (ImGui::Button("Rename")) {
				std::error_code err;
				std::filesystem::rename(m_CurrentPath/__originalName, m_CurrentPath/s, err);
				__isRenaming = false;
			}
			ImGui::End();
		}

	}

}