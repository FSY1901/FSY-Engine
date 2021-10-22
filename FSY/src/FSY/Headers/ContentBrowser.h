#include "../../vendor/imgui/imgui.h"

#include "Core.h"
#include <filesystem>

namespace FSY {

	class ContentBrowser {

	public:
		ContentBrowser();
		void Draw();

	private:

		const std::filesystem::path m_AssetPath = "src/Data";
		std::filesystem::path m_CurrentPath;

	};

}