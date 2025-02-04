#pragma once
#include <Scene.h>

class Shelf : public GameObject {
private:
	GameObject* PtrED{};

	int NumShelf{};

	// 선반 하나의 길이
	GLfloat Length = 2.0;

	// 선반 중간지점 위치
	GLfloat MiddlePoint{};
	           
	// 선반 끝지점 위치
	GLfloat EndPoint{};

	bool NextShelfGenerated{};

	GLfloat Position{};

public:
	Shelf(int Num, GLfloat PositionValue) {
		NumShelf = Num;
		Position = PositionValue;

		MiddlePoint = Length * (GLfloat)(Num - 1) * 0.5;
		EndPoint = Length * (GLfloat)(Num - 1) + Length * 0.5;

		PtrED = scene.Find("ed");
	}

	void UpdateFunc(float FrameTime) {
		if (!DeleteCommand) {
			GLfloat EDPosition = PtrED->GetPosition();

			// 이드가 선반의 중간 지점에 도달하면 다음 선반을 미리 생성한다
			if (!NextShelfGenerated && Position + MiddlePoint <= EDPosition) {
				NextShelfGenerated = true;
				scene.AddObject(new Shelf(NumShelf + 1, Position + EndPoint + Length * 2), "shelf", LAYER2);

				// 이드가 이동해야 할 다음 위치를 알린다
				PtrED->TellNextPosition(Position + EndPoint + Length * 2 - 1.75);
			}

			// 선반이 화면에서 보이지 않게 되면 스스로 삭제한다
			if (Position + EndPoint <= WindowRect.lx + EDPosition - 0.5) {
				scene.DeleteObject(this);
			}
		}
	}

	void RenderFunc() {
		for (int i = 0; i < NumShelf; i++) {
			Begin();
			transform.Move(MoveMatrix, Position + Length * i, 0.0);
			transform.Scale(MoveMatrix, Length, Length);

			// 맨 앞부분과 맨 뒷 부분을 제외한 나머지 부분은 같은 프레임으로 렌더링한다.
			if(i == 0)
				imageUtil.RenderStaticSpriteSheet(Img.Shelf, 0);
			else if(i == NumShelf - 1)
				imageUtil.RenderStaticSpriteSheet(Img.Shelf, 2);
			else
				imageUtil.RenderStaticSpriteSheet(Img.Shelf, 1);
		}
	}
};