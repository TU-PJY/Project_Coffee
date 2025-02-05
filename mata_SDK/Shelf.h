#pragma once
#include <Scene.h>

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

	// ������ ������ �����ϱ� ���� �̵��� ���� ��ġ
	GLfloat EDPosition{};

	//////////////////////// ����
	// Ŀ�ǵ��� ��ġ �� ������ �����ϴ� ����
	std::vector<ItemStruct> CoffeeVec{};

	// Ŀ�ǰ� �ƴ� �ٸ� ���ǵ��� ��ġ �� ������ �����ϴ� ����
	std::vector<ItemStruct> OtherVec{}; 

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
		// �̵� ��ġ ���
		EDPosition = PtrED->GetPosition();

		// �̵尡 ������ �߰� ������ �����ϸ� ���� ������ �̸� �����Ѵ�
		if (!NextShelfGenerated && MiddlePoint <= EDPosition) {
			NextShelfGenerated = true;
			scene.AddObject(new Shelf(NumShelf + 1, EndPoint + Length * 2.0), "shelf", LAYER2);

			// �̵尡 �̵��ؾ� �� ���� ��ġ�� �˸���
			PtrED->TellNextPosition(EndPoint + Length * 2.0 - 1.75);
		}

		// ������ ȭ�鿡�� ������ �ʰ� �Ǹ� ������ �����Ѵ�
		if (EndPoint < WindowRect.lx - CameraPosition.x) 
			scene.DeleteObject(this);
	}

	void RenderFunc() {
		// ���� ������
		for (int i = 0; i < NumShelf; i++) {
			GLfloat ShelfPosition = Position + Length * i;

			// ȭ�鿡 ������ �ʴ� ������ �������� �ǳʶڴ�.
			if (ShelfPosition < WindowRect.lx -CameraPosition.x - Length * 0.5 ||
				ShelfPosition > -CameraPosition.x + WindowRect.rx + Length * 0.5)
				continue;

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
		for (auto& Coffee : CoffeeVec) {
			// ȭ�鿡 ������ �ʴ� Ŀ�Ǵ� �������� �ǳʶڴ�.
			if (Coffee.Position.x < WindowRect.lx - CameraPosition.x - 0.225 || 
				Coffee.Position.x > -CameraPosition.x + WindowRect.rx + 0.225)
				continue;

			Begin();
			transform.Move(MoveMatrix, Coffee.Position);
			transform.Scale(MoveMatrix, 0.45, 0.45);
			imageUtil.RenderStaticSpriteSheet(Img.Coffee, Coffee.Type);
		}

		// �ٸ� ���� ������
		for (auto& Other : OtherVec) {
			// ȭ�鿡 ������ �ʴ� ������ �������� �ǳʶڴ�.
			if (Other.Position.x < WindowRect.lx - CameraPosition.x - 0.225 || 
				Other.Position.x > -CameraPosition.x + WindowRect.rx + 0.225)
				continue;

			Begin();
			transform.Move(MoveMatrix, Other.Position);
			transform.Scale(MoveMatrix, 0.45, 0.45);
			imageUtil.RenderStaticSpriteSheet(Img.Other, Other.Type);
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