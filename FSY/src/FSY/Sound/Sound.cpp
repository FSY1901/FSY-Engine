#include "Sound.h"

namespace FSY {

	irrklang::ISoundEngine* Sound::engine = nullptr;
	Vector3f Sound::listenerPos = Vector3f();

	Sound::Sound() {
		
	}

	Sound::~Sound() {
		if(music)
			music->drop();

		source->drop();
	}

	irrklang::ISoundEngine* Sound::GetSoundEngine() { return engine; }

	void Sound::Path(std::string path) {
		source = engine->addSoundSourceFromFile(path.c_str());
		source->setDefaultMinDistance(5.0f);
	}

	void Sound::Stop() {
		play = false;
		isPlaying = false;
		music->stop();
	}

	void Sound::Play(Vector3f position, float minDistance, float maxDistance) {
		if (play) {
			if (!isPlaying) {
				music = engine->play3D(source, irrklang::vec3df(position.x, position.y, position.z), true, false, true);
				music->setMinDistance(minDistance);
			}

			music->setPosition(irrklang::vec3df(position.x, position.y, position.z));

			if (Vector3f::Distance(listenerPos, position) > maxDistance) {
				music->setVolume(0);
			}
			else {
				music->setVolume(1);
			}

			isPlaying = true;
		}
	}

}