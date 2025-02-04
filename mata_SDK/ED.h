#pragma once
#include <Scene.h>

class ED : public GameObject {
private:
	// �� ���¸��� �ٸ� �������� ����Ѵ�
	GLfloat Frame = HitHigh1;

	// ���� �����Ӱ� ���� �������� �ٸ��� ��¦ ������ �ִϸ��̼��� ����Ѵ�
	GLfloat PrevFrame = Frame;
	GLfloat AnimationSize{};

	// ���°� ����Ǹ� ���� �ð� ���� �ٽ� Idle ���·� �ϱ��ϵ��� �Ѵ�
	TimerUtil StateTimer{};

public:
	void InputKey(KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN && Event.NormalKey == NK_SPACE) { 
			Frame = randomUtil.Gen(RANDOM_TYPE_INT, HitHigh1, HitPeople); 
			StateTimer.Reset();
		}
	}

	void UpdateFunc(float FrameTime) {
		// ���� �����Ӱ� ���� �������� �ٸ� ��� ���� �������� �����ϰ� �ִϸ��̼� ����ϵ��� �Ѵ�
		// ��, ���� �������� Idle�� ��� AnimationSize�� �������� �ʴ´�
		if ((int)PrevFrame != (int)Frame) {
			if (Frame != Idle) { AnimationSize = -0.4; }
			PrevFrame = Frame;
		}
		
		// ���� ���°� Idle�� �ƴ϶�� 0.3�� �� �ٽ� Idle ���·� ���ͽ�Ų��
		if (Frame != Idle) {
			StateTimer.Update(FrameTime);
			if (StateTimer.CheckMiliSec(0.3, 2, CHECK_AND_RESET))
				Frame = Idle;
		}

		// AnimationSize�� 0.0���� ũ�ٸ� �ٽ� 0.0���� ���ͽ�Ų��
		mathUtil.Lerp(AnimationSize, 0.0, 15.0, FrameTime);
	}

	void RenderFunc() {
		Begin();
		transform.Move(MoveMatrix, -0.5, 0.0 + AnimationSize * 0.5);
		transform.Scale(MoveMatrix, 2.0, 2.0 + AnimationSize);
		imageUtil.RenderSpriteSheet(Img.ED, Frame);
	}
};