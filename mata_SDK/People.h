#pragma once
#include <Scene.h>
#include "Cart.h"
#include "Smoke.h"

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

int Num;

class People : public GameObject {
private:
	// �ε����� ���� �������Ǵ� ĳ���Ͱ� �޶�����
	int ChloeFrame{};

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

	// ���� �ϴ� Ƚ��
	int HitCount = 1;

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

	// ���� ���� ����� ��ġ
	GLfloat TiltValue{};

	// ���� ä��
	SoundChannel SndChannel{};

public:
	People(glm::vec2 PositionValue) {
		Position = PositionValue;
		CartPosition.x = PositionValue.x + 1.2;
		CartPosition.y = PositionValue.y - 0.3;

		int RandNum = randomUtil.Gen(RANDOM_TYPE_INT, Listy, EOE - 1);

		// �� �� ���õ� ĳ���ʹ� ���� 2ȸ�� ���ȿ��� ���õ��� �ʴ´�.
		if (Glb.CreateAvailable[RandNum]) {
			Glb.CreateAvailable[RandNum] = false;
			ChloeFrame = RandNum * 2;
		}

		else {
			std::vector<int> AvailableNum{};

			// ���� �� �ִ� ĳ���͵��� ������ ��ȣ�� ���Ϳ� ����
			for (int i = Listy; i < EOE; i++) {
				if (Glb.CreateAvailable[i])
					AvailableNum.emplace_back(i);
			}

			// ������ ��ȣ �� �ϳ��� �����Ͽ� ĳ���� �̱�
			int Rand = randomUtil.Gen(RANDOM_TYPE_INT, 0, AvailableNum.size() - 1);
			ChloeFrame = AvailableNum[Rand] * 2;
		}

		// ���� ���� �� ���� ĳ���͵��� ī��Ʈ ���� 
		for (int i = Listy; i < EOE; i++) {
			if (!Glb.CreateAvailable[i])
				Glb.PrevChFrame[i]++;

			// ī��Ʈ�� 2�� �Ǹ� �ٽ� ���� �� �ִ� ���·� ��ȯ
			if (Glb.PrevChFrame[i] > 2) {
				Glb.CreateAvailable[i] = true;
				Glb.PrevChFrame[i] = 0;
			}
		}

		// ��ǥ�� ��� 2�� ���� �Ѵ�
		if (ChloeFrame == Daepyo * 2)
			HitCount = 2;
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
				CartPosition.y -= 0.75 * FrameTime;

				// -1.3���� �̵��ϸ� �Ѿ��� ���¸� Ȱ��ȭ �Ѵ�
				if (Position.y <= -1.3) {
					// ���̴̹� �������� ����Ѵ�
					if (ChloeFrame == Yumimi * 2) {
						soundUtil.Play(Snd.Explode, SndChannel);

						// ���߷� ���� ��鸲 �߰�
						cameraControl.AddShakeValue(0.8);
					}
					else
						soundUtil.Play(Snd.CartCrash, SndChannel);

					if (ChloeFrame == Silphir * 2)
						Position.y = -1.2;
					else if (ChloeFrame == Kidian * 2)
						Position.y = -0.8;
					else if (ChloeFrame == Melloon * 2)
						Position.y = -1.53;
					else if (ChloeFrame == Yumimi * 2)
						Position.y = -1.4;
					else if (ChloeFrame == Daepyo * 2)
						Position.y = -1.2;
					else
						Position.y = -1.3;

					CartPosition.x += 0.5;
					CartPosition.y = -0.35;
					                                    
					// 5�� ���̾�� �̵�

					// �Ѿ��� īƮ �߰�
					// ���̴̹� īƮ�� �߰����� �ʴ´�
					if (ChloeFrame != Yumimi * 2) {
						scene.SwapLayer(this, LAYER6);
						scene.AddObject(new Cart(true, CartPosition), "cart", LAYER4);
					}
					else {
						scene.SwapLayer(this, LAYER2);
						scene.AddObject(new Smoke(Position.x), "smoke", LAYER5);
					}

					// ���� �������� ���� ������ �Ѿ��� ������
					ChloeFrame++;

					// �Ѿ��� ���� Ȱ��ȭ
					FellDown = true;
				}
			}
			// �Ѿ��� ��
			else {
				// Ű����� ��� �౸���� �Ǿ� ��������
				if (ChloeFrame == Kidian * 2 + 1) {
					Rotation -= 360 * FrameTime;
					Position.x -= 4.0 * FrameTime;
				}

				// ���̹�, ����� ��� ������ �ִ´�
				else if (ChloeFrame == Melloon * 2 + 1) 
					Rotation = 180.0;
				else if (ChloeFrame == Yumimi * 2 + 1) 
					Rotation = 0.0;

				else {
					Rotation = -25.0 - Loop.Update(2.5, 20.0, FrameTime);
					FellDownSize = Loop2.Update(0.1, 20.0, FrameTime);
				}
			}
		}

		// ������ �ִϸ��̼�
		else {
			if (ChloeFrame == Naia * 2)
				LoopSize = Loop.Update(0.03, 30.0, FrameTime);

			// ����� ������ �ִ´�
			else if (ChloeFrame == Melloon * 2) {
				LoopSize = Loop.Update(0.0, 0.0, 0.0);
			}
			else
				LoopSize = Loop.Update(0.03, 4.0, FrameTime);
		}

		// ����� ��ġ ������Ʈ
		mathUtil.Lerp(TiltValue, 0.0, 10.0, FrameTime);

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
		transform.Move(MoveMatrix, Position.x + TiltValue * 0.5, Position.y + LoopSize * 0.5);
		transform.Rotate(MoveMatrix, Rotation);
		if(ChloeFrame == Yumimi * 2 + 1)
			transform.Scale(MoveMatrix, 3.0, 3.0 + LoopSize + FellDownSize);
		else
			transform.Scale(MoveMatrix, 2.0, 2.0 + LoopSize + FellDownSize);

		transform.Shear(MoveMatrix, TiltValue, 0.0);
		imageUtil.RenderStaticSpriteSheet(Img.People, ChloeFrame);

		// īƮ ������
		// ���̴̹� īƮ�� ���������� �ʴ´�
		if (!FellDown && ChloeFrame != Yumimi * 2) {
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
		HitCount--;

		// ����� HitCount�� 0�� �� ��� Ŀ�Ǹ� �ٽ� �μ� �� �ִ� ���·� ��ȯ�Ѵ� 
		if (HitCount == 0) {
			if(auto Shelf = scene.Find("shelf"); Shelf)
				Shelf->EnableCoffeeHit();
			HitState = true;
			ObjectTag = "";
			Loop.SetValue(Preset::HalfPositive);                     
			Loop2.SetValue(Preset::HalfNegative);
			LoopSize = 0.0;

			GameObject* Score = scene.Find("score_indicator");

			if (ChloeFrame == Daepyo * 2)
				Score->AddScore(118);
			else
				Score->AddScore(100);

			Glb.PeopleHitCount++;
		}

		// HitCount�� �������� ��� ����� ��ġ�� �߰��Ѵ�
		else 
			TiltValue = 2.0;
	}
};