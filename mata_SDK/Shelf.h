#pragma once
#include <Scene.h>

#include "Explode.h"
#include "DestroyedCan.h"
#include "People.h"

class Shelf : public GameObject {
private:
	//////////////////////// 선반
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

	// 선반 렌더링을 시작하는 인덱스
	int StartShelfIndex{};

	// 다음 선반 생성 여부
	bool NextShelfGenerated{};

	//////////////////////// 물건
	// 커피들의 위치 및 종류를 저장하는 벡터
	std::vector<ItemStruct> CoffeeVec{};

	// 커피 렌더링을 시작하는 인덱스
	int StartCoffeeIndex{};

	// 가장 먼저 참조할 커피 인덱스
	int CurrentCoffeeIndex{};

	// 커피가 아닌 다른 물건들의 위치 및 종류를 저장하는 벡터
	std::vector<ItemStruct> OtherVec{}; 

	// 커피가 아닌 다른 물건들의 렌더링을 시작하는 인덱스
	int StartOtherIndex{};

	// 사운드 채널
	SoundChannel SndChannel[5]{};
	int PlayChannel = 0;
	int StopChannel = 2;

	// 사람을 추가할 것인지의 여부
	bool PeopleAddActivated{};

	// 사람이 추가된 자리 인덱스 번호
	int AddedIndex{};

	// 최대로 추가 가능한 사람 수
	// 전체 커피 개수 / 14로 개수를 결정한다
	int AddCountNum{};

public:
	Shelf(int Num, GLfloat PositionValue) {
		NumShelf = Num;
		Position = PositionValue;

		// 중간 지점 및 끝 지점 길이 계산 
		MiddlePoint = Position + Length * (GLfloat)(Num - 1) * 0.5;
		EndPoint = Position + Length * (GLfloat)(Num - 1) + Length * 0.5;

		//// 50퍼센트의 확률로 사람을 배치한다
		//if (NumShelf > 2) {
		//	int RandNum = randomUtil.Gen(RANDOM_TYPE_INT, 0, 1);
		//	if (RandNum == 1)
		//		PeopleAddActivated = true;
		//}

		// 선반 한 칸당 4개의 커피들을 랜덤으로 배치한다.
		// 마지막 칸은 3개만 배치한다.
		int GenTime = Num * 4 - 1;
		AddCountNum = Num * 4 - 1 / 7;

		for (int i = 0; i < GenTime; ++i) {
			ItemStruct Coffee{};
			ItemStruct Other{};

			// 최소 5칸 간격으로 배치한다
			if ((AddedIndex == 0 || i - AddedIndex > 5) && NumShelf > 2) {
				//각 커피 칸 마다 10퍼센트의 확률로 사람을 배치한다
				int RandNum = randomUtil.Gen(RANDOM_TYPE_INT, 1, 10);
				if (RandNum == 1) {
					glm::vec2 AddPosition = glm::vec2(PositionValue - 0.75 + 0.5 * i, 0.0);
					scene.AddObject(new People(AddPosition), "people", LAYER2);

					// 사람이 배치된 자리의 커피는 별도 표시한다
					Coffee.IsPeopleFront = true;

					// 마지막으로 사람을 추가한 인덱스 기록
					AddedIndex = i;
				}
			}

			// 타입 결정
			Coffee.Type = randomUtil.Gen(RANDOM_TYPE_INT, 0, 2);
			Other.Type = randomUtil.Gen(RANDOM_TYPE_INT, 0, 2);

			// 위치 결정
			// 1이면 위칸, 0이면 아래칸
			// 커피와 다른 물건이 겹치지 않도록 배치한다
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

			// x 위치는 커피와 다른 물건이 같도록 한다
			Coffee.Position.x = PositionValue - 0.75 + 0.5 * i;
			Other.Position.x = Coffee.Position.x;

			CoffeeVec.emplace_back(Coffee);
			OtherVec.emplace_back(Other);
		}
	}

	void UpdateFunc(float FrameTime) {
		// 카메라 위치가 중간 지점에 도달하면 다음 선반을 미리 생성한다
		if (!NextShelfGenerated && MiddlePoint <= CameraPosition.x) {
			NextShelfGenerated = true;
			scene.AddObject(new Shelf(NumShelf + 1, EndPoint + Length * 2.0), "shelf", LAYER1);

			// 이드가 이동해야 할 다음 위치를 알린다
			if(auto ED = scene.Find("ed"); ED)
				ED->TellNextPosition(EndPoint + Length * 2.0 - 1.75);
		}

		// 마지막 선반이 화면에서 보이지 않게 되면 스스로 삭제한다
		if (EndPoint < CameraPosition.x - ASP(1.0))
			scene.DeleteObject(this);
	}

	void RenderFunc() {
		// 선반 렌더링
		for (int i = StartShelfIndex; i < NumShelf; i++) {
			GLfloat ShelfPosition = Position + Length * i;

			// 화면보다 오른쪽에 있는 선반은 렌더링을 건너뛴다
			if (ShelfPosition > CameraPosition.x + ASP(1.0) + Length * 0.5)
				break;

			// 더 이상 화면에서 보이지 않을 선반은 렌더링을 더 이상 하지 않는다.
			else if (Position + Length * StartShelfIndex < CameraPosition.x - ASP(1.0) - Length * 0.5) {
				StartShelfIndex++;
				continue;
			}

			Begin();
			transform.Move(MoveMatrix, ShelfPosition, 0.0);
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
		size_t CoffeeVecSize = CoffeeVec.size();
		for (int i = StartCoffeeIndex; i < CoffeeVecSize; i++) {
			// 화면보다 오른쪽에 있는 커피는 렌더링을 건너뛴다.
			if (CoffeeVec[i].Position.x > CameraPosition.x + ASP(1.0) + 0.255)
				break;

			// 더 이상 보이지 않을 커피는 더 이상 렌더링 하지 않는다.
			else if (CoffeeVec[i].Position.x < CameraPosition.x - ASP(1.0) - 0.255) {
				StartCoffeeIndex++;
				continue;
			}

			Begin();
			transform.Move(MoveMatrix, CoffeeVec[i].Position);
			transform.Scale(MoveMatrix, 0.45, 0.45);

			// 파괴된 커피는 파괴된 스프라이트를 렌더링한다
			if(CoffeeVec[i].Destroyed)
				imageUtil.RenderStaticSpriteSheet(Img.DestroyedCoffee, CoffeeVec[i].Type);
			else
				imageUtil.RenderStaticSpriteSheet(Img.Coffee, CoffeeVec[i].Type);
		}
	

		// 다른 물건 렌더링
		size_t OtherVecSize = OtherVec.size();
		for (int i = StartOtherIndex; i < OtherVecSize; i++) {
			// 화면보다 오른쪽에 있는 물건은 렌더링을 건너뛴다.
			if (OtherVec[i].Position.x > CameraPosition.x + ASP(1.0) + 0.255)
				break;

			// 더 이상 보이지 않을 물건은 더 이상 렌더링 하지 않는다.
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

	// 가장 앞에 있는 커피의 위 또는 아래의 위치 여부를 얻는다
	// 위에 있을 시 true,  아래에 있을 시 false
	ItemStruct GetFrontCoffee() {
		return CoffeeVec[CurrentCoffeeIndex];
	}

	// 커피를 부술 수 있는 상태로 전환한다
	void EnableCoffeeHit() {
		CoffeeVec[CurrentCoffeeIndex].IsPeopleFront = false;
	}


	// 가장 앞에 있는 커피를 부순다.
	void BreakCoffee() {
		// 사운드 3가지 중 1가지 랜덤 선택
		int RandomNum = randomUtil.Gen(RANDOM_TYPE_INT, 0, 2);

		// 커피 종류마다 다른 사운드를 재생한다
		soundUtil.Stop(SndChannel[StopChannel++]);
		switch (CoffeeVec[CurrentCoffeeIndex].Type) {
		case Box:
			soundUtil.Play(Snd.BoxBreak[RandomNum], SndChannel[PlayChannel++]);  break;
		case Glass:
			soundUtil.Play(Snd.GlassBreak[RandomNum], SndChannel[PlayChannel++]);  break;
		case Can:
			soundUtil.Play(Snd.CanBreak[RandomNum], SndChannel[PlayChannel++]);  break;
		}

		EX.ClampValue(StopChannel, 0, 4, CLAMP_RETURN);
		EX.ClampValue(PlayChannel, 0, 4, CLAMP_RETURN);

		// 종이 커피 이외의 커피를 부수면 커피가 터져나오는 애니메이션 객체를 추가한다
		if(CoffeeVec[CurrentCoffeeIndex].Type != Box)
			scene.AddObject(new Explode(CoffeeVec[CurrentCoffeeIndex].Position, false), "explode", LAYER4);
		// 종이 커피라면 커피 스틱이 터져나오는 애니메이션 객체를 추가한다
		else
			scene.AddObject(new Explode(CoffeeVec[CurrentCoffeeIndex].Position, true), "explode", LAYER4);

		// 캔커피라면 찌그러진 캔을 추가한다
		if (CoffeeVec[CurrentCoffeeIndex].Type == Can)
			scene.AddObject(new DestroyedCan(CoffeeVec[CurrentCoffeeIndex].Position), "destroyed_can", LAYER4);

		// 커피는 파괴 상태가 되고 더 이상 이드와 상호작용하지 않는다.
		CoffeeVec[CurrentCoffeeIndex].Destroyed = true;

		// 참조 인덱스 증가
		CurrentCoffeeIndex++;

		// 커피가 모두 부숴지면 자신의 태그를 지워 다음 선반이 검색되도록 한다.
		if (CurrentCoffeeIndex == NumShelf * 4 - 1)
			ObjectTag = "";
	}
};