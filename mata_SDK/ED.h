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

	// 현재 프레임과 이전 프레임이 다르면 살짝 눌리는 애니메이션을 재생한다
	GLfloat PrevFrame = Frame;
	GLfloat AnimationSize{};

	// 상태가 변경되면 일정 시간 이후 다시 Idle 상태로 북귀하도록 한다
	TimerUtil StateTimer{};

	// 이드 사운드 채널
	// 안정적인 빠른 재생을 위해 순환 채널을 사용한다
	SoundChannel SndChannel[5]{};
	int PlayChannel = 0;
	int StopChannel = 1;
	        
	// 이드가 커피를 부순 횟수
	int BreakCount{};

	// 커피를 부숴야하는 개수, 초기값 8개
	int MaxBreak{8};

	// 다음 선반의 시작 지점
	GLfloat NextPosition{};

public:
	void InputKey(KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN && Event.NormalKey == NK_SPACE) { 
			Frame = randomUtil.Gen(RANDOM_TYPE_INT, HitHigh1, HitLow2); 
			if ((int)PrevGeneratedFrame == (int)Frame) 
				Frame++;
			PrevGeneratedFrame = Frame;
			StateTimer.Reset();

			soundUtil.Stop(SndChannel[StopChannel++]);
			soundUtil.Play(Snd.Whoosh, SndChannel[PlayChannel++]);
			EX.ClampValue(StopChannel, 0, 4, CLAMP_RETURN);
			EX.ClampValue(PlayChannel, 0, 4, CLAMP_RETURN);

			DestPosition += 0.5;

			BreakCount++;

			if (BreakCount >= MaxBreak) {
				DestPosition = NextPosition;
				BreakCount = 0;
				MaxBreak += 4;
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		// 이전 프레임과 현재 프레임이 다를 경우 이전 프레임을 갱신하고 애니메이션 출력하도록 한다
		// 단, 현재 프레임이 Idle일 경우 AnimationSize를 변경하지 않는다
		if ((int)PrevFrame != (int)Frame) {
			if ((int)Frame != Idle) 
				AnimationSize = -0.4;
			PrevFrame = Frame;
		}
		
		// 현재 상태가 Idle이 아니라면 0.3초 후 다시 Idle 상태로 복귀시킨다
		if ((int)Frame != Idle) {
			StateTimer.Update(FrameTime);
			if (StateTimer.CheckMiliSec(0.3, 2, CHECK_AND_RESET)) 
				Frame = Idle; 
		}

		// AnimationSize가 0.0보다 작다면 다시 0.0으로 복귀시킨다
		mathUtil.Lerp(AnimationSize, 0.0, 15.0, FrameTime);

		// 목표 위치로 이동하도록 한다
		mathUtil.Lerp(Position, DestPosition, 20.0, FrameTime);

		// 카메라가 이드를 부드럽게 따라오도록 한다
		mathUtil.Lerp(CameraPosition, -DestPosition, 7.0, FrameTime);

		// 이드를 약간 오른쪽에서 바라보도록 한다
		CameraControl->MoveCamera(CameraPosition - 0.5, 0.0);
	}

	void RenderFunc() {
		Begin();
		transform.Move(MoveMatrix, Position, AnimationSize * 0.5);
		transform.Scale(MoveMatrix, 2.0, 2.0 + AnimationSize);
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