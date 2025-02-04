#pragma once
#include "GameObject.h"

extern glm::vec2 CameraPosition;
extern GLfloat CameraRotation;
extern GLfloat CameraZoom;

class CameraController : public GameObject {
public:
	void InputKey(KeyEvent& Event);
	void UpdateFunc(float FrameTime);
	void MoveCamera(GLfloat X, GLfloat Y);
	void MoveCamera(glm::vec2& PositionValue);
	void RotateCamera(GLfloat Degree);
	void CameraZoom(int ZoomType, GLfloat ZoomValue);
	void ChangeCameraZoom(GLfloat ZoomValue);
	GLfloat ComputeNextZoom(int ZoomType, GLfloat ZoomValue);

private:
	void ComputeCameraMatrix();
};

extern GameObject* CameraControl;