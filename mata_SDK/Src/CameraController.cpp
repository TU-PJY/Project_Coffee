#include "CameraController.h"
#include "CameraUtil.h"
#include "TransformUtil.h"

glm::vec2 CameraPosition;
GLfloat CameraRotation;
GLfloat CameraZoom;

CameraController cameraControl;

void CameraController::Update(float FrameTime){
	// add logic here

	ComputeCameraMatrix();
}

void CameraController::MoveCamera(GLfloat X, GLfloat Y){
	Position.x = -X;
	Position.y = -Y;
	CameraPosition.x = X;
	CameraPosition.y = Y;
}

void CameraController::MoveCamera(glm::vec2& PositionValue){
	Position = -PositionValue;
	CameraPosition = PositionValue;
}

void CameraController::RotateCamera(GLfloat Degree){
	Rotation = -Degree;
	CameraRotation = Degree;
}

void CameraController::CameraZoom(int ZoomType, GLfloat ZoomValue){
	switch (ZoomType) {
	case ZOOM_IN:
		camera.ZoomValue = camera.ZoomValue / (1.0f - ZoomValue);
		break;

	case ZOOM_OUT:
		camera.ZoomValue = camera.ZoomValue * (1.0f - ZoomValue);
		break;
	}
}

void CameraController::ChangeCameraZoom(GLfloat ZoomValue){
	camera.ZoomValue = ZoomValue;
}

GLfloat CameraController::ComputeNextZoom(int ZoomType, GLfloat ZoomValue) {
	if (ZoomType == ZOOM_IN)
		return camera.ZoomValue / (1.0f - ZoomValue);
	else if (ZoomType == ZOOM_OUT)
		return camera.ZoomValue * (1.0f - ZoomValue);
	else
		return camera.ZoomValue;
}

///////////////////////////////////////// private

void CameraController::ComputeCameraMatrix(){
	transform.Identity(camera.CameraMatrix);
	transform.Rotate(camera.CameraMatrix, Rotation);
	transform.Move(camera.CameraMatrix, Position);
}