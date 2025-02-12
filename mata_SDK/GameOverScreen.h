#pragma once
#include <Scene.h>

#include "Cover.h"
#include "TitleMode.h"

struct Reparations {
	int DestroyedType;
	int NumDestroyed;
};

class GameOverScreen : public GameObject {
private:
	// 클로에가 뺨을 때리는 간격 타이머
	TimerUtil Timer{};

	// 클로에가 뺨을 때리는 간격
	GLfloat CheekInterval{};

	// 시온이 떠는 간격 타이머
	TimerUtil ShiverTimer{};

	// 떠는 애니메이션 수치
	glm::vec2 ShiverValue{};
	glm::vec2 ShiverValue2{};

	// 상하 크기 수치
	GLfloat VerticalSize{};

	// 텍스트 크기
	GLfloat TextSize{};

	// 텍스트 크기 루프
	SinLoop TextSizeLoop{};

	// 프레임
	int ChloeFrame{};
	int EDFrame{};

	// 이드 프레임 타이머
	TimerUtil EDFrameTimer{};

	// 홈모드 전환 상태
	bool ExitState{};

	std::wstring Str{};
	std::wstring Str2{};

	TextUtil Text{}, ScoreText{};

	// 배상금 
	///////
	// 배상금 표시되는 상태
	bool ShowRep{};

	// 총 배상금 표시되는 상태
	bool ShowTotalRep{};
	 
	// 배상 품목을 저장하는 벡터
	std::vector <Reparations> RepVec{};
	size_t VecSize{};
	int CurrentSize{};

	// 총 배상금
	int TotalRep{};

	// 배상금액을 순차적으로 보여주기 위한 타이머
	TimerUtil RepTimer{};

	// 총 배상금액 텍스트 흔들림 값
	glm::vec2 TextShake{};
	GLfloat TextShakeValue{0.05};
	TimerUtil TextShakeTimer{};

	SoundChannel SndChannel1{};
	SoundChannel SndChannel2{};
	SoundChannel SndChannel3{};
	GLfloat Volume = 1.0;


	// 타이틀로 나가기 지연 타이머
	TimerUtil DeleteTimer{};

public:
	GameOverScreen() {
		System.SetBackColorRGB(74, 78, 101);

		Text.Init(L"픽셀로보로보체", FW_DONTCARE);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.SetUseShadow(true);
		Text.SetShadow(0.1, glm::vec3(0.0, 0.0, 0.0), 0.7);
		Text.SetColor(1.0, 1.0, 1.0);
		Text.Rotate(-10.0);

		ScoreText.Init(L"픽셀로보로보체", FW_DONTCARE);
		ScoreText.SetAlign(ALIGN_MIDDLE);
		ScoreText.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		ScoreText.SetUseShadow(true);
		ScoreText.SetShadow(0.1, glm::vec3(0.0, 0.0, 0.0), 0.7);
		ScoreText.SetColor(1.0, 1.0, 1.0);
		ScoreText.SetLineGap(0.02);

		if (Glb.Ending == GameOver_TimeOut) {
			CheekInterval = 0.7;
			Str = L"유죄!!!";
			Str2 = L"미쳤어?!!";
		}

		else if (Glb.Ending == GameOver_HitXion) {
			CheekInterval = 0.25;
			Str = L"찐짜 유죄!!!";
			Str2 = L"언니를 걷어 차?!!";
		}

		else if (Glb.Ending == GameOver_Suppressed) {
			Str = L"무죄!!!";
			Str2 = L"이드! 해냈구나!";
		}

		soundUtil.Play(Snd.GameEnd[Glb.Ending], SndChannel1);

		// 파괴한 품목이 존재하면 벡터에 저장한다
		for (int i = 0; i < 6; i++) {
			Reparations Rep{};
			if (Glb.DestroyedItems[i] > 0) {
				Rep.DestroyedType = i;
				Rep.NumDestroyed = Glb.DestroyedItems[i];
				RepVec.emplace_back(Rep);
			}
		}

		VecSize = RepVec.size();

		// 총 배상액 계산
		TotalRep += Glb.DestroyedItems[Item_BoxCoffee] * 25000;
		TotalRep += Glb.DestroyedItems[Item_BottleCoffee] * 2500;
		TotalRep += Glb.DestroyedItems[Item_CanCoffee] * 1200;
		TotalRep += Glb.DestroyedItems[Item_People] * 1000000;
		TotalRep += Glb.DestroyedItems[Item_Xion] * 1000000;
		TotalRep += Glb.DestroyedItems[Item_HouseDoc] * 100000;
	}

	void InputKey(KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN && Event.NormalKey == NK_ENTER) {
			// 첫 번쨰 입력은 총 배상금 화면으로 전환하고, 두 번째 입력은 홈모드로 전환한다
			if (!ShowRep) {
				ShowRep = true;
				ScoreText.SetUseShadow(false);
			}
			else {
				ExitState = true;
				scene.AddObject(new Cover, "cover", LAYER7);
				// 나가는 상태를 활성화하고 더 이상 입력을 받지 않도록 한다
				scene.DeleteInputObject(this);
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		if (Glb.Ending == GameOver_TimeOut || Glb.Ending == GameOver_HitXion) {
			Timer.Update(FrameTime);
			if (Timer.CheckMiliSec(CheekInterval, 2, CHECK_AND_INTERPOLATE)) {
				ChloeFrame++;
				
				// 일정 간격으로 클로에가 이드의 뺨을 때린다
				if (ChloeFrame == 1) {
					EDFrame = 1;
					EDFrameTimer.Reset();
					VerticalSize = 0.3;
					soundUtil.Stop(SndChannel2);
					soundUtil.Play(Snd.HitCheek, SndChannel2);
				}
				EX.ClampValue(ChloeFrame, 0, 1, CLAMP_RETURN);
			}

			// 떨림 애니메이션
			ShiverTimer.Update(FrameTime);
			if (ShiverTimer.CheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE)) {
				ShiverValue.x = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				ShiverValue.y = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				ShiverValue2.x = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				ShiverValue2.y = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
			}

			// 수직 크기 애니메이션
			mathUtil.Lerp(VerticalSize, 0.0, 10.0, FrameTime);

			// 이드는 뺨을 맞은 후 조금 후에 다시 원래 프레임으로 복귀한다
			if (EDFrame == 1) {
				EDFrameTimer.Update(FrameTime);
				if (EDFrameTimer.CheckMiliSec(CheekInterval * 0.5, 2, CHECK_AND_RESET))
					EDFrame = 0;
			}

			// 텍스트 애니메이션
			TextSize = TextSizeLoop.Update(0.01, 6.0, FrameTime);
		}

		// 각 배상액을 순차적으로 보여준다
		if (ShowRep) {
			RepTimer.Update(FrameTime);
			if (CurrentSize != VecSize && RepTimer.CheckMiliSec(0.5, 1, CHECK_AND_INTERPOLATE)) {
				CurrentSize++;
				soundUtil.Play(Snd.Rep, SndChannel3);
			}
			
			else if (CurrentSize == VecSize && RepTimer.CheckMiliSec(1, 1, CHECK_AND_INTERPOLATE)) {
				ShowTotalRep = true;
				soundUtil.Play(Snd.RepTotal, SndChannel3);
				RepTimer.Stop();
				RepTimer.Reset();
			}

			// 배상액 총액은 흔들리는 애니메이션과 함께 보여준다
			if (ShowTotalRep) {
				TextShakeTimer.Update(FrameTime);
				if (TextShakeTimer.CheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE)) {
					TextShake.x = randomUtil.Gen(RANDOM_TYPE_REAL, -TextShakeValue, TextShakeValue);
					TextShake.y = randomUtil.Gen(RANDOM_TYPE_REAL, -TextShakeValue, TextShakeValue);
				}

				mathUtil.Lerp(TextShakeValue, 0.0, 5.0, FrameTime);
			}
		}

		// 화면이 어두워지면서 홈모드로 전환한다
		// 볼륨을 부드럽게 줄인다
		if (ExitState) {
			Volume -= FrameTime * 0.5;
			EX.ClampValue(Volume, 0.0, CLAMP_LESS);
			SndChannel1->setVolume(Volume);
			SndChannel2->setVolume(Volume);

			if(auto Cover = scene.Find("cover"); Cover)
				if (Cover->GetState()) {
					DeleteTimer.Update(FrameTime);
					if (DeleteTimer.Sec() >= 1)
						scene.SwitchMode(TitleMode.Start);
				}
		}
	}

	void RenderFunc() {
		// 엔딩마다 시온의 프레임이 달라진다
		if (Glb.Ending == GameOver_TimeOut || Glb.Ending == GameOver_HitXion || Glb.Ending == GameOver_Suppressed) {
			// 배경 렌더링
			Begin();
			transform.Scale(MoveMatrix, 3.0, 3.0);
			imageUtil.Render(Img.GameOverBackGround, 0.7);

			// 시온 렌더링
			Begin();
			transform.Move(MoveMatrix, ASP(-1.0) + 0.5 + ShiverValue.x, -0.3 + ShiverValue.y);
			transform.Scale(MoveMatrix, 1.5, 1.5);

			if(Glb.Ending == GameOver_HitXion)
				imageUtil.RenderStaticSpriteSheet(Img.Xion, Xion_Hurt);
			else if(Glb.Ending == GameOver_TimeOut)
				imageUtil.RenderStaticSpriteSheet(Img.Xion, Xion_Cry1);

			// 이드 렌더링
			Begin();
			transform.Move(MoveMatrix, ASP(1.0) - 1.03 + ShiverValue2.x - VerticalSize * 0.5, -0.7 - VerticalSize * 0.5 + ShiverValue2.y);
			transform.Scale(MoveMatrix, 2.0 + VerticalSize, 2.0 - VerticalSize);
			transform.Shear(MoveMatrix, -VerticalSize * 0.5, 0.0);
			imageUtil.RenderStaticSpriteSheet(Img.ED_GameOver, EDFrame);

			// 클로에 렌더링
			Begin();
			transform.Move(MoveMatrix, ASP(1.0) - 0.5 + ShiverValue2.x - VerticalSize * 0.5, -0.6 - VerticalSize * 0.5 + ShiverValue2.y);
			transform.Scale(MoveMatrix, 2.2 + VerticalSize, 2.2 - VerticalSize);
			transform.Shear(MoveMatrix, -VerticalSize * 0.5, 0.0);
			imageUtil.RenderStaticSpriteSheet(Img.Chloe, ChloeFrame);
			
			if (!ShowRep) {
				// 텍스트 출력
				Text.Render(ASP(-1.0) + 0.8, 0.85, 0.2 + TextSize, Str.c_str());
				Text.Render(ASP(-1.0) + 0.8, 0.6, 0.1 + TextSize, Str2.c_str());

				// 점수 출력
				ScoreText.Render(0.0, 0.5, 0.15, L"SCORE: %d", Glb.Score);
			}

			// 배상금 출력
			else {
				// 뒷 배경 출력
				Begin();
				transform.Scale(MoveMatrix, ASP(2.0), 2.0);
				imageUtil.Render(SysRes.COLOR_TEXTURE, 0.6);

				// 배상금 계산
				GLfloat RenderHeight{0.8};

				for (int i = 0; i < CurrentSize; i++) {
					ScoreText.SetColor(1.0, 1.0, 1.0);

					switch (RepVec[i].DestroyedType) {
					case Item_BoxCoffee:
						ScoreText.Render(0.0, RenderHeight, 0.1, L"커피 상자 파손: %d개 x 25000골드", Glb.DestroyedItems[RepVec[i].DestroyedType]);
						break;

					case Item_BottleCoffee:
						ScoreText.Render(0.0, RenderHeight, 0.1, L"병 커피 파손: %d개 x 2500골드", Glb.DestroyedItems[RepVec[i].DestroyedType]);
						break;

					case Item_CanCoffee:
						ScoreText.Render(0.0, RenderHeight, 0.1, L"캔 커피 파손: %d개 x 1200골드", Glb.DestroyedItems[RepVec[i].DestroyedType]);
						break;

					case Item_People:
						ScoreText.Render(0.0, RenderHeight, 0.1, L"타인 상해: %d명 x 1000000골드", Glb.DestroyedItems[RepVec[i].DestroyedType]);
						break;

					case Item_Xion:
						ScoreText.Render(0.0, RenderHeight, 0.1, L"시온 치료비: 1000000골드");
						break;

					case Item_HouseDoc:
						ScoreText.Render(0.0, RenderHeight, 0.1, L"불 탄 집문서: %d장 x 100000골드", Glb.DestroyedItems[RepVec[i].DestroyedType]);
						break;
					}

					RenderHeight -= 0.2;
				}

				if (ShowTotalRep) {
					ScoreText.SetColorRGB(255, 213, 80);
					ScoreText.Render(0.0 + TextShake.x, -0.7 + TextShake.y, 0.15, L"총 배상액: %d 골드", TotalRep);
				}
			}
		}
	}
};