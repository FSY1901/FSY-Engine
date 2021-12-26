#include "Console.h"

namespace FSY {

	Console* Console::m_Console = nullptr;
	bool Console::m_logged = false;

	Console::Console() {
		m_Console = this;
	}

	void Console::Log(std::string message) {
		std::time_t t = std::time(0);
		std::tm now;
		localtime_s(&now, &t);
		std::string time = std::to_string(now.tm_hour) + ":" + std::to_string(now.tm_min) + ":" + std::to_string(now.tm_sec);
		std::string msg = time + " [LOG]: " + message;
		Item item = Item(msg, MessageType::LOG);
		m_Console->m_items.push_back(item);
		m_logged = true;
	}

	void Console::Warn(std::string message) {
		std::time_t t = std::time(0);
		std::tm now;
		localtime_s(&now, &t);
		std::string time = std::to_string(now.tm_hour) + ":" + std::to_string(now.tm_min) + ":" + std::to_string(now.tm_sec);
		std::string msg = time + " [WARNING]: " + message;
		Item item = Item(msg, MessageType::WARN);
		m_Console->m_items.push_back(item);
		m_logged = true;
	}

	void Console::Error(std::string message) {
		std::time_t t = std::time(0);
		std::tm now;
		localtime_s(&now, &t);
		std::string time = std::to_string(now.tm_hour) + ":" + std::to_string(now.tm_min) + ":" + std::to_string(now.tm_sec);
		std::string msg = time + " [ERROR]: " + message;
		Item item = Item(msg, MessageType::ERROR);
		m_Console->m_items.push_back(item);
		m_logged = true;
	}

	void Console::Draw() {
		ImGui::Begin("Console");
		if (ImGui::SmallButton("Clear")) { m_items.clear(); }
		ImGui::BeginChild("Scroll Region");
		for (auto& item : m_items) {
			ImVec4 color;
			bool hasColor = false;
			switch (item.msgType)
			{
			case MessageType::LOG:
				ImGui::TextUnformatted(item.message.c_str());
				break;
			case MessageType::WARN:
				hasColor = true;
				color = ImVec4(0.921f, 0.764f, 0.0f, 1.0f);
				ImGui::PushStyleColor(ImGuiCol_Text, color);
				ImGui::TextUnformatted(item.message.c_str());
				break;
			case MessageType::ERROR:
				hasColor = true;
				color = ImVec4(0.552f, 0.039f, 0.003f, 1.0f);
				ImGui::PushStyleColor(ImGuiCol_Text, color);
				ImGui::TextUnformatted(item.message.c_str());
				break;
			default:
				break;
			}
			if (hasColor)
				ImGui::PopStyleColor();
		}
		if (m_logged) {
			ImGui::SetScrollHereY(1.0f);
			m_logged = false;
		}
		ImGui::EndChild();
		ImGui::End();
	}

}