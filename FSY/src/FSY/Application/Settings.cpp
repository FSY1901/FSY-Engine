#include "Settings.h"

namespace FSY {

	bool Settings::s_fullscreen = false;
	std::string Settings::s_editorFontPath = "";
	std::string Settings::s_folderIconPath = "./src/Data/Assets/Icons/Folder.png";
	std::string Settings::s_ttfIcon = "./src/Data/Assets/Icons/Font.png";
	std::string Settings::s_pngIcon = "./src/Data/Assets/Icons/PNG.png";

	Settings::Settings() {
		
	}

}