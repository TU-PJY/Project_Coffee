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

		// 중력 지정
		ps.SetGravity(15.0);

		// 랜덤 바닥 높이 설정
		GLfloat RandomFloorHeight = randomUtil.Gen(RANDOM_TYPE_REAL, -0.85, -0.95);
		ps.SetFloorHeight(RandomFloorHeight);

		// 랜덤 중력 가속도 설정
		GLfloat RandomAcc = randomUtil.Gen(RANDOM_TYPE_REAL, 5.0, 8.0);
		ps.AddGravityAcc(RandomAcc);

		// 최소 바운스 수치와 바운드 감소 수치 설정
		ps.SetMinimumRebounce(2.0);
		ps.SetRebounceReduce(3.0);

		// 랜덤 속도 지정
		MoveSpeed = randomUtil.Gen(RANDOM_TYPE_REAL, 1.0, 3.0);

		// 랜덤 회전 속도 지정
		RotateSpeed = randomUtil.Gen(RANDOM_TYPE_REAL, 800.0, 1500.0);
	}

	void UpdateFunc(float FrameTime) {
		// 떨어지면서 바닥에 튕기는 물리 엔진 시뮬레이션 실행
		if (ps.GetFallingState()) {
			Position.x += MoveSpeed * FrameTime;
			Rotation += RotateSpeed * FrameTime;

			// 바닥에 튕길때마다 소리를 재생한다
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

		// 화면을 벗어날 경우 삭제
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