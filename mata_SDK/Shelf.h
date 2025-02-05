#pragma once
#include <Scene.h>

// Ŀ�� ����ü
typedef struct {
	// Ŀ�� ����
	int Type;

	// Ŀ�� ��ġ
	glm::vec2 Position;
}CoffeeStruct;

class Shelf : public GameObject {
private:
	//////////////////////// ����
	// �̵��� ��ġ���� ��� ���� �̵� ��ü ������
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

	// ���� ���� ���� ����
	bool NextShelfGenerated{};

	//////////////////////// Ŀ��
	// Ŀ�ǵ��� ��ġ �� ������ �����ϴ� ����
	std::vector<CoffeeStruct> CoffeeVec{};

	// Ŀ���� ��ġ�� �����ϴ� ����
	std::vector<bool> IndexVec{};

	// ���� ä��
	SoundChannel SndChannel[5]{};
	int PlayChannel = 0;
	int StopChannel = 2;

public:
	Shelf(int Num, GLfloat PositionValue) {
		NumShelf = Num;
		Position = PositionValue;

		// �߰� ���� �� �� ���� ���� ��� 
		MiddlePoint = Length * (GLfloat)(Num - 1) * 0.5;
		EndPoint = Length * (GLfloat)(Num - 1) + Length * 0.5;

		// �̵� ��ü ������ ����
		PtrED = scene.Find("ed");

		// ���� �� ĭ�� 4���� Ŀ�ǵ��� �������� ��ġ�Ѵ�.
		// ������ ĭ�� 3���� ��ġ�Ѵ�.
		int GenTime = Num * 4 - 1;
		for (int i = 0; i < GenTime; ++i) {
			CoffeeStruct Coffee{};
			// Ÿ�� ����
			Coffee.Type = randomUtil.Gen(RANDOM_TYPE_INT, 0, 2);

			// ��ġ ����
			// 1�̸� ��ĭ, 0�̸� �Ʒ�ĭ
			int RandomNum = randomUtil.Gen(RANDOM_TYPE_INT, 0, 1);
			if (RandomNum == 1)
				Coffee.Position.y = 0.14;
			else
				Coffee.Position.y = -0.27;

			Coffee.Position.x = PositionValue - 0.75 + 0.5 * i;
			CoffeeVec.emplace_back(Coffee);

			// ���Ʒ� ��ġ ���θ� ������ �����Ͽ� �̵�� ��ȣ�ۿ� �� ���
			IndexVec.emplace_back(RandomNum);
		}
	}

	void UpdateFunc(float FrameTime) {
		// �̵� ��ġ ���
		GLfloat EDPosition = PtrED->GetPosition();

		// �̵尡 ������ �߰� ������ �����ϸ� ���� ������ �̸� �����Ѵ�
		if (!NextShelfGenerated && Position + MiddlePoint <= EDPosition) {
			NextShelfGenerated = true;
			scene.AddObject(new Shelf(NumShelf + 1, Position + EndPoint + Length * 2.0), "shelf", LAYER2);

			// �̵尡 �̵��ؾ� �� ���� ��ġ�� �˸���
			PtrED->TellNextPosition(Position + EndPoint + Length * 2.0 - 1.75);
		}

		// ������ ȭ�鿡�� ������ �ʰ� �Ǹ� ������ �����Ѵ�
		if (Position + EndPoint <= WindowRect.lx + EDPosition - 0.5)
			scene.DeleteObject(this);
	}

	void RenderFunc() {
		// ���� ������
		for (int i = 0; i < NumShelf; i++) {
			Begin();
			transform.Move(MoveMatrix, Position + Length * i, 0.0);
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
		for (auto& Coffee : CoffeeVec) {
			Begin();
			transform.Move(MoveMatrix, Coffee.Position);
			transform.Scale(MoveMatrix, 0.45, 0.45);
			imageUtil.RenderStaticSpriteSheet(Img.Coffee, Coffee.Type);
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

		IndexVec.erase(IndexVec.begin());
		CoffeeVec.erase(CoffeeVec.begin());

		// Ŀ�ǰ� ��� �ν����� �ڽ��� �±׸� ���� ���� ������ �˻��ǵ��� �Ѵ�.
		if (IndexVec.empty())
			ObjectTag = "";
	}
};