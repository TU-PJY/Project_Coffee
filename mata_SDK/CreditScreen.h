#pragma once
#include <Scene.h>

#include "TitleMode.h"

class CreditScreen : public GameObject {
private:
	GLfloat CreditZoom{ 2.2 };
	GLfloat CreditHeight{ 0.3 };
	GLfloat CreditPosition{};

	GLfloat EDPosition = -0.35;
	int EDFrame{};

	GLfloat XionPosition = 0.4;

	TimerUtil EDTimer{};
	GLfloat BlinkInterval{};

	SinLoop EDLoop{};
	SinLoop XionLoop{};
	SinLoop LightLoop{};
	GLfloat LightOpacity{};

	GLfloat EDSize{};
	GLfloat XionSize{};

	TextUtil Text{};

	std::vector<std::wstring> CreditStr = {
		L"- Original Game by -",
		L"- A Computer game by -",
		L"- Programming & Art -",
		L"- Music -",
		L"- Effect sound -",
		L"- Made using FMOD API by -",
		L"",
		L"플레이 해주셔서 감사합니다!"
	};

	std::vector<std::wstring> Compositor = {
		L"EPID Games",
		L"mata_",
		L"mata_",
		L"Zane Little\nContext Sensitive\nHolizna",
		L"Context Sensitive\nPixabay",
		L"Firelight Technologies Pty Ltd.",
		L"",
		L"",
	};

	bool StartTextRender{};
	bool TextRenderState{};
	TimerUtil CreditTimer{};
	int CurrentIndex{};

	TimerUtil ExitEDTimer{};
	bool EDExitState{};
	bool ExitState{};

	GLfloat Volume = Glb.BGMVolume;

	SoundChannel SndChannel{};

public:
	CreditScreen() {
		Text.Init(L"픽셀로보로보체", FW_DONTCARE);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.SetLineGap(0.01);

		soundUtil.Play(Snd.CreditBGM, Glb.BGMChannel);
		Glb.BGMChannel->setVolume(Glb.BGMVolume);
	}

	void InputKey(KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN && Event.NormalKey == NK_ENTER) {
			EDExitState = true;
			scene.DeleteInputObject(this);
		}
	}

	void UpdateFunc(float FrameTime) {
		EDSize = EDLoop.Update(0.01, 4.0, FrameTime);
		XionSize = XionLoop.Update(0.03, 2.0, FrameTime);
		LightOpacity = LightLoop.Update(0.2, 20.0, FrameTime);

		
		if (!EDExitState) {
			EDTimer.Update(FrameTime);
			if (EDTimer.CheckMiliSec(BlinkInterval, 1, CHECK_AND_INTERPOLATE)) {
				if (EDFrame == 0) {
					BlinkInterval = 0.2;
					EDFrame = 1;
				}
				else {
					BlinkInterval = 2.0;
					EDFrame = 0;
				}
			}
		}

		else {
			EDFrame = 0;
		}


		CreditTimer.Update(FrameTime);
		if (!StartTextRender && CreditTimer.CheckMiliSec(11.8, 1, CHECK_AND_INTERPOLATE)) {
			StartTextRender = true;
			TextRenderState = true;
		}

		else if (StartTextRender && !TextRenderState && CreditTimer.CheckMiliSec(0.73, 2, CHECK_AND_INTERPOLATE)) 
			TextRenderState = true;

		else if (CurrentIndex < CreditStr.size() - 1 && StartTextRender && TextRenderState && CreditTimer.CheckMiliSec(5.12, 2, CHECK_AND_INTERPOLATE)) {
			TextRenderState = false;
			CurrentIndex++;
		}

		if (!EDExitState) {
			CreditZoom -= 1.4 * 0.1 * FrameTime;
			CreditHeight -= 0.2 * 0.1 * FrameTime;
			EX.ClampValue(CreditZoom, 0.8, CLAMP_LESS);
			EX.ClampValue(CreditHeight, 0.1, CLAMP_LESS);
		}

		if (EDExitState) {
			Volume -= FrameTime;
			EX.ClampValue(Volume, 0.0, CLAMP_LESS);
			Glb.BGMChannel->setVolume(Volume);

			mathUtil.Lerp(CreditPosition, EDPosition, 3.0, FrameTime);
			mathUtil.Lerp(CreditHeight, 0.2, 3.0, FrameTime);
			mathUtil.Lerp(CreditZoom, 1.8, 3.0, FrameTime);

			ExitEDTimer.Update(FrameTime);
			if (ExitEDTimer.CheckSec(1, CHECK_AND_RESUME))
				EDFrame = 2;

			if (ExitEDTimer.CheckSec(2, CHECK_AND_RESUME)) {
				if (!ExitState) {
					soundUtil.Play(Snd.ShutDown, SndChannel);
					SndChannel->setVolume(Glb.SFXVolume);
				}
				ExitState = true;
			}

			if (ExitEDTimer.CheckSec(4, CHECK_AND_RESUME)) {
				Glb.TitleIntroPlayed = false;
				scene.SwitchMode(TitleMode.Start);
			}
		}

		cameraControl.ChangeCameraZoom(CreditZoom);
		cameraControl.MoveCamera(CreditPosition, CreditHeight);
	}

	void RenderFunc() {
		if (ExitState) {
			Begin(RENDER_TYPE_STATIC);
			SetColor(0.0, 0.0, 0.0);
			transform.Scale(MoveMatrix, ASP(2.0), 2.0);
			imageUtil.Render(SysRes.COLOR_TEXTURE);
		}

		else {
			// 바닥
			Begin();
			SetColorRGB(67, 76, 99);
			transform.Move(MoveMatrix, CreditPosition, -1.5);
			transform.Scale(MoveMatrix, ASP(2.0) / camera.ZoomValue, 2.0);
			imageUtil.Render(SysRes.COLOR_TEXTURE);

			// 소파 뒷 부분
			Begin();
			transform.Scale(MoveMatrix, 4.0, 4.0);
			imageUtil.RenderStaticSpriteSheet(Img.Sofa, 0);

			// 시온 몸통
			Begin();
			transform.Move(MoveMatrix, XionPosition, XionSize * 0.5);
			transform.Scale(MoveMatrix, 2.0, 2.0 + XionSize);
			imageUtil.RenderStaticSpriteSheet(Img.CreditXion, 0);

			// 이드 몸통
			Begin();
			transform.Move(MoveMatrix, EDPosition, EDSize * 0.5);
			transform.Scale(MoveMatrix, 2.0, 2.0 + EDSize);
			imageUtil.RenderStaticSpriteSheet(Img.CreditED, EDFrame);

			// 소파 쿠션 부분
			Begin();
			transform.Scale(MoveMatrix, 4.0, 4.0);
			imageUtil.RenderStaticSpriteSheet(Img.Sofa, 2);
			imageUtil.RenderStaticSpriteSheet(Img.Sofa, 1);

			// 시온 다리
			Begin();
			transform.Move(MoveMatrix, XionPosition, XionSize * 0.5);
			transform.Scale(MoveMatrix, 2.0, 2.0 + XionSize);
			imageUtil.RenderStaticSpriteSheet(Img.CreditXion, 1);

			// 의자
			Begin();
			transform.Move(MoveMatrix, XionPosition, 0.0);
			transform.Scale(MoveMatrix, 2.0, 2.0);
			imageUtil.Render(Img.Chair);

			// 이드 다리
			Begin();
			transform.Move(MoveMatrix, EDPosition, EDSize * 0.5);
			transform.Scale(MoveMatrix, 2.0, 2.0 + EDSize);
			imageUtil.RenderStaticSpriteSheet(Img.CreditED, 3);

			// TV 불빛
			Begin();
			SetColor(1.0, 1.0, 1.0);
			transform.Move(MoveMatrix, 0.0, -1.4);
			transform.Scale(MoveMatrix, 4.0, 4.0);
			imageUtil.Render(Img.TVLight, 0.5 - LightOpacity);

			SetColor(0.0, 0.0, 0.0);
			imageUtil.Render(Img.TV);

			// 텍스트
			if (!EDExitState) {
				Text.SetAlign(ALIGN_MIDDLE);
				Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
				if (TextRenderState) {
					Text.SetColorRGB(255, 213, 80);
					if (CurrentIndex == 7)
						Text.Render(0.0, 0.75, 0.15, CreditStr[CurrentIndex].c_str());
					else
						Text.Render(0.0, 0.95, 0.15, CreditStr[CurrentIndex].c_str());

					Text.SetColor(1.0, 1.0, 1.0);
					Text.Render(0.0, 0.8, 0.08, Compositor[CurrentIndex].c_str());

					if (CurrentIndex == 5) {
						Begin(RENDER_TYPE_STATIC);
						transform.Move(MoveMatrix, 0.0, 0.65);
						transform.Scale(MoveMatrix, 0.5, 0.5);
						imageUtil.Render(SysRes.FMOD_LOGO);
					}

					else if (CurrentIndex == 6) {
						Begin(RENDER_TYPE_STATIC);
						transform.Move(MoveMatrix, 0.0, 0.75);
						transform.Scale(MoveMatrix, 0.7, 0.7);
						imageUtil.Render(SysRes.SDK_LOGO);
					}

				}
				Text.SetAlign(ALIGN_LEFT);
				Text.SetHeightAlign(HEIGHT_ALIGN_DEFAULT);
				Text.SetColorRGB(255, 213, 80);
				Text.Render(ASP(1.0) - 0.05, -0.95, 0.1, L"Enter를 눌러 계속");
			}
		}
	}
};