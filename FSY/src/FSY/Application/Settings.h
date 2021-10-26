#ifndef FSY_SETTINGS_H
#define FSY_SETTINGS_H

#include "../Core/Core.h"
#include <string>

namespace FSY {

	class FSY_API Settings {

	public:
		Settings();

		///Vars///

		//Specifies if the application runs in Windowed Fullscreen Mode. The Graphics will adjust automatically.
		static bool s_fullscreen;
		//The Path where the Font for the Editor can be found.
		static std::string s_editorFontPath;
		static std::string s_folderIconPath;
		static std::string s_ttfIcon;
		static std::string s_pngIcon;

	};

}

#endif // !FSY_SETTINGS_H
