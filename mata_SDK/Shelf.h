#pragma once
#include <Scene.h>

class Shelf : public GameObject {
private:
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

public:
	Shelf(int Num, GLfloat PositionValue) {
		NumShelf = Num;
		Position = PositionValue;

		// �߰� ���� �� �� ���� ���� ��� 
		MiddlePoint = Length * (GLfloat)(Num - 1) * 0.5;
		EndPoint = Length * (GLfloat)(Num - 1) + Length * 0.5;

		// �̵� ��ü ������ ����
		PtrED = scene.Find("ed");
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
	}
};