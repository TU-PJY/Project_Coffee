#pragma once
#include "SDKHeader.h"

class TimerUtil {
private:
	GLfloat Time{};
	bool    StopState{};
	
public:
	void Update(float FT);
	void Stop();
	void Resume();
	void Reset();
	int Sec();
	GLfloat MiliSec(int DemicalPlace=1);
	void Interpolate(GLfloat Value);
	bool CheckSec(int DestTime, int CheckOption);
	bool CheckMiliSec(GLfloat DestTime, int DemicalPlace, int CheckOption);
};



class Metronome {
private:
	GLdouble BeatInterval{};
	GLdouble Time{};
	bool     StopState{};
	bool     BeatState{};

public:
	void SetBPM(int Value);
	void Update(float FT);
	void Stop();
	void Resume();
	void Reset();
	bool IsBeat();
};