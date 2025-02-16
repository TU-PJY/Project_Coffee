#pragma once
#include <Scene.h>

class CreditScreen : public GameObject {
private:
	TextUtil Text{};
	GLfloat CreditZoom{ 2.2 };
	GLfloat CreditHeight{ 0.3 };

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

public:
	CreditScreen() {
		Text.Init(L"픽셀로보로보체", FW_DONTCARE);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.SetUseShadow(true);
		Text.SetShadow(0.1, glm::vec3(0.0, 0.0, 0.0), 0.8);
	}

	void UpdateFunc(float FrameTime) {
		EDSize = EDLoop.Update(0.01, 4.0, FrameTime);
		XionSize = XionLoop.Update(0.03, 2.0, FrameTime);
		LightOpacity = LightLoop.Update(0.2, 20.0, FrameTime);

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

		CreditZoom -= 1.4 * 0.1 * FrameTime;
		CreditHeight -= 0.3 * 0.1 * FrameTime;
		EX.ClampValue(CreditZoom, 0.8, CLAMP_LESS);
		EX.ClampValue(CreditHeight, 0.0, CLAMP_LESS);
		cameraControl.ChangeCameraZoom(CreditZoom);
		cameraControl.MoveCamera(0.0, CreditHeight);
	}

	void RenderFunc() {
		// 바닥
		Begin();
		SetColorRGB(67, 76, 99);
		transform.Move(MoveMatrix, 0.0, -1.5);
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
	}
};