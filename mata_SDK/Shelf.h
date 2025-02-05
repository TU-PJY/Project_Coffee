#pragma once
#include <Scene.h>

// 커피 구조체
typedef struct {
	// 커피 종류
	int Type;

	// 커피 위치
	glm::vec2 Position;
}CoffeeStruct;

class Shelf : public GameObject {
private:
	//////////////////////// 선반
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

	//////////////////////// 커피
	// 커피들의 위치 및 종류를 저장하는 벡터
	std::vector<CoffeeStruct> CoffeeVec{};

	// 커피의 위치를 저장하는 벡터
	std::vector<bool> IndexVec{};

	// 사운드 채널
	SoundChannel SndChannel[5]{};
	int PlayChannel = 0;
	int StopChannel = 2;

public:
	Shelf(int Num, GLfloat PositionValue) {
		NumShelf = Num;
		Position = PositionValue;

		// 중간 지점 및 끝 지점 길이 계산 
		MiddlePoint = Length * (GLfloat)(Num - 1) * 0.5;
		EndPoint = Length * (GLfloat)(Num - 1) + Length * 0.5;

		// 이드 객체 포인터 연결
		PtrED = scene.Find("ed");

		// 선반 한 칸당 4개의 커피들을 랜덤으로 배치한다.
		// 마지막 칸은 3개만 배치한다.
		int GenTime = Num * 4 - 1;
		for (int i = 0; i < GenTime; ++i) {
			CoffeeStruct Coffee{};
			// 타입 결정
			Coffee.Type = randomUtil.Gen(RANDOM_TYPE_INT, 0, 2);

			// 위치 결정
			// 1이면 위칸, 0이면 아래칸
			int RandomNum = randomUtil.Gen(RANDOM_TYPE_INT, 0, 1);
			if (RandomNum == 1)
				Coffee.Position.y = 0.14;
			else
				Coffee.Position.y = -0.27;

			Coffee.Position.x = PositionValue - 0.75 + 0.5 * i;
			CoffeeVec.emplace_back(Coffee);

			// 위아래 위치 여부만 별도로 저장하여 이드와 상호작용 시 사용
			IndexVec.emplace_back(RandomNum);
		}
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
		// 선반 렌더링
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

		// 커피 렌더링
		for (auto& Coffee : CoffeeVec) {
			Begin();
			transform.Move(MoveMatrix, Coffee.Position);
			transform.Scale(MoveMatrix, 0.45, 0.45);
			imageUtil.RenderStaticSpriteSheet(Img.Coffee, Coffee.Type);
		}
	}

	// 가장 앞에 있는 커피의 위 또는 아래의 위치 여부를 얻는다
	// 위에 있을 시 true,  아래에 있을 시 false
	bool GetFrontCoffee() {
		return IndexVec.front();
	}

	// 가장 앞에 있는 커피를 부순다.
	void BreakCoffee() {
		// 커피 종류마다 다른 부서지는 사운드를 재생한다
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

		// 커피가 모두 부숴지면 자신의 태그를 지워 다음 선반이 검색되도록 한다.
		if (IndexVec.empty())
			ObjectTag = "";
	}
};