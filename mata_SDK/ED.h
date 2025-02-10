#pragma once
#include <Scene.h>
#include <CameraController.h>

#include "Cover.h"

class ED : public GameObject {
private:
	// 현재 위치, 커피를 부술 때 마다 오른쪽으로 이동한다.
	GLfloat Position{};

	// 커피를 부순 후 목표 이동 위치
	GLfloat DestPosition{};

	// 카메라 위치
	GLfloat EDCameraPosition{};
	GLfloat EDCameraHeight{};

	// 각 상태마다 다른 프레임을 출력한다
	int Frame = Idle;

	GLfloat PrevGeneratedFrame{};

	// 현재 프레임과 이전 프레임
	int PrevFrame = Frame;

	// 앞뒤로 늘어나는 효과를 준다
	GLfloat AnimationSize{};

	// 앞으로 기울어지는 효과를 준다
	GLfloat TiltValue{};

	// 이드 숨쉬기 애니메이션
	GLfloat BreatheSize{};
	SinLoop BreatheLoop{};

	// 수평회전값
	GLfloat HRotation{};

	// 상태가 변경되면 일정 시간 이후 다시 Idle 상태로 북귀하도록 한다
	TimerUtil StateTimer{};

	// 떨림 애니메이션
	glm::vec2 ShakeValue{};
	TimerUtil ShakeTimer{};

	// 게임오버 지연 타이머
	TimerUtil GameOverTimer{};

	// 이드 사운드 채널
	SoundChannel SndChannel{};
	SoundChannel SndChannel2{};
	        
	// 이드가 커피를 부순 횟수
	int BreakCount{};

	// 커피를 부숴야하는 개수, 초기값 7개
	int MaxBreak{7};

	// 다음 선반의 시작 지점
	GLfloat NextPosition{};

	// 키가 눌린 상태
	// 하나라도 눌린 키가 있으면 조작할 수 없다
	bool KeyPressed[4]{};

	// 시온을 뒤로 미는 상태
	bool PushState{};

	// 조작키 입력 가능한 상태, 게임오버 되면 비활성화 되어 더 이상 키를 입력할 수 없다
	bool InputAvailable{true};

	// 게임오버 사운드 재생 여부
	bool SoundPlayed{};

public:
	void InputKey(KeyEvent& Event) {
		// 비활성화 상태에서는 입력을 받지 않는다.
		if (!InputAvailable)
			return;

		if (Event.Type == SPECIAL_KEY_DOWN) { 
			// 3개의 키 중 하나라도 눌린 키가 있으면 동작하지 않는다
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

			// 가장 앞에 있는 커피를 부순다. 
			if (auto Shelf = scene.Find("shelf"); Shelf) {
				bool IsCorrect{};

				ItemStruct Item = Shelf->GetFrontCoffee();

				// 앞에 사람이 서있다면 커피와 상호작용 할 수 없다.
				if (Item.IsPeopleFront)
					return;

				// 시온이 서있다면 커피와 상호작용 할 수 없지만 시온과 상호작용 할 수 있다.
				else if (Item.IsXionFront) {
					// 아무키나 누르면 시온을 때린다
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

							// 시온을 뒤로 민다.
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

					// 종류에 맞는 키를 눌러야 부술 수 있다.
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

						// 커피를 다 부수면 다음 선반으로 이동한다
						// 부숴야할 커피는 4개 증가한다
						if (BreakCount == MaxBreak) {
							DestPosition = NextPosition;
							BreakCount = 0;
							MaxBreak += 4;

							Glb.ShelfCount++;

							// 하나의 선반을 지날 때마다 10초의 추가 시간을 얻는다.
							if (auto TimeWatch = scene.Find("time_watch"); TimeWatch)
								TimeWatch->AddTime();

							if (auto Score = scene.Find("score_indicator"); Score)
								Score->AddScore(200);

							TiltValue = 3.0;

							soundUtil.Stop(SndChannel2);
							soundUtil.Play(Snd.NextWhoosh, SndChannel2);
						}
					}

					// 맞는 키가 아닐 경우 엉뚱한 곳을 친다.
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

		// 키 입력을 모두 중단해야 다른 키를 입력할 수 있다
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

			// 앞에 있는 사람을 발로 찬다.
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
					// 시온이 앞에 있을 때 스페이스를 누르면 시온을 때린다
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
		// 프레임
		// 이전 프레임과 현재 프레임이 다를 경우 이전 프레임을 갱신하고 애니메이션 출력하도록 한다
		if (!Glb.GameOver) {
			if (PrevFrame != Frame) 
				PrevFrame = Frame;

			// 현재 상태가 Idle이 아니라면 0.3초 후 다시 Idle 상태로 복귀시킨다
			if (Frame != Idle) {
				StateTimer.Update(FrameTime);
				if (StateTimer.CheckMiliSec(0.3, 2, CHECK_AND_RESET)) {
					Frame = Idle;
					// 시온 밀기 상태 해제
					PushState = false;
				}
			}

			if (PushState)
				mathUtil.Lerp(HRotation, 180.0, 8.0, FrameTime);
			else
				mathUtil.Lerp(HRotation, 0.0, 15.0, FrameTime);

			// 애니메이션
			// AnimationSize가 0.0보다 작다면 다시 0.0으로 복귀시킨다
			mathUtil.Lerp(AnimationSize, 0.0, 15.0, FrameTime);

			// TiltValue가 0.0보다 크다면 다시 0.0으로 복귀시킨다
			mathUtil.Lerp(TiltValue, 0.0, 5.0, FrameTime);

			// 이드의 숨쉬기 애니메이션을 업데이트 한다
			BreatheSize = BreatheLoop.Update(0.03, 6.0, FrameTime);


			// 이동
			// 목표 위치로 이동하도록 한다
			mathUtil.Lerp(Position, DestPosition, 20.0, FrameTime);

			// 카메라가 이드를 부드럽게 따라오도록 한다
			mathUtil.Lerp(EDCameraPosition, DestPosition + 0.5, 7.0, FrameTime);

			// 이드를 약간 오른쪽에서 바라보도록 한다
			cameraControl.MoveCamera(EDCameraPosition, 0.0);
		}

		else {
			// 입력 가능 상태 비활성화
			InputAvailable = false;
			soundUtil.PlayOnce(Snd.GameOver, SndChannel, SoundPlayed);

			// 이드에 포커싱 한다.
			mathUtil.Lerp(EDCameraPosition, Position, 5.0, FrameTime);
			mathUtil.Lerp(EDCameraHeight, 0.4, 5.0, FrameTime);
			cameraControl.MoveCamera(EDCameraPosition, EDCameraHeight);

			// 점수가 0점일 경우 프레임에 변화를 주지 않는다.
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

				// 이드의 숨쉬기 애니메이션을 업데이트 한다
				BreatheSize = BreatheLoop.Update(0.03, 6.0, FrameTime);
			}

			// 3초 후 화면이 어두워진디
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

	// 다음 선반의 시작 지점을 미리 알린다
	void TellNextPosition(GLfloat Position) {
		NextPosition = Position;
	}

	// 조작키 입력 비활성화
	void DisableInput() {
		InputAvailable = false;
	}
};