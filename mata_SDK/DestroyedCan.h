#pragma once
#include <Scene.h>

class DestroyedCan : public GameObject {
private:
	PhysicsUtil ps{};
	glm::vec2 Position{};
	GLfloat MoveSpeed{};

	GLfloat Rotation{};
	GLfloat RotateSpeed{};

	SoundChannel SndChannel{};

	TimerUtil Timer{};

	bool SoundPlayed{};

public:
	DestroyedCan(glm::vec2 PositionValue) {
		Position = PositionValue;

		// �߷� ����
		ps.SetGravity(15.0);

		// ���� �ٴ� ���� ����
		GLfloat RandomFloorHeight = randomUtil.Gen(RANDOM_TYPE_REAL, -0.85, -0.95);
		ps.SetFloorHeight(RandomFloorHeight);

		// ���� �߷� ���ӵ� ����
		GLfloat RandomAcc = randomUtil.Gen(RANDOM_TYPE_REAL, 5.0, 8.0);
		ps.AddGravityAcc(RandomAcc);

		// �ּ� �ٿ ��ġ�� �ٿ�� ���� ��ġ ����
		ps.SetMinimumRebounce(2.0);
		ps.SetRebounceReduce(3.0);

		// ���� �ӵ� ����
		MoveSpeed = randomUtil.Gen(RANDOM_TYPE_REAL, 1.0, 3.0);

		// ���� ȸ�� �ӵ� ����
		RotateSpeed = randomUtil.Gen(RANDOM_TYPE_REAL, 800.0, 1500.0);
	}

	void UpdateFunc(float FrameTime) {
		// �������鼭 �ٴڿ� ƨ��� ���� ���� �ùķ��̼� ����
		if (ps.GetFallingState()) {
			Position.x += MoveSpeed * FrameTime;
			Rotation += RotateSpeed * FrameTime;

			// �ٴڿ� ƨ�涧���� �Ҹ��� ����Ѵ�
			if (ps.CheckFloorCollision(Position)) {
				RotateSpeed *= 0.5;
				MoveSpeed *= 0.7;
				soundUtil.Stop(SndChannel);
				soundUtil.Play(Snd.CanDrop, SndChannel);
				SndChannel->setVolume(Glb.SFXVolume);
			}
		}
		else {
			soundUtil.PlayOnce(Snd.CanDrop, SndChannel, SoundPlayed);
			SndChannel->setVolume(Glb.SFXVolume);
		}

		ps.UpdateBouncing(Position, FrameTime);

		// ȭ���� ��� ��� ����
		if (Position.x < CameraPosition.x - ASP(1.0) - 0.255)
			scene.DeleteObject(this);
	}

	void RenderFunc() {
		Begin();
		transform.Move(MoveMatrix, Position);
		transform.Rotate(MoveMatrix, Rotation);
		transform.Scale(MoveMatrix, 0.45, 0.45);
		imageUtil.Render(Img.DestroyedCan);
	}
};