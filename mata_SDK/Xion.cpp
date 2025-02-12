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
	// ������ 0���� ��� ǥ���� ��ȭ�� �����
	if (Glb.GameOver && Glb.Score == 0) {
		ShakePosition.x = 0.0;
		ShakePosition.y = 0.0;

		VerticalSize = VerticalLoop.Update(0.05, 5.0, FrameTime);
		Position.x = -1.3;
		Frame = Xion_Curious;
	}

	// ��ǥ �������� �̵� �� �� �����
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

				// �ÿ��� �ٴڿ� �Ѿ����� �̵��� �̼��� ���ƿ��鼭 ���ӿ��� �ȴ�
				if (Position.y <= -1.3) {
					Position.y = -1.3;
					soundUtil.Play(Snd.CartCrash, SndChannel);
					FellDown = true;
				}
			}

			// 1�� �� ���ӿ��� �ȴ�
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

	// ���ӿ��� ���� ����
	Glb.Ending = GameOver_HitXion;

	soundUtil.Play(Snd.PeopleHit, SndChannel);
	HitState = true;
	ObjectTag = "";
}

// �ÿ��� �ڷ� �δ�
void Xion::PushPeople() {
	soundUtil.Play(Snd.Whoosh, SndChannel);
	DestPosition = Position.x - 2.5;
	DestHRotation = 180.0;
	ObjectTag = "";

	PushState = true;

	// Ŀ�Ǹ� �ٽ� �μ� �� �ִ� ���·� ��ȯ�Ѵ�
	if (auto Shelf = scene.Find("shelf"); Shelf)
		Shelf->EnableCoffeeHit();
}

void Xion::SetFrame(int Value) {
	Frame = Value;
}
