#include "Xion.h"

Xion::Xion(GLfloat PositionValue, bool BoolMoveState, int FrameValue) {
	Position = PositionValue;
	MoveState = BoolMoveState;
	VerticalLoop.SetValue(Preset::HalfNegative);
	TiltLoop.SetValue(Preset::HalfPositive);
	Frame = FrameValue;
}

void Xion::UpdateFunc(float FrameTime) {
	// 목표 지점까지 이동 한 후 멈춘다
	if (MoveState) {
		if (Position > DestPosition) {
			Position -= 10.0 * FrameTime;
			if (EX.CheckClampValue(Position, DestPosition, CLAMP_LESS))
				MoveState = false;
		}
	}

	if (Frame == Blocking) {
		VerticalSize = VerticalLoop.Update(0.05, 20.0, FrameTime);
		TiltValue = TiltLoop.Update(0.1, 20.0, FrameTime);
	}

	else if (Frame == Cry1) 
		VerticalSize = VerticalLoop.Update(0.05, 40.0, FrameTime);

	if (Position < CameraPosition.x - ASP(1.0) - 1.0)
		scene.DeleteObject(this);
}

void Xion::RenderFunc() {
	if (Position > CameraPosition.x + ASP(1.0) + 1.0)
		return;
	Begin();
	transform.Move(MoveMatrix, Position + TiltValue * 0.5, VerticalSize * 0.5);
	transform.Scale(MoveMatrix, 2.0, 2.0 + VerticalSize);
	transform.Shear(MoveMatrix, TiltValue, 0.0);
	imageUtil.RenderStaticSpriteSheet(Img.Xion, Frame);
}
