#pragma once
#include <Scene.h>
#include "Cart.h"

enum PeopleTypeEnum {
	Listy,
	Mayo,
	Naia,
	EOE
};

class People : public GameObject {
private:
	// �ε����� ���� �������Ǵ� ĳ���Ͱ� �޶�����
	int Frame{};

	// ��ġ
	glm::vec2 Position{};

	// ���ư� �Ÿ�
	GLfloat MoveDistance{};

	// ȸ����
	GLfloat Rotation{};

	// īƮ ��ġ
	glm::vec2 CartPosition{};

	// īƮ ȸ����
	GLfloat CartRotation{};

	// �̵� �߿� ���������� ����
	bool HitState{};

	// �ٴڿ� ���������� ����
	bool FellDown{};

	// ���� �ִϸ��̼� ��ü
	SinLoop Loop{};
	SinLoop Loop2{};

	// ������ �ִϸ��̼� ��ġ
	GLfloat LoopSize{};

	// �Ѿ��� ���� �ִϸ��̼� ��ġ
	GLfloat FellDownSize{};

	// ���� ä��
	SoundChannel SndChannel{};

public:
	People(glm::vec2 PositionValue) {
		Position = PositionValue;
		CartPosition.x = PositionValue.x + 1.2;
		CartPosition.y = PositionValue.y - 0.3;

		// ĳ���� ���� ����
		Frame = randomUtil.Gen(RANDOM_TYPE_INT, Listy, EOE - 1) * 2;
	}

	void UpdateFunc(float FrameTime) {
		// �߿� ���� ���¿����� ������ ������.
		if (HitState) {
			// �Ѿ����� ��
			if (!FellDown) {
				Rotation += 250 * FrameTime;
				Position.x += 8.0 * FrameTime;
				Position.y -= 1.0 * FrameTime;

				CartPosition.x += 8.0 * FrameTime;
				CartPosition.y -= 0.5 * FrameTime;

				// -1.3���� �̵��ϸ� �Ѿ��� ���¸� Ȱ��ȭ �Ѵ�
				if (Position.y <= -1.3) {
					soundUtil.Play(Snd.CartCrash, SndChannel);
					Position.y = -1.3;
					CartPosition.x += 0.5;
					CartPosition.y = -0.35;
					scene.AddObject(new Cart(true, CartPosition), "cart", LAYER4);

					// ���� �������� ���� ������ �Ѿ��� ������
					Frame++;

					FellDown = true;

					// 5�� ���̾�� �̵�
					scene.SwapLayer(this, LAYER5);
				}
			}
			// �Ѿ��� ��
			else {
				Rotation = -25.0 - Loop.Update(2.5, 20.0, FrameTime);
				FellDownSize = Loop2.Update(0.1, 20.0, FrameTime);
			}
		}

		// ������ �ִϸ��̼�
		else {
			if(Frame == Naia * 2)
				LoopSize = Loop.Update(0.03, 30.0, FrameTime);
			else
				LoopSize = Loop.Update(0.03, 4.0, FrameTime);
		}

		// ȭ�� �������� ����� �����Ѵ�.
		if (Position.x < CameraPosition.x - ASP(1.0) - 1.2)
			scene.DeleteObject(this);
	}

	void RenderFunc() {
		// ȭ�麸�� �����ʿ� ���� �� �������� �������� �ʴ´�.
		if (Position.x > CameraPosition.x + ASP(1.0) + 1.2)
			return;

		// ��� ������
		Begin();
		transform.Move(MoveMatrix, Position.x, Position.y + LoopSize * 0.5);
		transform.Rotate(MoveMatrix, Rotation);
		transform.Scale(MoveMatrix, 2.0, 2.0 + LoopSize + FellDownSize);
		imageUtil.RenderStaticSpriteSheet(Img.People, Frame);

		// īƮ ������
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
		// ���� ���°� �Ǹ� �������� �����ϰ� ������Ʈ �±׸� ���� �� �̻� ��ȣ�ۿ����� �ʵ��� �Ѵ�
		HitState = true;
		ObjectTag = "";
		Loop.SetValue(Preset::HalfPositive);
		Loop2.SetValue(Preset::HalfNegative);
		LoopSize = 0.0;
	}
};