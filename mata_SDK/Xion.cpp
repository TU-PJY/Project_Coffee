#include "Xion.h"

Xion::Xion(GLfloat PositionValue, GLfloat DestPositionValue, bool BoolMoveState, int FrameValue) {
	Position.x = PositionValue;
	DestPosition = DestPositionValue;
	MoveState = BoolMoveState;
	VerticalLoop.SetValue(Preset::HalfNegative);
	TiltLoop.SetValue(Preset::HalfPositive);
	Frame = FrameValue;

	if(Frame == Blocking)
		soundUtil.Play(Snd.MissWhoosh, SndChannel);
}

void Xion::UpdateFunc(float FrameTime) {
	// 목표 지점까지 이동 한 후 멈춘다
	if (MoveState && !HitState) 
		mathUtil.Lerp(Position.x, DestPosition, 15.0, FrameTime);

	if (Frame == Blocking) {
		if (!HitState) {
			VerticalSize = VerticalLoop.Update(0.05, 20.0, FrameTime);
			TiltValue = TiltLoop.Update(0.1, 20.0, FrameTime);
		}
		else {
			if (!FellDown) {
				VerticalSize = 0.0;
				TiltValue = 0.0;

				Rotation += 250 * FrameTime;
				Position.x += 8.0 * FrameTime;
				Position.y -= 1.0 * FrameTime;

				// 시온이 바닥에 넘어지면 이드의 이성이 돌아오면서 게임오버 된다
				if (Position.y <= -1.3) {
					Position.y = -1.3;
					soundUtil.Play(Snd.CartCrash, SndChannel);
					FellDown = true;
				}
			}

			// 1초 후 게임오버 된다
			else {
				GameOverTimer.Update(FrameTime);
				if (GameOverTimer.Sec() >= 1) {
					if (auto ED = scene.Find("ed"); ED)
						ED->SetFrame(GameOver);
					GameOverTimer.Stop();
				}
			}
		}
	}

	else if (Frame == Cry1) {
		ShakeTimer.Update(FrameTime);
		if (ShakeTimer.CheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE)) {
			ShakeValue.x = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
			ShakeValue.y = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
		}
	}
	
	if (Position.x < CameraPosition.x - ASP(1.0) - 1.0)
		scene.DeleteObject(this);
}

void Xion::RenderFunc() {
	if (Position.x > CameraPosition.x + ASP(1.0) + 1.0)
		return;

	Begin();
	transform.Move(MoveMatrix, Position.x + TiltValue * 0.5 + ShakeValue.x, Position.y + VerticalSize * 0.5 + ShakeValue.y);
	transform.Scale(MoveMatrix, 2.0, 2.0 + VerticalSize);
	transform.Rotate(MoveMatrix, Rotation);
	if (Frame == Blocking)
		SetFlip(FLIP_TYPE_X);
	transform.Shear(MoveMatrix, TiltValue, 0.0);
	imageUtil.RenderStaticSpriteSheet(Img.Xion, Frame);
}

void Xion::HitPeople() {
	// 이드의 컨트롤러를 비활성화한다
	if (auto ED = scene.Find("ed"); ED)
		ED->DisableInput();

	soundUtil.Play(Snd.PeopleHit, SndChannel);
	HitState = true;
	ObjectTag = "";
}
