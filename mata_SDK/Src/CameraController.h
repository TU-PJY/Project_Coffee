#pragma once
#include "CameraUtil.h"

extern glm::vec2 CameraPosition;
extern GLfloat CameraRotation;
extern GLfloat CameraZoom;

class CameraController {
public:
	void Update(float FrameTime);
	void MoveCamera(GLfloat X, GLfloat Y);
	void MoveCamera(glm::vec2& PositionValue);
	void RotateCamera(GLfloat Degree);
	void CameraZoom(int ZoomType, GLfloat ZoomValue);
	void ChangeCameraZoom(GLfloat ZoomValue);
	GLfloat ComputeNextZoom(int ZoomType, GLfloat ZoomValue);

private:
	glm::vec2 Position{};
	GLfloat Rotation{};

	void ComputeCameraMatrix();
};

extern CameraController cameraControl;