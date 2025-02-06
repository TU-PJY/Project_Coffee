#pragma once
#include <Scene.h>

#include "Explode.h"

// ���� ����ü
typedef struct {
	// Ŀ�� ����
	int Type;

	// Ŀ�� ��ġ
	glm::vec2 Position;
}ItemStruct;

class Shelf : public GameObject {
private:
	//////////////////////// ����
	// �̵� ��ü ������
	GameObject* PtrED{};

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

	// Ŀ�ǰ� �ƴ� �ٸ� ���ǵ��� ��ġ �� ������ �����ϴ� ����
	std::vector<ItemStruct> OtherVec{}; 

	// Ŀ�ǰ� �ƴ� �ٸ� ���ǵ��� �������� �����ϴ� �ε���
	int StartOtherIndex{};

	// Ŀ���� ��ġ�� �����ϴ� ��
	std::deque<bool> IndexVec{};

	// ���� ä��
	SoundChannel SndChannel[5]{};
	int PlayChannel = 0;
	int StopChannel = 2;

public:
	Shelf(int Num, GLfloat PositionValue) {
		NumShelf = Num;
		Position = PositionValue;

		// �߰� ���� �� �� ���� ���� ��� 
		MiddlePoint = Position + Length * (GLfloat)(Num - 1) * 0.5;
		EndPoint = Position + Length * (GLfloat)(Num - 1) + Length * 0.5;

		// �̵� ��ü ������ ����
		PtrED = scene.Find("ed");

		// ���� �� ĭ�� 4���� Ŀ�ǵ��� �������� ��ġ�Ѵ�.
		// ������ ĭ�� 3���� ��ġ�Ѵ�.
		int GenTime = Num * 4 - 1;
		for (int i = 0; i < GenTime; ++i) {
			ItemStruct Coffee{};
			ItemStruct Other{};

			// Ÿ�� ����
			Coffee.Type = randomUtil.Gen(RANDOM_TYPE_INT, 0, 2);
			Other.Type = randomUtil.Gen(RANDOM_TYPE_INT, 0, 2);

			// ��ġ ����
			// 1�̸� ��ĭ, 0�̸� �Ʒ�ĭ
			// Ŀ�ǿ� �ٸ� ������ ��ġ�� �ʵ��� ��ġ�Ѵ�
			int RandomNum = randomUtil.Gen(RANDOM_TYPE_INT, 0, 1);
			if (RandomNum == 1) {
				Coffee.Position.y = 0.14;
				Other.Position.y = -0.27;
			}
			else {
				Coffee.Position.y = -0.27;
				Other.Position.y = 0.14;
			}

			// x ��ġ�� Ŀ�ǿ� �ٸ� ������ ������ �Ѵ�
			Coffee.Position.x = PositionValue - 0.75 + 0.5 * i;
			Other.Position.x = Coffee.Position.x;
			CoffeeVec.emplace_back(Coffee);
			OtherVec.emplace_back(Other);

			// ���Ʒ� ��ġ ���θ� ������ �����Ͽ� �̵�� ��ȣ�ۿ� �� ���
			IndexVec.emplace_back(RandomNum);
		}
	}

	void UpdateFunc(float FrameTime) {
		// ī�޶� ��ġ�� �߰� ������ �����ϸ� ���� ������ �̸� �����Ѵ�
		if (!NextShelfGenerated && MiddlePoint <= CameraPosition.x) {
			NextShelfGenerated = true;
			scene.AddObject(new Shelf(NumShelf + 1, EndPoint + Length * 2.0), "shelf", LAYER2);

			// �̵尡 �̵��ؾ� �� ���� ��ġ�� �˸���
			PtrED->TellNextPosition(EndPoint + Length * 2.0 - 1.75);
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
				continue;

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
				continue;

			// �� �̻� ������ ���� Ŀ�Ǵ� �� �̻� ������ ���� �ʴ´�.
			else if (CoffeeVec[i].Position.x < CameraPosition.x - ASP(1.0) - 0.255) {
				StartCoffeeIndex++;
				continue;
			}

			Begin();
			transform.Move(MoveMatrix, CoffeeVec[i].Position);
			transform.Scale(MoveMatrix, 0.45, 0.45);
			imageUtil.RenderStaticSpriteSheet(Img.Coffee, CoffeeVec[i].Type);
		}
	
		// �ٸ� ���� ������
		size_t OtherVecSize = OtherVec.size();
		for (int i = StartOtherIndex; i < OtherVecSize; i++) {
			// ȭ�麸�� �����ʿ� �ִ� ������ �������� �ǳʶڴ�.
			if (OtherVec[i].Position.x > CameraPosition.x + ASP(1.0) + 0.255)
				continue;

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
	bool GetFrontCoffee() {
		return IndexVec.front();
	}

	// ���� �տ� �ִ� Ŀ�Ǹ� �μ���.
	void BreakCoffee() {
		// Ŀ�� �������� �ٸ� �μ����� ���带 ����Ѵ�
		int RandomNum = randomUtil.Gen(RANDOM_TYPE_INT, 0, 2);

		soundUtil.Stop(SndChannel[StopChannel++]);

		if (CoffeeVec.begin()->Type == Box)
			soundUtil.Play(Snd.BoxBreak[RandomNum], SndChannel[PlayChannel++]);

		else if (CoffeeVec.begin()->Type == Glass)
			soundUtil.Play(Snd.GlassBreak[RandomNum], SndChannel[PlayChannel++]);

		else if (CoffeeVec.begin()->Type == Can)
			soundUtil.Play(Snd.CanBreak[RandomNum], SndChannel[PlayChannel++]);

		EX.ClampValue(StopChannel, 0, 4, CLAMP_RETURN);
		EX.ClampValue(PlayChannel, 0, 4, CLAMP_RETURN);

		// ���� Ŀ�� �̿��� Ŀ�Ǹ� �μ��� Ŀ�ǰ� ���������� �ִϸ��̼� ��ü�� �߰��Ѵ�
		if(CoffeeVec.begin()->Type != Box)
			scene.AddObject(new Explode(CoffeeVec.begin()->Position, false), "explode", LAYER3);
		// ���� Ŀ�Ƕ�� Ŀ�� ��ƽ�� ���������� �ִϸ��̼� ��ü�� �߰��Ѵ�
		else
			scene.AddObject(new Explode(CoffeeVec.begin()->Position, true), "explode", LAYER3);

		IndexVec.erase(IndexVec.begin());
		CoffeeVec.erase(CoffeeVec.begin());

		// Ŀ�ǰ� ��� �ν����� �ڽ��� �±׸� ���� ���� ������ �˻��ǵ��� �Ѵ�.
		if (IndexVec.empty())
			ObjectTag = "";
	}
};