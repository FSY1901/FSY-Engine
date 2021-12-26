#ifndef FSY_CONSOLE_H
#define FSY_CONSOLE_H

#include "../Core/Core.h"

#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/imgui_impl_glfw.h"
#include "../../vendor/imgui/imgui_impl_opengl3.h"
#include "../../vendor/imgui/imgui_stdlib.h"

#include <string>
#include <vector>
#include <ctime>

namespace FSY {

	extern class Application;

	enum class MessageType {
		LOG,
		WARN,
		ERROR
	};

	//Console Log Message
	struct Item {
		Item(std::string msg, MessageType type) : message(msg), msgType(type) {}
		std::string message;
		MessageType msgType;
	};

	class FSY_API Console {

	public:
		Console();
		static void Log(std::string message);
		static void Warn(std::string message);
		static void Error(std::string message);

	private:
		static Console* m_Console;
		static bool m_logged;
		std::vector<Item> m_items;

		void Draw();

		friend class Application;
	};

}

#endif // !FSY_CONSOLE_H
