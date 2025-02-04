#include "AnimationUtil.h"
#include "MathUtil.h"
#include "ExUtil.h"

GLfloat SinLoop::Update(GLfloat MoveScale, GLfloat Speed, float FrameTime) {
	Num += Speed * FrameTime;
	return sin(Num) * MoveScale;
}

void SinLoop::SetValue(GLfloat Value) {
	Num = Value;
}

void SinLoop::Reset() {
	Num = 0.0;
}


GLfloat SinLerp::Update(GLfloat Value, GLfloat Dest, GLfloat Speed, float FrameTime) {
	Num += FrameTime * Speed;
	EX.ClampValue(Num, Preset::MaxPositive, CLAMP_GREATER);
	GLfloat Progress = (sin(Num) - sin(Preset::MaxNegative)) / (sin(Preset::MaxPositive) - sin(Preset::MaxNegative));

	return Value + (Dest - Value) * Progress;
}

void SinLerp::Reset() {
	Num = Preset::MaxNegative;
}


GLfloat PopBounce::Update(GLfloat SizeDest, GLfloat ShakeScale, GLfloat SizeIncreaseSpeed, GLfloat ShakeSpeed, GLfloat ShakeReduceSpeed, float FrameTime) {
	Num3 += FrameTime * SizeIncreaseSpeed;

	if (Num3 >= SizeDest) {
		Num3 = SizeDest;
		Num1 += FrameTime * ShakeSpeed;
		mathUtil.Lerp(Num2, ShakeScale, ShakeReduceSpeed, FrameTime);
	}

	return Num3 + sin(Num1) * (ShakeScale - Num2);
}

void PopBounce::Reset(){
	Num1 = 0.0;
	Num2 = 0.0;
	Num3 = 0.0;
}


GLfloat ReverseLerp::Update(GLfloat Value, GLfloat Dest, GLfloat Speed, GLfloat IncreaseSpeed, float FrameTime) {
	GLfloat ReturnValue = Value;
	GLfloat Diff = Dest - Value;

	if (MoveState) {
		if (std::abs(Diff) < std::abs(Velocity)) {
			ReturnValue = Dest;
			Velocity = 0.0;
			Acc = 0.0;
			MoveState = false;
		}
		else {
			ReturnValue += Velocity * (Diff > 0 ? 1 : -1);
			Velocity += Acc * Speed * FrameTime;
			Acc += IncreaseSpeed * IncreaseSpeed * FrameTime;
		}
	}

	return ReturnValue;
}

void ReverseLerp::Reset() {
	Velocity = 0.0;
	Acc = 0.0;
	MoveState = true;
}
