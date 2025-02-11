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
	// 인덱스에 따라 렌더링되는 캐릭터가 달라진다
	int ChloeFrame{};

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

	// 차야 하는 횟수
	int HitCount = 1;

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

	// 차인 후의 기울임 수치
	GLfloat TiltValue{};

	// 사운드 채널
	SoundChannel SndChannel{};

public:
	People(glm::vec2 PositionValue) {
		Position = PositionValue;
		CartPosition.x = PositionValue.x + 1.2;
		CartPosition.y = PositionValue.y - 0.3;

		int RandNum = randomUtil.Gen(RANDOM_TYPE_INT, Listy, EOE - 1);

		// 한 번 선택된 캐릭터는 다음 2회차 동안에는 선택되지 않는다.
		if (Glb.CreateAvailable[RandNum]) {
			Glb.CreateAvailable[RandNum] = false;
			ChloeFrame = RandNum * 2;
		}

		else {
			std::vector<int> AvailableNum{};

			// 뽑을 수 있는 캐릭터들의 프레임 번호를 벡터에 저장
			for (int i = Listy; i < EOE; i++) {
				if (Glb.CreateAvailable[i])
					AvailableNum.emplace_back(i);
			}

			// 벡터의 번호 중 하나를 선택하여 캐릭터 뽑기
			int Rand = randomUtil.Gen(RANDOM_TYPE_INT, 0, AvailableNum.size() - 1);
			ChloeFrame = AvailableNum[Rand] * 2;
		}

		// 현재 뽑을 수 없는 캐릭터들은 카운트 증가 
		for (int i = Listy; i < EOE; i++) {
			if (!Glb.CreateAvailable[i])
				Glb.PrevChFrame[i]++;

			// 카운트가 2가 되면 다시 뽑을 수 있는 상태로 전환
			if (Glb.PrevChFrame[i] > 2) {
				Glb.CreateAvailable[i] = true;
				Glb.PrevChFrame[i] = 0;
			}
		}

		// 대표의 경우 2번 차야 한다
		if (ChloeFrame == Daepyo * 2)
			HitCount = 2;
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
				CartPosition.y -= 0.75 * FrameTime;

				// -1.3까지 이동하면 넘어짐 상태를 활성화 한다
				if (Position.y <= -1.3) {
					// 유미미는 폭발음을 재생한다
					if (ChloeFrame == Yumimi * 2) {
						soundUtil.Play(Snd.Explode, SndChannel);

						// 폭발로 인한 흔들림 추가
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
					                                    
					// 5번 레이어로 이동

					// 넘어진 카트 추가
					// 유미미는 카트를 추가하지 않는다
					if (ChloeFrame != Yumimi * 2) {
						scene.SwapLayer(this, LAYER6);
						scene.AddObject(new Cart(true, CartPosition), "cart", LAYER4);
					}
					else {
						scene.SwapLayer(this, LAYER2);
						scene.AddObject(new Smoke(Position.x), "smoke", LAYER5);
					}

					// 현재 프레임의 다음 프레임 넘어진 프레임
					ChloeFrame++;

					// 넘어짐 상태 활성화
					FellDown = true;
				}
			}
			// 넘어진 후
			else {
				// 키디언의 경우 축구공이 되어 굴러간다
				if (ChloeFrame == Kidian * 2 + 1) {
					Rotation -= 360 * FrameTime;
					Position.x -= 4.0 * FrameTime;
				}

				// 유미미, 멜룬의 경우 가만히 있는다
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

		// 숨쉬기 애니메이션
		else {
			if (ChloeFrame == Naia * 2)
				LoopSize = Loop.Update(0.03, 30.0, FrameTime);

			// 멜룬은 가만히 있는다
			else if (ChloeFrame == Melloon * 2) {
				LoopSize = Loop.Update(0.0, 0.0, 0.0);
			}
			else
				LoopSize = Loop.Update(0.03, 4.0, FrameTime);
		}

		// 기울임 수치 업데이트
		mathUtil.Lerp(TiltValue, 0.0, 10.0, FrameTime);

		// 화면 왼쪽으로 벗어나면 삭제한다.
		if (Position.x < CameraPosition.x - ASP(1.0) - 1.2)
			scene.DeleteObject(this);
	}

	void RenderFunc() {
		// 화면보다 오른쪽에 있을 때 랜더링을 진행하지 않는다.
		if (Position.x > CameraPosition.x + ASP(1.0) + 1.2)
			return;

		// 사람 렌더링
		Begin();
		transform.Move(MoveMatrix, Position.x + TiltValue * 0.5, Position.y + LoopSize * 0.5);
		transform.Rotate(MoveMatrix, Rotation);
		if(ChloeFrame == Yumimi * 2 + 1)
			transform.Scale(MoveMatrix, 3.0, 3.0 + LoopSize + FellDownSize);
		else
			transform.Scale(MoveMatrix, 2.0, 2.0 + LoopSize + FellDownSize);

		transform.Shear(MoveMatrix, TiltValue, 0.0);
		imageUtil.RenderStaticSpriteSheet(Img.People, ChloeFrame);

		// 카트 렌더링
		// 유미미는 카트를 렌더링하지 않는다
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
		// 때린 상태가 되면 프레임을 변경하고 오브젝트 태그를 지워 더 이상 상호작용하지 않도록 한다
		HitCount--;

		// 사람의 HitCount가 0이 될 경우 커피를 다시 부술 수 있는 상태로 전환한다 
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

		// HitCount가 남아있을 경우 기울임 수치를 추가한다
		else 
			TiltValue = 2.0;
	}
};