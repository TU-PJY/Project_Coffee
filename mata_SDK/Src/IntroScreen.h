#pragma once
#include "GameObject.h"

class IntroScreen : public GameObject {
private:
	TimerUtil    timer;
	SoundChannel IntroChannel{};

	GLfloat      LogoSize        = 1.0;
	GLfloat      LogoOpacity     = 0.0;
	GLfloat      LogoPosition    = -0.3;
	GLfloat      AnimationSpeed  = 3.0;

	int          SceneNumber{}; 

	TextUtil Text{};

	bool ScenePlay{};

public:
	IntroScreen() {
		Text.Init(L"픽셀로보로보체", FW_DONTCARE);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.SetFixMiddle(true);
		Text.SetColor(1.0, 1.0, 1.0);
		Text.SetLineGap(0.02);
	}

	void InputKey(KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN) {
			switch (Event.NormalKey) {
			case NK_ENTER:
				soundUtil.Stop(IntroChannel);
				scene.SwitchMode(START_MODE);
				break;

#ifdef ENABLE_DEV_EXIT
			case NK_ESCAPE:
				System.Exit();
				break;
#endif
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		timer.Update(FrameTime);
		if (!ScenePlay && timer.CheckSec(10, CHECK_AND_RESET))
			ScenePlay = true;

		if (ScenePlay) {
			switch (SceneNumber) {
			case 0:
				if (timer.CheckMiliSec(1.0, 1, CHECK_AND_INTERPOLATE)) {
					soundUtil.Play(SysRes.INTRO_SOUND, IntroChannel);
					++SceneNumber;
				}
				break;


			case 1:
				if (timer.MiliSec() < 2.5) {
					mathUtil.Lerp(LogoPosition, 0.0, AnimationSpeed, FrameTime);
					mathUtil.Lerp(LogoOpacity, 1.0, AnimationSpeed, FrameTime);
				}

				if (timer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
					LogoOpacity -= FrameTime * 2;
					EX.ClampValue(LogoOpacity, 0.0, CLAMP_LESS);
				}

				if (timer.CheckMiliSec(4.0, 1, CHECK_AND_INTERPOLATE)) {
					LogoPosition = -0.3;
					++SceneNumber;
				}
				break;


			case 2:
				if (timer.MiliSec() < 2.5) {
					mathUtil.Lerp(LogoPosition, 0.0, AnimationSpeed, FrameTime);
					mathUtil.Lerp(LogoOpacity, 1.0, AnimationSpeed, FrameTime);
				}

				if (timer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
					LogoOpacity -= FrameTime * 2;
					EX.ClampValue(LogoOpacity, 0.0, CLAMP_LESS);
				}

				if (timer.CheckMiliSec(4.0, 1, CHECK_AND_RESUME))
					scene.SwitchMode(START_MODE);

				break;
			}
		}
	}

	void RenderFunc() {
		if (!ScenePlay) {
			Text.Render(0.0, 0.0, 0.1, L"본 게임은 '트릭컬:리바이브' 팬게임으로, \n해당 게임의 캐릭터에 대한 모든 권한은\nEPIDGames가 소유함을 알립니다.\n\n(C)2024 EPIDGames. All Rights Reserved.");
		}

		else {
			Begin(RENDER_TYPE_STATIC);
			transform.Move(MoveMatrix, 0.0, LogoPosition);

			switch (SceneNumber) {
			case 1:
				imageUtil.Render(Img.mataLogo, LogoOpacity);
				break;

			case 2: case 3:
				imageUtil.Render(SysRes.FMOD_LOGO, LogoOpacity);
				break;
			}
		}
	}
};