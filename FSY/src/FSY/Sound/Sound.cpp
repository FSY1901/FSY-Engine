#include "Sound.h"

#include "../Editor/Console.h"

namespace FSY {

	ALCdevice* Device::device = nullptr;

	ALCdevice* Device::GetDevice() {
		return device;
	}

	void Device::Setup() {
		const ALCchar* deviceString = alcGetString(/*device*/nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
		device = alcOpenDevice(NULL);
		if (!device)
		{
			Console::Error("Could not find default device");
		}
	}

	void Device::Delete() {
		alcCloseDevice(device);
	}

	ALCcontext* Context::context = nullptr;

	void Context::Create() {
		context = alcCreateContext(Device::GetDevice(), /*attrlist*/ nullptr);
		if (!alcMakeContextCurrent(context))
		{
			Console::Error("Failed to make context the current context");
		}
	}
	void Context::Delete() {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
	}

	Listener::Listener() {

	}

	void Listener::Update(Vector3f position, Vector3f forward, Vector3f up) {
		alec(alListener3f(AL_POSITION, 0.0f, 0.0f,0.0f));
		alec(alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f)); //TODO: Calculate velocity
		ALfloat forwardAndUpVectors[] = {
			forward.x, forward.y, forward.z,
			up.x, up.y, up.z
		};
		alec(alListenerfv(AL_ORIENTATION, forwardAndUpVectors));
	}

	Source::Source() {

	}

	void Source::Load(std::string path) {

		ReadWavData data;
		drwav_int16* pSampleData = drwav_open_file_and_read_pcm_frames_s16(path.c_str(), &data.channels, &data.sampleRate, &data.totalPCMFrameCount, nullptr);
		if (pSampleData == NULL) {
			Console::Error("Failed to load Audiofile.");
			drwav_free(pSampleData, nullptr); //todo use raii to clean this up
		}
		if (data.getTotalSamples() > drwav_uint64(std::numeric_limits<size_t>::max()))
		{
			Console::Error("Too much data in file for 32bit addressed vector.");
			drwav_free(pSampleData, nullptr);
		}
		data.pcmData.resize(size_t(data.getTotalSamples()));
		std::memcpy(data.pcmData.data(), pSampleData, data.pcmData.size() * /*twobytes_in_s16*/2);
		drwav_free(pSampleData, nullptr);

		alec(alGenBuffers(1, &m_soundBuffer));
		alec(alBufferData(m_soundBuffer, data.channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, data.pcmData.data(), data.pcmData.size() * 2 /*two bytes per sample*/, data.sampleRate));

		alec(alGenSources(1, &m_source));
		alec(alSource3f(m_source, AL_POSITION, 0.f, 0.f, 0.f));
		alec(alSource3f(m_source, AL_VELOCITY, 0.f, 0.f, 0.f));
		alec(alSourcef(m_source, AL_PITCH, 1.0f));
		alec(alSourcef(m_source, AL_GAIN, 1.f));
		alec(alSourcei(m_source, AL_LOOPING, AL_FALSE));
		alec(alSourcei(m_source, AL_BUFFER, m_soundBuffer));

	}

	void Source::Play() {
		if(m_playing == false)
			alec(alSourcePlay(m_source));
		m_playing = true;

		alec(alGetSourcei(m_source, AL_SOURCE_STATE, &sourceState));

		if (sourceState == AL_PLAYING)
		{
			alec(alGetSourcei(m_source, AL_SOURCE_STATE, &sourceState));
		}
	}

	Source::~Source() {
		alec(alDeleteSources(1, &m_source));
		alec(alDeleteBuffers(1, &m_soundBuffer));
	}

}