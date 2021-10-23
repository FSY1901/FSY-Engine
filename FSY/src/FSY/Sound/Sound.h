#ifndef FSY_SOUND_H
#define FSY_SOUND_H

#include "../../vendor/irrKlang/include/irrKlang.h"
#include <iostream>

#include "../Core/Core.h"
#include "../Maths/Maths.h"

using namespace FSY::MATHS;

extern class Application;

namespace FSY {

	class FSY_API Sound {

	public:
		Sound();
		Sound(std::string path);
		~Sound();

		void Play(Vector3f position,float minDistance, float maxDistance);

		static irrklang::ISoundEngine* GetSoundEngine();

		bool is3D = true;

	private:
		std::string m_path;
		bool isPlaying = false;

		irrklang::ISoundSource* source = nullptr;
		irrklang::ISound* music;

		static irrklang::ISoundEngine* engine;
		static Vector3f listenerPos;

		friend class Application;

	};

}

#endif // !FSY_SOUND