#include "../../vendor/Audeo/audeo.hpp"
#include <iostream>
#include "../Core/Core.h"

namespace FSY {

	class FSY_API SoundExperimental {

	public:
		static void Init();

		void LoadSource(std::string_view source);

		void Play();

		static void Quit();

		audeo::SoundSource src;

	};

}