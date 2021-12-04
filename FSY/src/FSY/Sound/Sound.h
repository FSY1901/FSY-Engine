#ifndef FSY_SOUND_H
#define FSY_SOUND_H

#include "../../vendor/irrKlang/include/irrKlang.h"
#include <iostream>

#include "../Core/Core.h"
#include "../Maths/Maths.h"
#include "../Scene/GameObject.h"
#include "../Editor/EditorUI.h"

extern class Application;

namespace FSY {

	class FSY_API Sound : public Component {

	public:
		Sound();
		~Sound();

		static irrklang::ISoundEngine* GetSoundEngine();
		static void CreateSoundEngine() {
			if (engine == nullptr)
				engine = irrklang::createIrrKlangDevice();
		}

		void Path(std::string path);
		void Stop();

		void Start() override {

		}

		void Update() override {
			if (play)
				Play(Vector3f(gameObject->position.x, gameObject->position.y, gameObject->position.z), minDistance, maxDistance);
			else
				Stop();
		}

		const char* getName() override{
			return "Sound";
		}

		void DrawUI() {
			EditorUI::CheckBox("Play", &play);
			EditorUI::CheckBox("3D", &is3D);
			EditorUI::InputFloat("Min Distance", &minDistance);
			EditorUI::InputFloat("Max Distance", &maxDistance);
		}

		bool is3D = true;
		bool play = false;

		float minDistance = 5.0f;
		float maxDistance = 100.0f;

	private:
		std::string m_path;
		bool isPlaying = false;

		void Play(Vector3f position, float minDistance, float maxDistance);

		irrklang::ISoundSource* source = nullptr;
		irrklang::ISound* music = nullptr;

		static irrklang::ISoundEngine* engine;
		static Vector3f listenerPos;

		friend class Application;

	};

}

#endif // !FSY_SOUND