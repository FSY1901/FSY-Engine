#include "SoundExperimental.h"

namespace FSY {

	void SoundExperimental::LoadSource(std::string_view source) {
		src = audeo::load_source(source, audeo::AudioType::Music);
	}

	void SoundExperimental::Play() {
		audeo::play_sound(src, 1);
	}

	void SoundExperimental::Init() {
		try {
			audeo::init();
		}
		catch(audeo::exception& e){
			std::cout << e.what();
		}
	}

	void SoundExperimental::Quit() {
		audeo::quit();
	}

}