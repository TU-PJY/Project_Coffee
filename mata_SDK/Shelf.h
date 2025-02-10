#pragma once
#include <Scene.h>

#include "Explode.h"
#include "DestroyedCan.h"
#include "People.h"
#include "Xion.h"

class Shelf : public GameObject {
private:
	//////////////////////// ����
	// ���� ����
	int NumShelf{};

	// ���� ���� ��ġ
	GLfloat Position{};

	// ���� �ϳ��� ����
	GLfloat Length = 2.0;

	// ���� �߰����� ��ġ
	GLfloat MiddlePoint{};
	           
	// ���� ������ ��ġ
	GLfloat EndPoint{};

	// ���� �������� �����ϴ� �ε���
	int StartShelfIndex{};

	// ���� ���� ���� ����
	bool NextShelfGenerated{};

	//////////////////////// ����
	// Ŀ�ǵ��� ��ġ �� ������ �����ϴ� ����
	std::vector<ItemStruct> CoffeeVec{};

	// Ŀ�� �������� �����ϴ� �ε���
	int StartCoffeeIndex{};

	// ���� ���� ������ Ŀ�� �ε���
	int CurrentCoffeeIndex{};

	// Ŀ�ǰ� �ƴ� �ٸ� ���ǵ��� ��ġ �� ������ �����ϴ� ����
	std::vector<ItemStruct> OtherVec{}; 

	// Ŀ�ǰ� �ƴ� �ٸ� ���ǵ��� �������� �����ϴ� �ε���
	int StartOtherIndex{};

	// ���� ä��
	SoundChannel SndChannel[5]{};
	int PlayChannel = 0;
	int StopChannel = 2;

	// ����� �߰��� �������� ����
	bool PeopleAddActivated{};

	// ����� �߰��� �ڸ� �ε��� ��ȣ
	int AddedIndex{};

	// �ÿ��� ��ġ�� �������� ����
	bool XionAddActivated{};

	// �ÿ��� ������ ��ġ ���� ����
	bool AddedXionPosition{};

	// ��� ��ġ�� ���������� ����
	bool AvailableAddPeople{};

	// �ÿ��� ������ �ڸ�
	int XionIndex{};

	// �ÿ� ��ǥ �̵� ��ġ
	GLfloat XionDestPosition{};

	// �ÿ� �߰� ����
	bool XionGenerated{};

public:
	Shelf(int Num, GLfloat PositionValue) {
		NumShelf = Num;
		Position = PositionValue;

		// �� ���ݸ��� 50�ۼ�Ʈ�� Ȯ���� �ÿ� ��ġ�� Ȱ��ȭ�ȴ�
		if (NumShelf > 2 && randomUtil.Probability(50))
			XionAddActivated = true;

		// �߰� ���� �� �� ���� ���� ��� 
		MiddlePoint = Position + Length * (GLfloat)(Num - 1) * 0.5;
		EndPoint = Position + Length * (GLfloat)(Num - 1) + Length * 0.5;

		// ���� �� ĭ�� 4���� Ŀ�ǵ��� �������� ��ġ�Ѵ�.
		// ������ ĭ�� 3���� ��ġ�Ѵ�.
		int GenTime = Num * 4 - 1;

		for (int i = 0; i < GenTime; ++i) {
			AvailableAddPeople = true;

			ItemStruct Coffee{};
			ItemStruct Other{};

			// �ּ� 4ĭ �������� ��ġ�Ѵ�
			if (i - AddedIndex > 4 && NumShelf > 2) {
				// �� Ŀ�� ĭ���� 10�ۼ�Ʈ�� Ȯ���� �ÿ� ��ġ�� �����Ѵ�.
				// �ÿ� ��ġ�� ������ �ڸ����� ����� ��ġ�� �� ���� �� �� �����ϸ� �ٽ� �������� �ʴ´�.
				if (XionAddActivated && !AddedXionPosition && randomUtil.Probability(10)) {
					AvailableAddPeople = false;
					AddedXionPosition = true;

					// �ÿ��� ��ġ�� �ڸ� ���� ǥ��
					Coffee.IsXionFront = true;

					// �ÿ� ����
					GLfloat Position = PositionValue - 0.75 + 0.5 * i + 0.5;
					scene.AddObject(new Xion(Position, 0.0, false, Blocking), "xion", LAYER3);

					// �ε��� ���
					AddedIndex = i;

					// �ÿ��� ������ �ڸ� ���
					XionIndex = i;
				}

				//�� Ŀ�� ĭ ���� 5�ۼ�Ʈ�� Ȯ���� ����� ��ġ�Ѵ�
				if(AvailableAddPeople && randomUtil.Probability(10)) {
					glm::vec2 AddPosition = glm::vec2(PositionValue - 0.75 + 0.5 * i, 0.0);
					scene.AddObject(new People(AddPosition), "people", LAYER3);

					// ����� ��ġ�� �ڸ��� Ŀ�Ǵ� ���� ǥ���Ѵ�
					Coffee.IsPeopleFront = true;

					// ���������� ����� �߰��� �ε��� ���
					AddedIndex = i;
				}
			}

			// Ÿ�� ����
			Coffee.Type = randomUtil.Gen(RANDOM_TYPE_INT, 0, 2);
			Other.Type = randomUtil.Gen(RANDOM_TYPE_INT, 0, 2);

			// ��ġ ����
			// 1�̸� ��ĭ, 0�̸� �Ʒ�ĭ
			// Ŀ�ǿ� �ٸ� ������ ��ġ�� �ʵ��� ��ġ�Ѵ�
			int RandomNum = randomUtil.Gen(RANDOM_TYPE_INT, 0, 1);
			if (RandomNum == 1) {
				Coffee.Position.y = 0.14;
				Coffee.IsUpside = true;
				Other.Position.y = -0.27;
			}
			else {
				Coffee.Position.y = -0.27;
				Coffee.IsUpside = false;
				Other.Position.y = 0.14;
			}

			// x ��ġ�� Ŀ�ǿ� �ٸ� ������ ������ �Ѵ�
			Coffee.Position.x = PositionValue - 0.75 + 0.5 * i;
			Other.Position.x = Coffee.Position.x;

			CoffeeVec.emplace_back(Coffee);
			OtherVec.emplace_back(Other);
		}
	}

	void UpdateFunc(float FrameTime) {
		if (auto ED = scene.Find("ed"); ED) {
			// ī�޶� ��ġ�� �߰� ������ �����ϸ� ���� ������ �̸� �����Ѵ�
			if (!NextShelfGenerated && MiddlePoint <= CameraPosition.x) {
				NextShelfGenerated = true;
				scene.AddObject(new Shelf(NumShelf + 1, EndPoint + Length * 2.0), "shelf", LAYER2);

				// �̵尡 �̵��ؾ� �� ���� ��ġ�� �˸���
				ED->TellNextPosition(EndPoint + Length * 2.0 - 1.75);
			}
		}

		// ������ ������ ȭ�鿡�� ������ �ʰ� �Ǹ� ������ �����Ѵ�
		if (EndPoint < CameraPosition.x - ASP(1.0))
			scene.DeleteObject(this);
	}

	void RenderFunc() {
		// ���� ������
		for (int i = StartShelfIndex; i < NumShelf; i++) {
			GLfloat ShelfPosition = Position + Length * i;

			// ȭ�麸�� �����ʿ� �ִ� ������ �������� �ǳʶڴ�
			if (ShelfPosition > CameraPosition.x + ASP(1.0) + Length * 0.5)
				break;

			// �� �̻� ȭ�鿡�� ������ ���� ������ �������� �� �̻� ���� �ʴ´�.
			else if (Position + Length * StartShelfIndex < CameraPosition.x - ASP(1.0) - Length * 0.5) {
				StartShelfIndex++;
				continue;
			}

			Begin();
			transform.Move(MoveMatrix, ShelfPosition, 0.0);
			transform.Scale(MoveMatrix, Length, Length);

			// �� �պκа� �� �� �κ��� ������ ������ �κ��� ���� ���������� �������Ѵ�.
			if(i == 0) // �պκ�
				imageUtil.RenderStaticSpriteSheet(Img.Shelf, 0);

			else if(i == NumShelf - 1) // �޺κ�
				imageUtil.RenderStaticSpriteSheet(Img.Shelf, 2);

			else // �߰� �κ�
				imageUtil.RenderStaticSpriteSheet(Img.Shelf, 1);
		}


		// Ŀ�� ������
		size_t CoffeeVecSize = CoffeeVec.size();
		for (int i = StartCoffeeIndex; i < CoffeeVecSize; i++) {
			// ȭ�麸�� �����ʿ� �ִ� Ŀ�Ǵ� �������� �ǳʶڴ�.
			if (CoffeeVec[i].Position.x > CameraPosition.x + ASP(1.0) + 0.255)
				break;

			// �� �̻� ������ ���� Ŀ�Ǵ� �� �̻� ������ ���� �ʴ´�.
			else if (CoffeeVec[i].Position.x < CameraPosition.x - ASP(1.0) - 0.255) {
				StartCoffeeIndex++;
				continue;
			}

			Begin();
			transform.Move(MoveMatrix, CoffeeVec[i].Position);
			transform.Scale(MoveMatrix, 0.45, 0.45);

			// �ı��� Ŀ�Ǵ� �ı��� ��������Ʈ�� �������Ѵ�
			if(CoffeeVec[i].Destroyed)
				imageUtil.RenderStaticSpriteSheet(Img.DestroyedCoffee, CoffeeVec[i].Type);
			else
				imageUtil.RenderStaticSpriteSheet(Img.Coffee, CoffeeVec[i].Type);
		}
	

		// �ٸ� ���� ������
		size_t OtherVecSize = OtherVec.size();
		for (int i = StartOtherIndex; i < OtherVecSize; i++) {
			// ȭ�麸�� �����ʿ� �ִ� ������ �������� �ǳʶڴ�.
			if (OtherVec[i].Position.x > CameraPosition.x + ASP(1.0) + 0.255)
				break;

			// �� �̻� ������ ���� ������ �� �̻� ������ ���� �ʴ´�.
			else if (OtherVec[i].Position.x < CameraPosition.x - ASP(1.0) - 0.255) {
				StartOtherIndex++;
				continue;
			}

			Begin();
			transform.Move(MoveMatrix, OtherVec[i].Position);
			transform.Scale(MoveMatrix, 0.45, 0.45);
			imageUtil.RenderStaticSpriteSheet(Img.Other, OtherVec[i].Type);
		}
	}

	// ���� �տ� �ִ� Ŀ���� �� �Ǵ� �Ʒ��� ��ġ ���θ� ��´�
	// ���� ���� �� true,  �Ʒ��� ���� �� false
	ItemStruct GetFrontCoffee() {
		return CoffeeVec[CurrentCoffeeIndex];
	}

	// Ŀ�Ǹ� �μ� �� �ִ� ���·� ��ȯ�Ѵ�
	void EnableCoffeeHit() {
		CoffeeVec[CurrentCoffeeIndex].IsPeopleFront = false;
		CoffeeVec[CurrentCoffeeIndex].IsXionFront = false;
	}


	// ���� �տ� �ִ� Ŀ�Ǹ� �μ���.
	void BreakCoffee() {
		GameObject* Score = scene.Find("score_indicator");

		// ���� 3���� �� 1���� ���� ����
		int RandomNum = randomUtil.Gen(RANDOM_TYPE_INT, 0, 2);

		// Ŀ�� �������� �ٸ� ���带 ����Ѵ�
		soundUtil.Stop(SndChannel[StopChannel++]);

		// ���� ���� / 100 ��ŭ ���� ����
		switch (CoffeeVec[CurrentCoffeeIndex].Type) {
		case Box:
			soundUtil.Play(Snd.BoxBreak[RandomNum], SndChannel[PlayChannel++]);  
			Score->AddScore(10);
			Glb.BoxCoffeeBreakCount++;
			break;

		case Glass:
			soundUtil.Play(Snd.GlassBreak[RandomNum], SndChannel[PlayChannel++]);  
			Score->AddScore(10);
			Glb.GlassCoffeeBreakCount++;
			break;

		case Can:
			soundUtil.Play(Snd.CanBreak[RandomNum], SndChannel[PlayChannel++]);  
			Score->AddScore(10);
			Glb.CanCoffeeBreakCount++;
			break;
		}

		EX.ClampValue(StopChannel, 0, 4, CLAMP_RETURN);
		EX.ClampValue(PlayChannel, 0, 4, CLAMP_RETURN);

		// ���� Ŀ�� �̿��� Ŀ�Ǹ� �μ��� Ŀ�ǰ� ���������� �ִϸ��̼� ��ü�� �߰��Ѵ�
		if(CoffeeVec[CurrentCoffeeIndex].Type != Box)
			scene.AddObject(new Explode(CoffeeVec[CurrentCoffeeIndex].Position, false), "explode", LAYER5);
		// ���� Ŀ�Ƕ�� Ŀ�� ��ƽ�� ���������� �ִϸ��̼� ��ü�� �߰��Ѵ�
		else
			scene.AddObject(new Explode(CoffeeVec[CurrentCoffeeIndex].Position, true), "explode", LAYER5);

		// ĵĿ�Ƕ�� ��׷��� ĵ�� �߰��Ѵ�
		if (CoffeeVec[CurrentCoffeeIndex].Type == Can)
			scene.AddObject(new DestroyedCan(CoffeeVec[CurrentCoffeeIndex].Position), "destroyed_can", LAYER5);

		// Ŀ�Ǵ� �ı� ���°� �ǰ� �� �̻� �̵�� ��ȣ�ۿ����� �ʴ´�.
		CoffeeVec[CurrentCoffeeIndex].Destroyed = true;

		// ���� �ε��� ����
		CurrentCoffeeIndex++;

		// Ŀ�ǰ� ��� �ν����� �ڽ��� �±׸� ���� ���� ������ �˻��ǵ��� �Ѵ�.
		if (CurrentCoffeeIndex == NumShelf * 4 - 1)
			ObjectTag = "";
	}
};