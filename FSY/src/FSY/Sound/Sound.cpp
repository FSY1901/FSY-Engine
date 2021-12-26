#include "Sound.h"

namespace FSY {

	Sound::Sound() {
		
	}

	Sound::~Sound() {
		audeo::free_source(src);
	}

	void Sound::Init() {
		try {
			audeo::init();
		}
		catch (audeo::exception& e) {
			std::cout << e.what();
		}
	}

	void Sound::Quit() {
		audeo::quit();
	}

	void Sound::SetListener(Vector3f forward, Vector3f position) {
		audeo::set_listener_forward(forward.x, forward.y, forward.z);
		audeo::set_listener_position(position.x, position.y, position.z);
	}

	void Sound::LoadSource(std::string_view source) {
		src = audeo::load_source(source, audeo::AudioType::Effect);
	}

	void Sound::Play() {
		snd = audeo::play_sound(src, 1);
		audeo::set_distance_range_max(snd, 100);
		audeo::set_position(snd, 0, 0 ,0);
	}

}