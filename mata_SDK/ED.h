#pragma once
#include <Scene.h>
#include <CameraController.h>

#include "Cover.h"

class ED : public GameObject {
private:
	// ���� ��ġ, Ŀ�Ǹ� �μ� �� ���� ���������� �̵��Ѵ�.
	GLfloat Position{};

	// Ŀ�Ǹ� �μ� �� ��ǥ �̵� ��ġ
	GLfloat DestPosition{};

	// ī�޶� ��ġ
	GLfloat EDCameraPosition{};
	GLfloat EDCameraHeight{};

	// �� ���¸��� �ٸ� �������� ����Ѵ�
	int Frame = Idle;

	GLfloat PrevGeneratedFrame{};

	// ���� �����Ӱ� ���� ������
	int PrevFrame = Frame;

	// �յڷ� �þ�� ȿ���� �ش�
	GLfloat AnimationSize{};

	// ������ �������� ȿ���� �ش�
	GLfloat TiltValue{};

	// �̵� ������ �ִϸ��̼�
	GLfloat BreatheSize{};
	SinLoop BreatheLoop{};

	// ����ȸ����
	GLfloat HRotation{};

	// ���°� ����Ǹ� ���� �ð� ���� �ٽ� Idle ���·� �ϱ��ϵ��� �Ѵ�
	TimerUtil StateTimer{};

	// ���� �ִϸ��̼�
	glm::vec2 ShakeValue{};
	TimerUtil ShakeTimer{};

	// ���ӿ��� ���� Ÿ�̸�
	TimerUtil GameOverTimer{};

	// �̵� ���� ä��
	SoundChannel SndChannel{};
	SoundChannel SndChannel2{};
	        
	// �̵尡 Ŀ�Ǹ� �μ� Ƚ��
	int BreakCount{};

	// Ŀ�Ǹ� �ν����ϴ� ����, �ʱⰪ 7��
	int MaxBreak{7};

	// ���� ������ ���� ����
	GLfloat NextPosition{};

	// Ű�� ���� ����
	// �ϳ��� ���� Ű�� ������ ������ �� ����
	bool KeyPressed[4]{};

	// �ÿ��� �ڷ� �̴� ����
	bool PushState{};

	// ����Ű �Է� ������ ����, ���ӿ��� �Ǹ� ��Ȱ��ȭ �Ǿ� �� �̻� Ű�� �Է��� �� ����
	bool InputAvailable{true};

	// ���ӿ��� ���� ��� ����
	bool SoundPlayed{};

public:
	void InputKey(KeyEvent& Event) {
		// ��Ȱ��ȭ ���¿����� �Է��� ���� �ʴ´�.
		if (!InputAvailable)
			return;

		if (Event.Type == SPECIAL_KEY_DOWN) { 
			// 3���� Ű �� �ϳ��� ���� Ű�� ������ �������� �ʴ´�
			for (int i = 0; i < 4; i++)
				if (KeyPressed[i])
					return;

			if (Event.SpecialKey == SK_ARROW_LEFT)
				KeyPressed[0] = true;
			else if (Event.SpecialKey == SK_ARROW_DOWN)
				KeyPressed[1] = true;
			else if (Event.SpecialKey == SK_ARROW_RIGHT)
				KeyPressed[2] = true;
			else if (Event.SpecialKey == SK_ARROW_UP)
			{}
			else
				return;

			// ���� �տ� �ִ� Ŀ�Ǹ� �μ���. 
			if (auto Shelf = scene.Find("shelf"); Shelf) {
				bool IsCorrect{};

				ItemStruct Item = Shelf->GetFrontCoffee();

				// �տ� ����� ���ִٸ� Ŀ�ǿ� ��ȣ�ۿ� �� �� ����.
				if (Item.IsPeopleFront)
					return;

				// �ÿ��� ���ִٸ� Ŀ�ǿ� ��ȣ�ۿ� �� �� ������ �ÿ°� ��ȣ�ۿ� �� �� �ִ�.
				else if (Item.IsXionFront) {
					// �ƹ�Ű�� ������ �ÿ��� ������
					if (auto Xion = scene.Find("xion"); Xion) {
						if (Event.SpecialKey != SK_ARROW_UP) {
							Xion->HitPeople();

							StateTimer.Reset();
							AnimationSize = 2.0;

							if (Item.IsUpside)
								Frame = randomUtil.Gen(RANDOM_TYPE_INT, HitLow1, HitLow2);
							else
								Frame = randomUtil.Gen(RANDOM_TYPE_INT, HitHigh1, HitHigh2);
						}
						else {
							StateTimer.Reset();
							PushState = true;
							AnimationSize = 1.0;

							// �ÿ��� �ڷ� �δ�.
							Xion->PushPeople();

							if (auto Score = scene.Find("score_indicator"); Score)
								Score->AddScore(100);

							Frame = HitHigh2;
						}
					}
				}

				else {
					if (Event.SpecialKey == SK_ARROW_UP)
						return;

					if (PushState)
						PushState = false;

					StateTimer.Reset();
					AnimationSize = 1.0;

					if (Item.Type == Can && Event.SpecialKey == SK_ARROW_LEFT)
						IsCorrect = true;
					else if (Item.Type == Box && Event.SpecialKey == SK_ARROW_DOWN)
						IsCorrect = true;
					else if (Item.Type == Glass && Event.SpecialKey == SK_ARROW_RIGHT)
						IsCorrect = true;

					// ������ �´� Ű�� ������ �μ� �� �ִ�.
					if (IsCorrect) {
						soundUtil.Stop(SndChannel);
						soundUtil.Play(Snd.Whoosh, SndChannel);

						if (Item.IsUpside)
							Frame = randomUtil.Gen(RANDOM_TYPE_INT, HitHigh1, HitHigh2);
						else
							Frame = randomUtil.Gen(RANDOM_TYPE_INT, HitLow1, HitLow2);

						DestPosition += 0.5;

						Shelf->BreakCoffee();
						BreakCount++;

						// Ŀ�Ǹ� �� �μ��� ���� �������� �̵��Ѵ�
						// �ν����� Ŀ�Ǵ� 4�� �����Ѵ�
						if (BreakCount == MaxBreak) {
							DestPosition = NextPosition;
							BreakCount = 0;
							MaxBreak += 4;

							Glb.ShelfCount++;

							// �ϳ��� ������ ���� ������ 10���� �߰� �ð��� ��´�.
							if (auto TimeWatch = scene.Find("time_watch"); TimeWatch)
								TimeWatch->AddTime();

							if (auto Score = scene.Find("score_indicator"); Score)
								Score->AddScore(200);

							TiltValue = 3.0;

							soundUtil.Stop(SndChannel2);
							soundUtil.Play(Snd.NextWhoosh, SndChannel2);
						}
					}

					// �´� Ű�� �ƴ� ��� ������ ���� ģ��.
					else {
						soundUtil.Stop(SndChannel);
						soundUtil.Play(Snd.MissWhoosh, SndChannel);

						if (Item.IsUpside)
							Frame = randomUtil.Gen(RANDOM_TYPE_INT, HitLow1, HitLow2);
						else
							Frame = randomUtil.Gen(RANDOM_TYPE_INT, HitHigh1, HitHigh2);
					}
				}
			}
		}

		// Ű �Է��� ��� �ߴ��ؾ� �ٸ� Ű�� �Է��� �� �ִ�
		if (Event.Type == SPECIAL_KEY_UP) {
			if (Event.SpecialKey == SK_ARROW_LEFT)
				KeyPressed[0] = false;
			else if (Event.SpecialKey == SK_ARROW_DOWN)
				KeyPressed[1] = false;
			else if (Event.SpecialKey == SK_ARROW_RIGHT)
				KeyPressed[2] = false;
		}

		if (Event.Type == NORMAL_KEY_DOWN && Event.NormalKey == NK_SPACE) {
			for (int i = 0; i < 4; i++)
				if (KeyPressed[i])
					return;

			KeyPressed[3] = true;

			// �տ� �ִ� ����� �߷� ����.
			if (auto Shelf = scene.Find("shelf"); Shelf) {
				ItemStruct Item = Shelf->GetFrontCoffee();

				if (Item.IsPeopleFront) {
					if (auto People = scene.Find("people"); People) {
						soundUtil.Stop(SndChannel);
						soundUtil.Play(Snd.Whoosh, SndChannel);

						StateTimer.Reset();
						AnimationSize = 2.0;

						People->HitPeople();
						Frame = KickPeople;
					}
				}

				else if (Item.IsXionFront) {
					// �ÿ��� �տ� ���� �� �����̽��� ������ �ÿ��� ������
					if (auto Xion = scene.Find("xion"); Xion) {
						Xion->HitPeople();

						StateTimer.Reset();
						AnimationSize = 2.0;

						Frame = KickPeople;
					}
				}
			}
		}

		if (Event.Type == NORMAL_KEY_UP && Event.NormalKey == NK_SPACE) {
			KeyPressed[3] = false;
		}
	}

	void UpdateFunc(float FrameTime) {
		// ������
		// ���� �����Ӱ� ���� �������� �ٸ� ��� ���� �������� �����ϰ� �ִϸ��̼� ����ϵ��� �Ѵ�
		if (!Glb.GameOver) {
			if (PrevFrame != Frame) 
				PrevFrame = Frame;

			// ���� ���°� Idle�� �ƴ϶�� 0.3�� �� �ٽ� Idle ���·� ���ͽ�Ų��
			if (Frame != Idle) {
				StateTimer.Update(FrameTime);
				if (StateTimer.CheckMiliSec(0.3, 2, CHECK_AND_RESET)) {
					Frame = Idle;
					// �ÿ� �б� ���� ����
					PushState = false;
				}
			}

			if (PushState)
				mathUtil.Lerp(HRotation, 180.0, 8.0, FrameTime);
			else
				mathUtil.Lerp(HRotation, 0.0, 15.0, FrameTime);

			// �ִϸ��̼�
			// AnimationSize�� 0.0���� �۴ٸ� �ٽ� 0.0���� ���ͽ�Ų��
			mathUtil.Lerp(AnimationSize, 0.0, 15.0, FrameTime);

			// TiltValue�� 0.0���� ũ�ٸ� �ٽ� 0.0���� ���ͽ�Ų��
			mathUtil.Lerp(TiltValue, 0.0, 5.0, FrameTime);

			// �̵��� ������ �ִϸ��̼��� ������Ʈ �Ѵ�
			BreatheSize = BreatheLoop.Update(0.03, 6.0, FrameTime);


			// �̵�
			// ��ǥ ��ġ�� �̵��ϵ��� �Ѵ�
			mathUtil.Lerp(Position, DestPosition, 20.0, FrameTime);

			// ī�޶� �̵带 �ε巴�� ��������� �Ѵ�
			mathUtil.Lerp(EDCameraPosition, DestPosition + 0.5, 7.0, FrameTime);

			// �̵带 �ణ �����ʿ��� �ٶ󺸵��� �Ѵ�
			cameraControl.MoveCamera(EDCameraPosition, 0.0);
		}

		else {
			// �Է� ���� ���� ��Ȱ��ȭ
			InputAvailable = false;
			soundUtil.PlayOnce(Snd.GameOver, SndChannel, SoundPlayed);

			// �̵忡 ��Ŀ�� �Ѵ�.
			mathUtil.Lerp(EDCameraPosition, Position, 5.0, FrameTime);
			mathUtil.Lerp(EDCameraHeight, 0.4, 5.0, FrameTime);
			cameraControl.MoveCamera(EDCameraPosition, EDCameraHeight);

			// ������ 0���� ��� �����ӿ� ��ȭ�� ���� �ʴ´�.
			if (Glb.Score > 0) {
				TiltValue = 0.0;
				AnimationSize = 0.0;
				BreatheSize = 0.0;

				Frame = GameOver;

				ShakeTimer.Update(FrameTime);
				if (ShakeTimer.CheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE)) {
					ShakeValue.x = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
					ShakeValue.y = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				}

				mathUtil.Lerp(camera.ZoomValue, 1.8, 5.0, FrameTime);
			}

			else {
				Frame = Idle;

				// �̵��� ������ �ִϸ��̼��� ������Ʈ �Ѵ�
				BreatheSize = BreatheLoop.Update(0.03, 6.0, FrameTime);
			}

			// 3�� �� ȭ���� ��ο�����
			GameOverTimer.Update(FrameTime);
			if (GameOverTimer.Sec() >= 4) {
				scene.AddObject(new Cover, "cover", LAYER7);
				GameOverTimer.Reset();
				GameOverTimer.Stop();
			}
		}
	}

	void RenderFunc() {
		glm::vec2 FinalPosition { Position + AnimationSize * 0.5 + TiltValue * 0.5 + ShakeValue.x , BreatheSize * 0.5 - AnimationSize * 0.25 + ShakeValue.y};
		glm::vec2 FinalSize{ 2.0 + AnimationSize, 2.0 + BreatheSize - AnimationSize * 0.5};

		Begin();
		transform.Move(MoveMatrix, FinalPosition);
		transform.Scale(MoveMatrix, FinalSize);
		transform.RotateH(MoveMatrix, HRotation);
		transform.Shear(MoveMatrix, TiltValue, 0.0);
		imageUtil.RenderStaticSpriteSheet(Img.ED, Frame);
	}

	GLfloat GetPosition() {
		return Position;
	}

	// ���� ������ ���� ������ �̸� �˸���
	void TellNextPosition(GLfloat Position) {
		NextPosition = Position;
	}

	// ����Ű �Է� ��Ȱ��ȭ
	void DisableInput() {
		InputAvailable = false;
	}
};