#pragma once
#include "SDKHeader.h"
#include <string>

class SoundUtil {
private:
	FMOD::System* SoundSystem{};
	FMOD::DSP*    BeatDetector{};
	FMOD::DSP*    LowPass{};

	FMOD_VECTOR   ListenerPosition{};
	FMOD_VECTOR   SoundPosition{};

	FMOD_RESULT   Result{};
	void*         ExtDvData{};

public:
	void Init();
	void Load(Sound& Sound, std::string FileName, FMOD_MODE Option=FMOD_DEFAULT);
	void Release(Sound& Sound);
	void Update();
	void AddChannelToGroup(SoundChannelGroup& Group, Sound& Sound, SoundChannel& Channel);
	void DeleteChannelFromGroup(SoundChannelGroup& Group, SoundChannel& Channel);
	void ClearGroup(SoundChannelGroup& Group);
	void PlayGroup(SoundChannelGroup& Group);
	void StopGroup(SoundChannelGroup& Group);
	void PauseGroup(SoundChannelGroup& Group, bool Flag);
	void Play(Sound& Sound, SoundChannel& ChannelVar, unsigned int Ms=0);
	void Play(Sound& Sound, SoundChannel& ChannelVar, unsigned int Minutes, unsigned int Seconds);
	void PlayOnce(Sound& Sound, SoundChannel& ChannelVar, bool& FlagValue, unsigned int Ms=0);
	void PlayOnce(Sound& Sound, SoundChannel& ChannelVar, bool& FlagValue, unsigned int Minutes, unsigned int Seconds);
	void Pause(SoundChannel& ChannelVar, bool Flag);
	void Stop(SoundChannel& ChannelVar);
	unsigned int GetLength(Sound& Sound);
	unsigned int GetPlayTime(SoundChannel& ChannelVar);
	void SetPlaySpeed(SoundChannel& ChannelVar, float Speed);
	void ResetPlaySpeed(SoundChannel& ChannelVar);
	void SetFreqCutOff(SoundChannel& ChannelVar, float Frequency);
	void SetBeatDetect(SoundChannel& ChannelVar);
	float DetectBeat(float Threshold, int SamplingRate);
	void DetectBeat(float& DestValue, float Threshold, int SamplingRate);
	bool IsBeat(float Threshold, int SamplingRate);
	void DisableBeatDetect(SoundChannel& ChannelVar);
	void DisableFreqCutOff(SoundChannel& ChannelVar);
	void SetDistance(SoundChannel& ChannelVar, float MinDist, float MaxDist);
	void SetListnerPosition(float X, float Y);
	void SetListnerPosition(glm::vec2 Position);
	void SetSoundPosition(SoundChannel& ChannelVar, float X, float Y, float Diff);
	void SetSoundPosition(SoundChannel& ChannelVar, glm::vec2 Position, float Diff);
	void StopAllSounds();
};
extern SoundUtil soundUtil;
