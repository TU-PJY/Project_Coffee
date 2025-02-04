#pragma once
#include "SDKHeader.h"

class MoveUtil {
public:
	void MoveStraight(GLfloat& Position, int MoveDirection, GLfloat Speed, float FT);
	void MoveStraight(GLfloat& Position, GLfloat Speed, float FT);
	void MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, int MoveDirection, GLfloat RotationValue, float FT, bool Plus90Option);
	void MoveForward(glm::vec2& Position, GLfloat Speed, int MoveDirection, GLfloat RotationValue, float FrameTime, bool Plus90Option);
	void MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, GLfloat RotationValue, float FT, bool Plus90Option);
	void MoveForward(glm::vec2& Position, GLfloat Speed, GLfloat RotationValue, float FrameTime, bool Plus90Option);
};

extern MoveUtil moveUtil;