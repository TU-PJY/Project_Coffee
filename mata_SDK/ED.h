#pragma once
#include <Scene.h>

class ED : public GameObject {
private:
	// 각 상태마다 다른 프레임을 출력한다
	GLfloat Frame = HitHigh1;

	// 현재 프레임과 이전 프레임이 다르면 살짝 눌리는 애니메이션을 재생한다
	GLfloat PrevFrame = Frame;
	GLfloat AnimationSize{};

	// 상태가 변경되면 일정 시간 이후 다시 Idle 상태로 북귀하도록 한다
	TimerUtil StateTimer{};

public:
	void InputKey(KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN && Event.NormalKey == NK_SPACE) { 
			Frame = randomUtil.Gen(RANDOM_TYPE_INT, HitHigh1, HitPeople); 
			StateTimer.Reset();
		}
	}

	void UpdateFunc(float FrameTime) {
		// 이전 프레임과 현재 프레임이 다를 경우 이전 프레임을 갱신하고 애니메이션 출력하도록 한다
		// 단, 현재 프레임이 Idle일 경우 AnimationSize를 변경하지 않는다
		if ((int)PrevFrame != (int)Frame) {
			if (Frame != Idle) { AnimationSize = -0.4; }
			PrevFrame = Frame;
		}
		
		// 현재 상태가 Idle이 아니라면 0.3초 후 다시 Idle 상태로 복귀시킨다
		if (Frame != Idle) {
			StateTimer.Update(FrameTime);
			if (StateTimer.CheckMiliSec(0.3, 2, CHECK_AND_RESET))
				Frame = Idle;
		}

		// AnimationSize가 0.0보다 크다면 다시 0.0으로 복귀시킨다
		mathUtil.Lerp(AnimationSize, 0.0, 15.0, FrameTime);
	}

	void RenderFunc() {
		Begin();
		transform.Move(MoveMatrix, -0.5, 0.0 + AnimationSize * 0.5);
		transform.Scale(MoveMatrix, 2.0, 2.0 + AnimationSize);
		imageUtil.RenderSpriteSheet(Img.ED, Frame);
	}
};