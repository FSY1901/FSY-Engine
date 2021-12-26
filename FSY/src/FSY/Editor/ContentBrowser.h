#include "../../vendor/imgui/imgui.h"

#include "../Core/Core.h"
#include "../Rendering/Texture.h"
#include <filesystem>

namespace FSY {

	class ContentBrowser {

	public:
		ContentBrowser();
		void LoadTextures();
		void Draw();

	private:

		const std::filesystem::path m_AssetPath = "src/Data";
		std::filesystem::path m_CurrentPath;
		//Textures/Icons
		Texture m_FolderIcon;
		Texture m_FontIcon;
		Texture m_PNGIcon;

		Texture m_buggyTexture; //For some reason the first texture that is loaded isn't loaded properly, so an unused texture is assigned

		//Creating Folders
		bool __creatingNewFolder = false;
		std::string __newFolderName = "New Folder";

		//Renaming Folders
		std::string __directoryToRename;
		std::string __originalName;
		bool __isRenaming = false;
		bool __wantsRename = false;

	};

}