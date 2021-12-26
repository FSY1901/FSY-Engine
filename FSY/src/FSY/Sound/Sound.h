#ifndef FSY_SOUND_H
#define FSY_SOUND_H

#include <iostream>

#include "../Core/Core.h"
#include "../Maths/Maths.h"
#include "../Scene/GameObject.h"
#include "../Editor/EditorUI.h"

#include "../../vendor/Audeo/audeo.hpp"

extern class Application;

namespace FSY {

	class FSY_API Sound {

	public:
		Sound();
		~Sound();

		static void Init();
		static void Quit();
		static void SetListener(Vector3f forward, Vector3f position);

		void LoadSource(std::string_view source);
		void Play();

	private:

		audeo::SoundSource src;
		audeo::Sound snd;

		friend class Application;

	};

}

#endif // !FSY_SOUND