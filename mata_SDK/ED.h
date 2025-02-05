#pragma once
#include <Scene.h>
#include <CameraController.h>

class ED : public GameObject {
private:
	// 현재 위치, 커피를 부술 때 마다 오른쪽으로 이동한다.
	GLfloat Position{};

	// 커피를 부순 후 목표 이동 위치
	GLfloat DestPosition{};

	// 카메라 위치
	GLfloat CameraPosition{};

	// 각 상태마다 다른 프레임을 출력한다
	GLfloat Frame = Idle;

	GLfloat PrevGeneratedFrame{};

	// 현재 프레임과 이전 프레임
	GLfloat PrevFrame = Frame;

	// 앞뒤로 늘어나는 효과를 준다
	GLfloat AnimationSize{};

	// 앞으로 기울어지는 효과를 준다
	GLfloat TiltValue{};

	// 이드 숨쉬기 애니메이션
	GLfloat BreatheSize{};
	SinLoop BreatheLoop{};

	// 상태가 변경되면 일정 시간 이후 다시 Idle 상태로 북귀하도록 한다
	TimerUtil StateTimer{};

	// 이드 사운드 채널
	SoundChannel SndChannel{};

	SoundChannel SndChannel2{};
	        
	// 이드가 커피를 부순 횟수
	int BreakCount{};

	// 커피를 부숴야하는 개수, 초기값 7개
	int MaxBreak{7};

	// 다음 선반의 시작 지점
	GLfloat NextPosition{};

public:
	void InputKey(KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN && Event.NormalKey == NK_SPACE) { 
			soundUtil.Stop(SndChannel);
			soundUtil.Play(Snd.Whoosh, SndChannel);

			StateTimer.Reset();

			DestPosition += 0.5;
			AnimationSize = 1.0;

			// 가장 앞에 있는 커피를 부순다. 
			if (auto Shelf = scene.Find("shelf"); Shelf) {
				// 커피가 위에 있을 경우
				if (Shelf->GetFrontCoffee()) 
					Frame = randomUtil.Gen(RANDOM_TYPE_INT, HitHigh1, HitHigh2);
				// 아래에 있을 경우
				else 
					Frame = randomUtil.Gen(RANDOM_TYPE_INT, HitLow1, HitLow2);

				//  이전 프레임 갱신
				PrevFrame = Frame;

				// 커피 파괴
				Shelf->BreakCoffee();
			}

			// 부순 커피 횟수 증가
			BreakCount++;

			// 커피를 모두 부수면 다음 선반으로 이동하고 부숴야하는 개수가 4 증가한다.
			// 앞으로 늘어나는 애니메이션을 재생한다.
			if (BreakCount >= MaxBreak) {
				DestPosition = NextPosition;
				BreakCount = 0;
				MaxBreak += 4;

				TiltValue = 2.0;

				soundUtil.Stop(SndChannel2);
				soundUtil.Play(Snd.NextWhoosh, SndChannel2);
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		// 프레임
		// 이전 프레임과 현재 프레임이 다를 경우 이전 프레임을 갱신하고 애니메이션 출력하도록 한다
		// 단, 현재 프레임이 Idle일 경우 AnimationSize를 변경하지 않는다
		if ((int)PrevFrame != (int)Frame) {
			if ((int)Frame != Idle) 
				AnimationSize = -0.7;
			PrevFrame = Frame;
		}
		
		// 현재 상태가 Idle이 아니라면 0.3초 후 다시 Idle 상태로 복귀시킨다
		if ((int)Frame != Idle) {
			StateTimer.Update(FrameTime);
			if (StateTimer.CheckMiliSec(0.3, 2, CHECK_AND_RESET)) 
				Frame = Idle; 
		}


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
		mathUtil.Lerp(CameraPosition, -DestPosition, 7.0, FrameTime);

		// 이드를 약간 오른쪽에서 바라보도록 한다
		CameraControl->MoveCamera(CameraPosition - 0.5, 0.0);
	}

	void RenderFunc() {
		glm::vec2 FinalPosition { Position + AnimationSize * 0.5 + TiltValue * 0.5 , BreatheSize * 0.5 };
		glm::vec2 FinalSize{ 2.0+ AnimationSize, 2.0 + BreatheSize };

		Begin();
		transform.Move(MoveMatrix, FinalPosition);
		transform.Scale(MoveMatrix, FinalSize);
		transform.Shear(MoveMatrix, TiltValue, 0.0);
		imageUtil.RenderSpriteSheet(Img.ED, Frame);
	}

	GLfloat GetPosition() {
		return Position;
	}

	// 다음 선반의 시작 지점을 미리 알린다
	void TellNextPosition(GLfloat Position) {
		NextPosition = Position;
	}
};