#include "TimerUtil.h"
#include <cmath>

void TimerUtil::Update(float FT) {
	if(!StopState)
		Time += FT;
}

void TimerUtil::Stop() {
	StopState = true;
}

void TimerUtil::Resume() {
	StopState = false;
}

void TimerUtil::Reset() {
	Time = 0.0;
}

int TimerUtil::Sec() {
	return (int)Time;
}

GLfloat TimerUtil::MiliSec(int DemicalPlace) {
	float TruncateValue = pow(10, DemicalPlace);
	float TruncatedNum = std::floor(Time * TruncateValue) / TruncateValue;
	return TruncatedNum;
}

void TimerUtil::Interpolate(GLfloat Value) {
	GLfloat OverTime = Time - Value;
	Time = OverTime;
}

bool TimerUtil::CheckSec(int DestTime, int CheckOption) {
	if (Sec() >= DestTime) {
		switch (CheckOption) {
		case CHECK_AND_RESUME:
			return true;

		case CHECK_AND_RESET:
			Reset();
			return true;

		case CHECK_AND_INTERPOLATE:
			Interpolate((GLfloat)DestTime);
			return true;

		case CHECK_AND_STOP:
			Stop();
			return true;
		}
	}

	return false;
}

bool TimerUtil::CheckMiliSec(GLfloat DestTime, int DemicalPlace, int CheckOption) {
	if (MiliSec(DemicalPlace) >= DestTime) {
		switch (CheckOption) {
		case CHECK_AND_RESUME:
			return true;

		case CHECK_AND_RESET:
			Reset();
			return true;

		case CHECK_AND_INTERPOLATE:
			Interpolate(DestTime);
			return true;

		case CHECK_AND_STOP:
			Stop();
			return true;
		}
	}
	return false;
}


void Metronome::SetBPM(int BPM) {
	BeatInterval = 60.0 / (GLdouble)BPM;
}

void Metronome::Update(float FT) {
	if (!StopState) {
		Time += FT;

		if (Time >= BeatInterval) {
			GLdouble OverTime = Time - BeatInterval;
			Time = OverTime;
			BeatState = true;
		}

		else
			BeatState = false;
	}
}

void Metronome::Stop() {
	StopState = true;
	BeatState = false;
}

void Metronome::Resume() {
	StopState = false;
}

void Metronome::Reset() {
	Time = 0.0;
}

bool Metronome::IsBeat() {
	return BeatState;
}