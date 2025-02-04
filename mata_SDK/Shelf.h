#pragma once
#include <Scene.h>

class Shelf : public GameObject {
private:
	// 이드의 위치값을 얻기 위한 이드 객체 포인터
	GameObject* PtrED{};

	// 선반 개수
	int NumShelf{};

	// 생성 기준 위치
	GLfloat Position{};

	// 선반 하나의 길이
	GLfloat Length = 2.0;

	// 선반 중간지점 위치
	GLfloat MiddlePoint{};
	           
	// 선반 끝지점 위치
	GLfloat EndPoint{};

	// 다음 선반 생성 여부
	bool NextShelfGenerated{};

public:
	Shelf(int Num, GLfloat PositionValue) {
		NumShelf = Num;
		Position = PositionValue;

		// 중간 지점 및 끝 지점 길이 계산 
		MiddlePoint = Length * (GLfloat)(Num - 1) * 0.5;
		EndPoint = Length * (GLfloat)(Num - 1) + Length * 0.5;

		// 이드 객체 포인터 연결
		PtrED = scene.Find("ed");
	}

	void UpdateFunc(float FrameTime) {
		// 이드 위치 얻기
		GLfloat EDPosition = PtrED->GetPosition();

		// 이드가 선반의 중간 지점에 도달하면 다음 선반을 미리 생성한다
		if (!NextShelfGenerated && Position + MiddlePoint <= EDPosition) {
			NextShelfGenerated = true;
			scene.AddObject(new Shelf(NumShelf + 1, Position + EndPoint + Length * 2.0), "shelf", LAYER2);

			// 이드가 이동해야 할 다음 위치를 알린다
			PtrED->TellNextPosition(Position + EndPoint + Length * 2.0 - 1.75);
		}

		// 선반이 화면에서 보이지 않게 되면 스스로 삭제한다
		if (Position + EndPoint <= WindowRect.lx + EDPosition - 0.5)
			scene.DeleteObject(this);
	}

	void RenderFunc() {
		for (int i = 0; i < NumShelf; i++) {
			Begin();
			transform.Move(MoveMatrix, Position + Length * i, 0.0);
			transform.Scale(MoveMatrix, Length, Length);

			// 맨 앞부분과 맨 뒷 부분을 제외한 나머지 부분은 같은 프레임으로 렌더링한다.
			if(i == 0) // 앞부분
				imageUtil.RenderStaticSpriteSheet(Img.Shelf, 0);

			else if(i == NumShelf - 1) // 뒷부분
				imageUtil.RenderStaticSpriteSheet(Img.Shelf, 2);

			else // 중간 부분
				imageUtil.RenderStaticSpriteSheet(Img.Shelf, 1);
		}
	}
};