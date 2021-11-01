#ifndef EDITORUI_H
#define EDITORUI_H

#include "../Core/Core.h"

#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/imgui_impl_glfw.h"
#include "../../vendor/imgui/imgui_impl_opengl3.h"
#include "../../vendor/imgui/imgui_stdlib.h"

namespace FSY {

	class FSY_API EditorUI {

	public:
		static void Text(const char* text);

		/*Label = Name of the Textfield
		*text = The string that is changed
		*/
		static void InputText(const char* label, std::string* text);

		static void SliderFloat(const char* label, float* f, float min, float max);

		static void InputFloat(const char* label, float* f);

		static void CheckBox(const char* label, bool* b);

	};

}

#endif // !EDITORUI_H
