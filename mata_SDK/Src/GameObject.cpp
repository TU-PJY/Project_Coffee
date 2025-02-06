#include "GameObject.h"
#include "CameraController.h"

void GameObject::Begin(int RenderType) {
	transform.Identity(ResultMatrix);
	transform.Identity(MoveMatrix);
	transform.Identity(RotateMatrix);
	transform.Identity(ScaleMatrix);
	transform.Identity(ImageAspectMatrix);
	transform.Identity(FlipMatrix);

	ObjectOpacityValue = 1.0f;
	ObjectBlurValue = 0.0;

	camera.SetCamera(RenderType);
}

void GameObject::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	ObjectColor.r = R;
	ObjectColor.g = G;
	ObjectColor.b = B;
}

void GameObject::SetColor(glm::vec3& Color) {
	ObjectColor.r = Color.r;
	ObjectColor.g = Color.g;
	ObjectColor.b = Color.b;
}

void GameObject::SetColorRGB(int R, int G, int B) {
	ObjectColor.r = (1.0f / 255.0f) * (GLfloat)R;
	ObjectColor.g = (1.0f / 255.0f) * (GLfloat)G;
	ObjectColor.b = (1.0f / 255.0f) * (GLfloat)B;
}

void GameObject::ComputeViewportPosition(GLfloat& DestX, GLfloat& DestY, bool ApplyAspect) {
	if (ApplyAspect)
		DestX = ASP(ViewportPosition().x);
	else
		DestX = ViewportPosition().x;
	DestY = ViewportPosition().y;
}

void GameObject::ComputeViewportPosition(glm::vec2& DestValue, bool ApplyAspect) {
	if (ApplyAspect) {
		DestValue.x = ASP(ViewportPosition().x);
		DestValue.y = ViewportPosition().y;
	}
	else
		DestValue = ViewportPosition();
}

void GameObject::ComputeLocalPosition(GLfloat& DestX, GLfloat& DestY) {
	DestX = LocalPosition().x;
	DestY = LocalPosition().y;
}

void GameObject::ComputeLocalPosition(glm::vec2& DestPosition) {
	DestPosition = LocalPosition();
}

void GameObject::SetFlip(int FlipOpt) {
	switch (FlipOpt) {
	case FLIP_TYPE_NONE:
		transform.Identity(FlipMatrix);
		break;

	case FLIP_TYPE_X:
		transform.RotateH(FlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_Y:
		transform.RotateV(FlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_XY:
		transform.RotateH(FlipMatrix, 180.0f);
		transform.RotateV(FlipMatrix, 180.0f);
		break;
	}
}

void GameObject::SetBlur(GLfloat Strength) {
	ObjectBlurValue = Strength;
}

void GameObject::SetUnitFlip(int FlipOpt) {
	switch (FlipOpt) {
	case FLIP_TYPE_NONE:
		transform.Identity(UnitFlipMatrix);
		break;

	case FLIP_TYPE_X:
		transform.RotateH(UnitFlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_Y:
		transform.RotateV(UnitFlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_XY:
		transform.RotateH(UnitFlipMatrix, 180.0f);
		transform.RotateV(UnitFlipMatrix, 180.0f);
		break;
	}
}

void GameObject::SetUnitOpacity(GLfloat Value) {
	UnitOpacityValue = Value;
}

void GameObject::SetUnitBlur(GLfloat Strength) {
	UnitBlurValue = Strength;
}

void GameObject::ResetUnitTransform() {
	transform.Identity(UnitMoveMatrix);
	transform.Identity(UnitRotateMatrix);
	transform.Identity(UnitScaleMatrix);
	transform.Identity(UnitFlipMatrix);

	UnitOpacityValue = 1.0f;
	UnitBlurValue = 0.0f;
}

////////////////////////// private

glm::vec4 GameObject::ViewportPosition() {
	computeUtil.ComputeMatrix(ViewportPositionMatrix, camera.Projection, camera.ViewMatrix, ResultMatrix);
	return ViewportPositionMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 GameObject::LocalPosition() {
	return ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}