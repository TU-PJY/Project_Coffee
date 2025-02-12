#include "CameraController.h"
#include "CameraUtil.h"
#include "TransformUtil.h"
#include "RandomUtil.h"
#include "MathUtil.h"
#include "EXUtil.h"
#include "TimerUtil.h"
#include "Scene.h"
glm::vec2 CameraPosition;
GLfloat CameraRotation;
GLfloat CameraZoom;

CameraController cameraControl;
GLfloat ShakeValue;

glm::vec2 ShakePosition;
glm::vec2 DestShakePosition;

TimerUtil ShakeTimer{};

void CameraController::Update(float FrameTime){
	if (scene.Mode() == "PlayMode") {
		if (ShakeValue > 0.0) {
			ShakeTimer.Update(FrameTime);
			if (ShakeTimer.CheckMiliSec(0.01, 2, CHECK_AND_INTERPOLATE)) {
				DestShakePosition.x = randomUtil.Gen(RANDOM_TYPE_REAL, -ShakeValue, ShakeValue);
				DestShakePosition.y = randomUtil.Gen(RANDOM_TYPE_REAL, -ShakeValue, ShakeValue);
			}

			ShakeValue -= FrameTime * 0.8;
			EX.ClampValue(ShakeValue, 0.0, CLAMP_LESS);
		}
		else {
			DestShakePosition = glm::vec2(0.0, 0.0);
			ShakeTimer.Reset();
		}

		mathUtil.Lerp(ShakePosition, DestShakePosition, 10.0, FrameTime);

		CameraPosition += ShakePosition;
		MoveCamera(CameraPosition);
	}

	else
		ShakePosition = glm::vec2(0.0, 0.0);

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

void CameraController::AddShakeValue(GLfloat Value) {
	ShakeValue += Value;
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