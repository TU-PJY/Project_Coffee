#pragma once
#include <Scene.h>
#include "Cart.h"

class People : public GameObject {
private:
	// 인덱스에 따라 렌더링되는 캐릭터가 달라진다
	int Frame{};

	// 위치
	glm::vec2 Position{};

	// 날아간 거리
	GLfloat MoveDistance{};

	// 회전값
	GLfloat Rotation{};

	// 카트 위치
	glm::vec2 CartPosition{};

	// 카트 회전값
	GLfloat CartRotation{};

	// 이드 발에 차였는지의 여부
	bool HitState{};

	// 바닥에 엎어졌는지 여부
	bool FellDown{};

	// 루프 애니메이션 객체
	SinLoop Loop{};
	SinLoop Loop2{};

	// 숨쉬기 애니메이션 수치
	GLfloat LoopSize{};

	// 넘어진 후의 애니메이션 수치
	GLfloat FellDownSize{};

	// 사운드 채널
	SoundChannel SndChannel{};

public:
	People(glm::vec2 PositionValue) {
		Position = PositionValue;
		CartPosition.x = PositionValue.x + 1.2;
		CartPosition.y = PositionValue.y - 0.3;
		Frame = 0;
	}

	void UpdateFunc(float FrameTime) {
		// 발에 차인 상태에서는 앞으로 구른다.
		if (HitState) {
			// 넘어지기 전
			if (!FellDown) {
				Rotation += 250 * FrameTime;
				Position.x += 8.0 * FrameTime;
				Position.y -= 1.0 * FrameTime;

				CartPosition.x += 8.0 * FrameTime;
				CartPosition.y -= 0.5 * FrameTime;

				// -1.3까지 이동하면 넘어짐 상태를 활성화 한다
				if (Position.y <= -1.3) {
					soundUtil.Play(Snd.CartCrash, SndChannel);
					Position.y = -1.3;
					CartPosition.x += 0.5;
					CartPosition.y = -0.35;
					scene.AddObject(new Cart(true, CartPosition), "cart", LAYER4);

					// 현재 프레임의 다음 프레임 넘어진 프레임
					Frame++;

					FellDown = true;

					// 5번 레이어로 이동
					scene.SwapLayer(this, LAYER5);
				}
			}
			// 넘어진 후
			else {
				Rotation = -25.0 - Loop.Update(2.5, 20.0, FrameTime);
				FellDownSize = Loop2.Update(0.1, 20.0, FrameTime);
			}
		}

		// 숨쉬기 애니메이션
		else
			LoopSize = Loop.Update(0.03, 4.0, FrameTime);

		// 화면 왼쪽으로 벗어나면 삭제한다.
		if (CartPosition.x < CameraPosition.x - ASP(1.0) - 0.75)
			scene.DeleteObject(this);
	}

	void RenderFunc() {
		// 화면보다 오른쪽에 있을 때 랜더링을 진행하지 않는다.
		if (Position.x > CameraPosition.x + ASP(1.0) + 1.0)
			return;

		// 사람 렌더링
		Begin();
		transform.Move(MoveMatrix, Position.x, Position.y + LoopSize * 0.5);
		transform.Rotate(MoveMatrix, Rotation);
		transform.Scale(MoveMatrix, 2.0, 2.0 + LoopSize + FellDownSize);
		imageUtil.RenderStaticSpriteSheet(Img.People, Frame);

		// 카트 렌더링
		if (!FellDown) {
			Begin();
			transform.Move(MoveMatrix, CartPosition.x, CartPosition.y);
			transform.Rotate(MoveMatrix, CartRotation);
			transform.Scale(MoveMatrix, 1.5, 1.5);
			imageUtil.RenderStaticSpriteSheet(Img.Cart, 0);
		}
	}
	
	void HitPeople() {
		soundUtil.Play(Snd.PeopleHit, SndChannel);
		// 때린 상태가 되면 프레임을 변경하고 오브젝트 태그를 지워 더 이상 상호작용하지 않도록 한다
		HitState = true;
		ObjectTag = "";
		Loop.SetValue(Preset::HalfPositive);
		Loop2.SetValue(Preset::HalfNegative);
		LoopSize = 0.0;
	}
};