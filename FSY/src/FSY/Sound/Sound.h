#include "../Core/Core.h"
#include "../Maths/Maths.h"

#include <vector>

#include "../../vendor/OpenAL/AL/al.h"
#include "../../vendor/OpenAL/AL/alc.h"

#include "../../vendor/drwav/dr_wav.h"

#ifndef FSY_SOUND_H
#define FSY_SOUND_H

namespace FSY {

#define OpenAL_ErrorCheck(message)\
{\
	ALenum error = alGetError();\
	if( error != AL_NO_ERROR)\
	{\
		std::cerr << "OpenAL Error: " << error << " with call for " << #message << std::endl;\
	}\
}

#define alec(FUNCTION_CALL)\
FUNCTION_CALL;\
OpenAL_ErrorCheck(FUNCTION_CALL)

	class Device {

	public:
		static ALCdevice* GetDevice();
		static void Setup();
		static void Delete();

	private:
		static ALCdevice* device;

	};

	struct Context {
		static void Create();
		static void Delete();
		static ALCcontext* context;
	};

	class FSY_API Listener {

	public:
		Listener();
		void Update(Vector3f position, Vector3f forward, Vector3f up);

	};

	struct ReadWavData
	{
		unsigned int channels = 0;
		unsigned int sampleRate = 0;
		drwav_uint64 totalPCMFrameCount = 0;
		std::vector<uint16_t> pcmData;
		drwav_uint64 getTotalSamples() { return totalPCMFrameCount * channels; }
	};

	class FSY_API Source {

	public:
		Vector3f position;

		Source();
		void Load(std::string path);
		void Play();
		~Source();

	private:
		ReadWavData m_data;
		ALuint m_soundBuffer;
		ALuint m_source;
		ALint sourceState;

		bool m_playing = false;

	};

}

#endif