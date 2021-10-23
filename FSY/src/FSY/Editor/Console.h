#ifndef FSY_CONSOLE_H
#define FSY_CONSOLE_H

#include "../Core/Core.h"
#include <string>

namespace FSY {

	class FSY_API Console {

	public:
		Console();

		static void Log(std::string message);

		//Returns the latest logged message --> should only be used by the Engine.
		static std::string _GetLatestMessage();

	private:
		static std::string m_latestMessage;

	};

}

#endif // !FSY_CONSOLE_H
