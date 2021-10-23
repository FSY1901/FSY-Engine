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
		static bool fullscreen;
		//The Path where the Font for the Editor can be found.
		static std::string editorFontPath;

	};

}

#endif // !FSY_SETTINGS_H
