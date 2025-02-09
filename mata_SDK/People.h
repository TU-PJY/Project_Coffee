#pragma once
#include <Scene.h>
#include "Cart.h"

enum PeopleTypeEnum {
	Listy,
	Mayo,
	Naia,
	Hildae,
	Silphir,
	Balong,
	Kidian,
	Melloon,
	Yumimi,
	Daepyo,
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

		// �ִ� 2ȸ�� �������� ������ ĳ���ͷ� �������� �ʴ´�.
		if ((Frame == Glb.PrevCharacterFrame1 && Frame == Glb.PrevCharacterFrame2) ||
			(Frame == Glb.PrevCharacterFrame1 && Frame != Glb.PrevCharacterFrame2) ||
			(Frame != Glb.PrevCharacterFrame1 && Frame == Glb.PrevCharacterFrame2)) {
			if (Glb.PrevCharacterFrame1 == Listy)
				Frame += 2;
			else if (Glb.PrevCharacterFrame1 == Daepyo)
				Frame -= 2;
			else {
				int RandNum = randomUtil.Gen(RANDOM_TYPE_INT, 0, 1);
				if (RandNum == 1)
					Frame += 2;
				else
					Frame -= 2;
			}
		}

		//  ���� ������ ����
		Glb.PrevCharacterFrame2 = Glb.PrevCharacterFrame1;
		Glb.PrevCharacterFrame1 = Frame;
	}

	void UpdateFunc(float FrameTime) {
		// �߿� ���� ���¿����� ������ ������.
		if (HitState) {
			// �Ѿ����� ��
			if (!FellDown) {
				Rotation += 250 * FrameTime;
				Position.x += 8.0 * FrameTime;
				Position.y -= 1.5 * FrameTime;

				CartPosition.x += 8.0 * FrameTime;
				CartPosition.y -= 0.75 * FrameTime;

				// -1.3���� �̵��ϸ� �Ѿ��� ���¸� Ȱ��ȭ �Ѵ�
				if (Position.y <= -1.3) {
					// ���̴̹� �������� ����Ѵ�
					if (Frame == Yumimi * 2) {
						soundUtil.Play(Snd.Explode, SndChannel);

						// ���߷� ���� ��鸲 �߰�
						cameraControl.AddShakeValue(0.8);
					}
					else
						soundUtil.Play(Snd.CartCrash, SndChannel);

					if (Frame == Silphir * 2)
						Position.y = -1.2;
					else if (Frame == Kidian * 2)
						Position.y = -0.8;
					else if (Frame == Melloon * 2)
						Position.y = -1.53;
					else if (Frame == Yumimi * 2)
						Position.y = -1.4;
					else
						Position.y = -1.3;

					CartPosition.x += 0.5;
					CartPosition.y = -0.35;
					                                    
					// 5�� ���̾�� �̵�

					// �Ѿ��� īƮ �߰�
					// ���̴̹� īƮ�� �߰����� �ʴ´�
					if (Frame != Yumimi * 2) {
						scene.SwapLayer(this, LAYER5);
						scene.AddObject(new Cart(true, CartPosition), "cart", LAYER3);
					}
					else 
						scene.SwapLayer(this, LAYER1);

					// ���� �������� ���� ������ �Ѿ��� ������
					Frame++;

					// �Ѿ��� ���� Ȱ��ȭ
					FellDown = true;
				}
			}
			// �Ѿ��� ��
			else {
				// Ű����� ��� �౸���� �Ǿ� ��������
				if (Frame == Kidian * 2 + 1) {
					Rotation -= 360 * FrameTime;
					Position.x -= 4.0 * FrameTime;
				}

				// ���̹�, ����� ��� ������ �ִ´�
				else if (Frame == Melloon * 2 + 1) 
					Rotation = 180.0;
				else if (Frame == Yumimi * 2 + 1) 
					Rotation = 0.0;

				else {
					Rotation = -25.0 - Loop.Update(2.5, 20.0, FrameTime);
					FellDownSize = Loop2.Update(0.1, 20.0, FrameTime);
				}
			}
		}

		// ������ �ִϸ��̼�
		else {
			if (Frame == Naia * 2)
				LoopSize = Loop.Update(0.03, 30.0, FrameTime);

			// ����� ������ �ִ´�
			else if (Frame == Melloon * 2) {
				LoopSize = Loop.Update(0.0, 0.0, 0.0);
			}
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
		if(Frame == Yumimi * 2 + 1)
			transform.Scale(MoveMatrix, 3.0, 3.0 + LoopSize + FellDownSize);
		else
			transform.Scale(MoveMatrix, 2.0, 2.0 + LoopSize + FellDownSize);
		imageUtil.RenderStaticSpriteSheet(Img.People, Frame);

		// īƮ ������
		// ���̴̹� īƮ�� ���������� �ʴ´�
		if (!FellDown && Frame != Yumimi * 2) {
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