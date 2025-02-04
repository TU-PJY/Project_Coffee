#include "Scene.h"
#include "SoundUtil.h"
#include "Config.h"
#include <vector>

SoundUtil soundUtil;

std::vector<float> FFTdata(FFT_SIZE);

void SoundUtil::Init() {
	Result = FMOD::System_Create(&SoundSystem);

	if (Result != FMOD_OK)	
		exit(EXIT_FAILURE);

	SoundSystem->init(MAX_CHANNEL_SIZE, FMOD_INIT_NORMAL, ExtDvData);
	SoundSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &LowPass);
	SoundSystem->set3DSettings(1.0, 1.0, 2.0); 
}

void SoundUtil::Load(Sound& Sound, std::string FileName, FMOD_MODE Option) {
	HRESULT Result = SoundSystem->createSound(FileName.c_str(), Option, 0, &Sound);
	if (Result != FMOD_OK)
		scene.SetErrorScreen(ERROR_TYPE_AUDIO_LOAD, FileName);
}

void SoundUtil::Release(Sound& Sound) {
	Sound->release();
}

void SoundUtil::Update() {
	SoundSystem->update();
}

void SoundUtil::AddChannelToGroup(SoundChannelGroup& Group, Sound& Sound, SoundChannel& Channel) {
	SoundSystem->playSound(Sound, 0, true, &Channel);
	Group.emplace_back(Channel);
}

void SoundUtil::DeleteChannelFromGroup(SoundChannelGroup& Group, SoundChannel& Channel) {
	auto ChannelFound = std::find(begin(Group), end(Group), Channel);
	if (ChannelFound != end(Group))
		Group.erase(ChannelFound);
}

void SoundUtil::ClearGroup(SoundChannelGroup& Group) {
	Group.clear();
}

void SoundUtil::PlayGroup(SoundChannelGroup& Group) {
	for (auto const& G : Group)
		G->setPaused(false);
}

void SoundUtil::StopGroup(SoundChannelGroup& Group) {
	for (auto const& G : Group) {
		G->setPaused(true);
		G->setPosition(0, FMOD_TIMEUNIT_MS);
	}
}

void SoundUtil::PauseGroup(SoundChannelGroup& Group, bool Flag) {
	for (auto const& G : Group)
		G->setPaused(Flag);
}

void SoundUtil::Play(Sound& Sound, SoundChannel& ChannelVar, unsigned int Ms) {
	SoundSystem->playSound(Sound, 0, false, &ChannelVar);
	if (Ms > 0)
		ChannelVar->setPosition(Ms, FMOD_TIMEUNIT_MS);
}

void SoundUtil::Play(Sound& Sound, SoundChannel& ChannelVar, unsigned int Minutes, unsigned int Seconds) {
	SoundSystem->playSound(Sound, 0, false, &ChannelVar);
		ChannelVar->setPosition((Minutes * 60 + Seconds) * 1000, FMOD_TIMEUNIT_MS);
}

void SoundUtil::PlayOnce(Sound& Sound, SoundChannel&ChannelVar, bool& FlagValue, unsigned int Ms) {
	if (FlagValue) {
		SoundSystem->playSound(Sound, 0, false, &ChannelVar);
		if(Ms > 0)
			ChannelVar->setPosition(Ms, FMOD_TIMEUNIT_MS);
		FlagValue = false;
	}
}

void SoundUtil::PlayOnce(Sound& Sound, SoundChannel& ChannelVar, bool& FlagValue, unsigned int Minutes, unsigned int Seconds) {
	if (FlagValue) {
		SoundSystem->playSound(Sound, 0, false, &ChannelVar);
		ChannelVar->setPosition((Minutes * 60 + Seconds) * 1000, FMOD_TIMEUNIT_MS);
	}
}

void SoundUtil::Pause(SoundChannel& ChannelVar, bool Flag) {
	ChannelVar->setPaused(Flag);
}

void SoundUtil::Stop(SoundChannel& ChannelVar) {
	ChannelVar->stop();
}

unsigned int SoundUtil::GetLength(Sound& Sound) {
	unsigned int Length{};
	Sound->getLength(&Length, FMOD_TIMEUNIT_MS);
	return Length;
}

unsigned int SoundUtil::GetPlayTime(SoundChannel& ChannelVar) {
	unsigned int Position{};
	ChannelVar->getPosition(&Position, FMOD_TIMEUNIT_MS);
	return Position;
}

void SoundUtil::SetPlaySpeed(SoundChannel& ChannelVar, float Speed) {
	ChannelVar->setPitch(Speed);
}

void SoundUtil::ResetPlaySpeed(SoundChannel& ChannelVar) {
	ChannelVar->setPitch(1.0);
}

void SoundUtil::SetFreqCutOff(SoundChannel& ChannelVar, float Frequency) {
	LowPass->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, Frequency);
	ChannelVar->addDSP(0, LowPass);
}

void SoundUtil::SetBeatDetect(SoundChannel& ChannelVar) {
	SoundSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &BeatDetector);
	ChannelVar->addDSP(0, BeatDetector);
}

float SoundUtil::DetectBeat(float Threshold, int SamplingRate) {
	FMOD_DSP_PARAMETER_FFT* FFT = nullptr;
	BeatDetector->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&FFT, 0, 0, 0);

	if (FFT) {
		int NumChannels = FFT->numchannels;

		if (NumChannels > 0) {
			memcpy(FFTdata.data(), FFT->spectrum[0], FFT_SIZE * sizeof(float));

			float BassEnergy = 0.0f;

			for (int i = 0; i < SamplingRate; ++i)
				BassEnergy += FFTdata[i];

			if (BassEnergy > Threshold)
				return BassEnergy;
		}
	}

	return 0;
}

void SoundUtil::DetectBeat(float& DestValue, float Threshold, int SamplingRate) {
	FMOD_DSP_PARAMETER_FFT* FFT = nullptr;
	BeatDetector->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&FFT, 0, 0, 0);

	if (FFT) {
		int NumChannels = FFT->numchannels;

		if (NumChannels > 0) {
			memcpy(FFTdata.data(), FFT->spectrum[0], FFT_SIZE * sizeof(float));

			float BassEnergy = 0.0f;

			for (int i = 0; i < SamplingRate; ++i)
				BassEnergy += FFTdata[i];

			if (BassEnergy > Threshold)
				DestValue = BassEnergy;
		}
	}

	DestValue = 0.0;
}

bool SoundUtil::IsBeat(float Threshold, int SamplingRate) {
	FMOD_DSP_PARAMETER_FFT* FFT = nullptr;
	BeatDetector->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&FFT, 0, 0, 0);

	if (FFT) {
		int NumChannels = FFT->numchannels;

		if (NumChannels > 0) {
			memcpy(FFTdata.data(), FFT->spectrum[0], FFT_SIZE * sizeof(float));

			float BassEnergy = 0.0f;

			for (int i = 0; i < SamplingRate; ++i)
				BassEnergy += FFTdata[i];

			if (BassEnergy > Threshold)
				return true;
		}
	}

	return false;
}

void SoundUtil::DisableBeatDetect(SoundChannel& ChannelVar) {
	ChannelVar->removeDSP(BeatDetector);
}

void SoundUtil::DisableFreqCutOff(SoundChannel& ChannelVar) {
	ChannelVar->removeDSP(LowPass);
}

void SoundUtil::StopAllSounds() {
	FMOD::ChannelGroup* MasterChannelGroup{};

	SoundSystem->getMasterChannelGroup(&MasterChannelGroup);
	Result = MasterChannelGroup->stop();
}

void SoundUtil::SetDistance(SoundChannel& ChannelVar, float MinDist, float MaxDist) {
	ChannelVar->set3DMinMaxDistance(MinDist, MaxDist);
}

void SoundUtil::SetListnerPosition(float X, float Y) {
	ListenerPosition.x = X;
	ListenerPosition.y = Y;
	ListenerPosition.z = 0.0f;

	SoundSystem->set3DListenerAttributes(0, &ListenerPosition, 0, 0, 0);
};

void SoundUtil::SetListnerPosition(glm::vec2 Position) {
	ListenerPosition.x = Position.x;
	ListenerPosition.y = Position.y;
	ListenerPosition.z = 0.0f;

	SoundSystem->set3DListenerAttributes(0, &ListenerPosition, 0, 0, 0);
}

void SoundUtil::SetSoundPosition(SoundChannel& ChannelVar, float X, float Y, float Diff) {
	SoundPosition.x = X;
	SoundPosition.y = Y;
	SoundPosition.z = Diff;

	ChannelVar->set3DAttributes(&SoundPosition, 0);
}

void SoundUtil::SetSoundPosition(SoundChannel& ChannelVar, glm::vec2 Position, float Diff) {
	SoundPosition.x = Position.x;
	SoundPosition.y = Position.y;
	SoundPosition.z = Diff;

	ChannelVar->set3DAttributes(&SoundPosition, 0);
}