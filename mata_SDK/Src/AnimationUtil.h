#pragma once
#include "SDKHeader.h"
#include <cmath>

class SinLoop {
private:
	GLfloat Num{ Preset::MaxNegative };

public:
	GLfloat Update(GLfloat MoveScale, GLfloat Speed, float FrameTime);
	void SetValue(GLfloat Value);
	void Reset();
};

class SinLerp{
private:
	GLfloat Num{ Preset::MaxNegative };

public:
	GLfloat Update(GLfloat Value, GLfloat Dest, GLfloat Speed, float FrameTime);
	void Reset();
};

class PopBounce {
private:
	GLfloat Num1{}, Num2{}, Num3{};

public:
	GLfloat Update(GLfloat SizeDest, GLfloat ShakeValue, GLfloat SizeIncreaseSpeed, GLfloat ShakeSpeed, GLfloat ShakeReduceSpeed, float FrameTime);
	void Reset();
};

class ReverseLerp {
private:
	bool    MoveState{ true };
	GLfloat Velocity{};
	GLfloat Acc{};

public:
	GLfloat Update(GLfloat Value, GLfloat Dest, GLfloat Speed, GLfloat IncreaseSpeed, float FrameTime);
	void Reset();
};
