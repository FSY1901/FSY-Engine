#include "Console.h"

namespace FSY {

	std::string Console::m_latestMessage = "";

	void Console::Log(std::string message) { m_latestMessage = message; }

	std::string Console::_GetLatestMessage() { return m_latestMessage; }

}