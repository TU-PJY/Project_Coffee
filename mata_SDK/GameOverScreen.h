#pragma once
#include <Scene.h>

#include "Cover.h"

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

	int ChloeFrame{};
	int EDFrame{};

	// 이드 프레임 타이머
	TimerUtil EDFrameTimer{};

	SoundChannel SndChannel1{};
	SoundChannel SndChannel2{};

	GLfloat Volume = 1.0;
	bool ExitState{};

public:
	GameOverScreen() {
		System.SetBackColorRGB(74, 78, 101);

		if (Glb.Ending == TimeOut) 
			CheekInterval = 0.7;

		else if (Glb.Ending == HitXion) 
			CheekInterval = 0.25;

		soundUtil.Play(Snd.GameEnd[Glb.Ending], SndChannel1);
	}

	void InputKey(KeyEvent& Event) {
		// 나가는 상태를 활성화하고 dj 이상 입력을 받지 않도록 한다
		if (Event.Type == NORMAL_KEY_DOWN && Event.NormalKey == NK_ENTER) {
			ExitState = true;
			scene.AddObject(new Cover, "cover", LAYER7);
			scene.DeleteInputObject(this);
		}
	}

	void UpdateFunc(float FrameTime) {
		if (Glb.Ending == TimeOut || Glb.Ending == HitXion) {
			Timer.Update(FrameTime);
			if (Timer.CheckMiliSec(CheekInterval, 2, CHECK_AND_INTERPOLATE)) {
				ChloeFrame++;

				if (ChloeFrame == 1) {
					EDFrame = 1;
					EDFrameTimer.Reset();
					VerticalSize = 0.3;
					soundUtil.Stop(SndChannel2);
					soundUtil.Play(Snd.HitCheek, SndChannel2);
				}
				EX.ClampValue(ChloeFrame, 0, 1, CLAMP_RETURN);
			}

			ShiverTimer.Update(FrameTime);
			if (ShiverTimer.CheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE)) {
				ShiverValue.x = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				ShiverValue.y = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				ShiverValue2.x = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				ShiverValue2.y = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
			}

			mathUtil.Lerp(VerticalSize, 0.0, 10.0, FrameTime);

			if (EDFrame == 1) {
				EDFrameTimer.Update(FrameTime);
				if (EDFrameTimer.CheckMiliSec(CheekInterval * 0.5, 2, CHECK_AND_RESET))
					EDFrame = 0;
			}

			TextSize = TextSizeLoop.Update(0.01, 6.0, FrameTime);
		}

		if (ExitState) {
			Volume -= FrameTime * 0.5;
			EX.ClampValue(Volume, 0.0, CLAMP_LESS);
			SndChannel1->setVolume(Volume);
			SndChannel2->setVolume(Volume);
		}
	}

	void RenderFunc() {
		Txt.PixelText.Reset();
		Txt.PixelText.SetAlign(ALIGN_MIDDLE);
		Txt.PixelText.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Txt.PixelText.SetColor(1.0, 1.0, 1.0);
		Txt.PixelText.Rotate(-10.0);

		if (Glb.Ending == TimeOut || Glb.Ending == HitXion || Glb.Ending == Suppress) {
			// 배경 렌더링
			Begin();
			transform.Scale(MoveMatrix, 3.0, 3.0);
			imageUtil.Render(Img.GameOverBackGround, 0.7);

			// 시온 렌더링
			Begin();
			transform.Move(MoveMatrix, ASP(-1.0) + 0.5 + ShiverValue.x, -0.3 + ShiverValue.y);
			transform.Scale(MoveMatrix, 1.5, 1.5);

			if(Glb.Ending == HitXion)
				imageUtil.RenderStaticSpriteSheet(Img.Xion, Hurt);
			else if(Glb.Ending == TimeOut)
				imageUtil.RenderStaticSpriteSheet(Img.Xion, Cry1);

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

			// 텍스트 렌더링
			if (Glb.Ending == HitXion) {
				Txt.PixelText.SetColor(0.0, 0.0, 0.0);
				Txt.PixelText.SetOpacity(0.8);
				Txt.PixelText.Render(ASP(-1.0) + 0.8 + (0.2 + TextSize) * 0.1, 0.75 - (0.2 + TextSize) * 0.1, 0.2 + TextSize, L"진짜 유죄!!!");
				Txt.PixelText.Render(ASP(-1.0) + 0.8 + (0.1 + TextSize) * 0.1, 0.55 - (0.1 + TextSize) * 0.1, 0.1 + TextSize, L"언니를 걷어 차?!!");

				Txt.PixelText.SetColor(1.0, 1.0, 1.0);
				Txt.PixelText.SetOpacity(1.0);
				Txt.PixelText.Render(ASP(-1.0) + 0.8, 0.75, 0.2 + TextSize, L"진짜 유죄!!!");
				Txt.PixelText.Render(ASP(-1.0) + 0.8, 0.55, 0.1 + TextSize, L"언니를 걷어 차?!!");
			}

			else if (Glb.Ending == TimeOut) {
				Txt.PixelText.SetColor(0.0, 0.0, 0.0);
				Txt.PixelText.SetOpacity(0.8);
				Txt.PixelText.Render(ASP(-1.0) + 0.8 + (0.2 + TextSize) * 0.1, 0.75 - (0.2 + TextSize) * 0.1, 0.2 + TextSize, L"유죄!!!");
				Txt.PixelText.Render(ASP(-1.0) + 0.8 + (0.1 + TextSize) * 0.1, 0.55 - (0.1 + TextSize) * 0.1, 0.1 + TextSize, L"미쳤어?!!");

				Txt.PixelText.SetColor(1.0, 1.0, 1.0);
				Txt.PixelText.SetOpacity(1.0);
				Txt.PixelText.Render(ASP(-1.0) + 0.8, 0.75, 0.2 + TextSize, L"유죄!!!");
				Txt.PixelText.Render(ASP(-1.0) + 0.8, 0.55, 0.1 + TextSize, L"미쳤어?!!");
			}

			else if (Glb.Ending == Suppress) {
				Txt.PixelText.SetColor(0.0, 0.0, 0.0);
				Txt.PixelText.SetOpacity(0.8);
				Txt.PixelText.Render(ASP(-1.0) + 0.8 + (0.2 + TextSize) * 0.1, 0.75 - (0.2 + TextSize) * 0.1, 0.2 + TextSize, L"무죄!!!");
				Txt.PixelText.Render(ASP(-1.0) + 0.8 + (0.1 + TextSize) * 0.1, 0.55 - (0.1 + TextSize) * 0.1, 0.1 + TextSize, L"이드! 해냈구나!");

				Txt.PixelText.SetColor(1.0, 1.0, 1.0);
				Txt.PixelText.SetOpacity(1.0);
				Txt.PixelText.Render(ASP(-1.0) + 0.8, 0.75, 0.2 + TextSize, L"무죄!!!");
				Txt.PixelText.Render(ASP(-1.0) + 0.8, 0.55, 0.1 + TextSize, L"이드! 해냈구나!");
			}
		}
	}
};