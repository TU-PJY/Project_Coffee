#include "Xion.h"

Xion::Xion(GLfloat PositionValue, GLfloat DestPositionValue, bool BoolMoveState, int FrameValue) {
	Position.x = PositionValue;
	DestPosition = DestPositionValue;
	MoveState = BoolMoveState;
	VerticalLoop.SetValue(Preset::HalfNegative);
	TiltLoop.SetValue(Preset::HalfPositive);
	Frame = FrameValue;
}

void Xion::UpdateFunc(float FrameTime) {
	// 점수가 0점일 경우 표정에 변화가 생긴다
	if (Glb.GameOver && Glb.Score == 0) {
		ShakePosition.x = 0.0;
		ShakePosition.y = 0.0;

		VerticalSize = VerticalLoop.Update(0.05, 5.0, FrameTime);
		Position.x = -1.3;
		Frame = Xion_Curious;
	}

	// 목표 지점까지 이동 한 후 멈춘다
	if (MoveState && !HitState && !PushState) 
		mathUtil.Lerp(Position.x, DestPosition, 20.0, FrameTime);

	else if (PushState) {
		mathUtil.Lerp(Position.x, DestPosition, 8.0, FrameTime);
		mathUtil.Lerp(HRotation, DestHRotation, 8.0, FrameTime);

		if (HRotation >= 90.0)
			Frame = Xion_Cry2;
	}

	if (Frame == Xion_Blocking) {
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
				if (GameOverTimer.Sec() >= 1) 
					Glb.GameOver = true;
			}
		}
	}

	else if (Frame == Xion_Cry1 || Frame == Xion_Cry2) {
		ShakeTimer.Update(FrameTime);
		if (ShakeTimer.CheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE)) {
			ShakeValue.x = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
			ShakeValue.y = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
		}
	}

	else if (Frame == Xion_Curious) {
		ShakeValue.x = 0.0;
		ShakeValue.y = 0.0;
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
	transform.RotateH(MoveMatrix, HRotation);
	if (Frame == Xion_Blocking || Frame == Xion_Cry2)
		SetFlip(FLIP_TYPE_X);
	transform.Shear(MoveMatrix, TiltValue, 0.0);
	imageUtil.RenderStaticSpriteSheet(Img.Xion, Frame);
}

void Xion::HitPeople() {
	if (auto TimeWatch = scene.Find("time_watch"); TimeWatch)
		TimeWatch->Stop();

	if (auto Manager = scene.Find("play_mode_manager"); Manager)
		Manager->StopBGM();

	Glb.DestroyedItems[Item_Xion]++;

	// 게임오버 엔딩 지정
	Glb.Ending = GameOver_HitXion;

	soundUtil.Play(Snd.PeopleHit, SndChannel);
	HitState = true;
	ObjectTag = "";
}

// 시온을 뒤로 민다
void Xion::PushPeople() {
	soundUtil.Play(Snd.Whoosh, SndChannel);
	DestPosition = Position.x - 2.5;
	DestHRotation = 180.0;
	ObjectTag = "";

	PushState = true;

	// 커피를 다시 부술 수 있는 상태로 전환한다
	if (auto Shelf = scene.Find("shelf"); Shelf)
		Shelf->EnableCoffeeHit();
}

void Xion::SetFrame(int Value) {
	Frame = Value;
}
