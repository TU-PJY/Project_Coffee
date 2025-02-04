#pragma once
#include <Scene.h>
#include <CameraController.h>

class ED : public GameObject {
private:
	// ���� ��ġ, Ŀ�Ǹ� �μ� �� ���� ���������� �̵��Ѵ�.
	GLfloat Position{};

	// Ŀ�Ǹ� �μ� �� ��ǥ �̵� ��ġ
	GLfloat DestPosition{};

	// ī�޶� ��ġ
	GLfloat CameraPosition{};

	// �� ���¸��� �ٸ� �������� ����Ѵ�
	GLfloat Frame = Idle;

	GLfloat PrevGeneratedFrame{};

	// ���� �����Ӱ� ���� �������� �ٸ��� ��¦ ������ �ִϸ��̼��� ����Ѵ�
	GLfloat PrevFrame = Frame;
	GLfloat AnimationSize{};

	// ���� �������� �̵� �� �������� �ִϸ��̼��� ����Ѵ�
	GLfloat TiltValue{};

	// ���°� ����Ǹ� ���� �ð� ���� �ٽ� Idle ���·� �ϱ��ϵ��� �Ѵ�
	TimerUtil StateTimer{};

	// �̵� ���� ä��
	// �������� ���� ����� ���� ��ȯ ä���� ����Ѵ�
	SoundChannel SndChannel[5]{};
	int PlayChannel = 0;
	int StopChannel = 1;

	SoundChannel SndChannel2{};
	        
	// �̵尡 Ŀ�Ǹ� �μ� Ƚ��
	int BreakCount{};

	// Ŀ�Ǹ� �ν����ϴ� ����, �ʱⰪ 8��
	int MaxBreak{8};

	// ���� ������ ���� ����
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

			// Ŀ�Ǹ� ��� �μ��� ���� �������� �̵��ϰ� �ν����ϴ� ������ 4 �����Ѵ�.
			// ������ �������� �ִϸ��̼��� ����Ѵ�.
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
		// ���� �����Ӱ� ���� �������� �ٸ� ��� ���� �������� �����ϰ� �ִϸ��̼� ����ϵ��� �Ѵ�
		// ��, ���� �������� Idle�� ��� AnimationSize�� �������� �ʴ´�
		if ((int)PrevFrame != (int)Frame) {
			if ((int)Frame != Idle) 
				AnimationSize = -0.7;
			PrevFrame = Frame;
		}
		
		// ���� ���°� Idle�� �ƴ϶�� 0.3�� �� �ٽ� Idle ���·� ���ͽ�Ų��
		if ((int)Frame != Idle) {
			StateTimer.Update(FrameTime);
			if (StateTimer.CheckMiliSec(0.3, 2, CHECK_AND_RESET)) 
				Frame = Idle; 
		}


		// AnimationSize�� 0.0���� �۴ٸ� �ٽ� 0.0���� ���ͽ�Ų��
		mathUtil.Lerp(AnimationSize, 0.0, 15.0, FrameTime);

		// TiltValue�� 0.0���� ũ�ٸ� �ٽ� 0.0���� ���ͽ�Ų��
		mathUtil.Lerp(TiltValue, 0.0, 5.0, FrameTime);


		// ��ǥ ��ġ�� �̵��ϵ��� �Ѵ�
		mathUtil.Lerp(Position, DestPosition, 20.0, FrameTime);


		// ī�޶� �̵带 �ε巴�� ��������� �Ѵ�
		mathUtil.Lerp(CameraPosition, -DestPosition, 7.0, FrameTime);

		// �̵带 �ణ �����ʿ��� �ٶ󺸵��� �Ѵ�
		CameraControl->MoveCamera(CameraPosition - 0.5, 0.0);
	}

	void RenderFunc() {
		Begin();
		transform.Move(MoveMatrix, Position + TiltValue * 0.5, AnimationSize * 0.5);
		transform.Scale(MoveMatrix, 2.0, 2.0 + AnimationSize);
		transform.Shear(MoveMatrix, TiltValue, 0.0);
		imageUtil.RenderSpriteSheet(Img.ED, Frame);
	}

	GLfloat GetPosition() {
		return Position;
	}

	// ���� ������ ���� ������ �̸� �˸���
	void TellNextPosition(GLfloat Position) {
		NextPosition = Position;
	}
};