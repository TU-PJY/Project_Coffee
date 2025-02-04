#pragma once
#include "GameObject.h"

class IntroScreen : public GameObject {
private:
	TimerUtil    timer;
	SoundChannel IntroChannel{};

	GLfloat      LogoSize        = 1.0;
	GLfloat      LogoOpacity     = 0.0;
	GLfloat      LogoPosition    = -0.3;
	GLfloat      AnimationSpeed  = 0.5;

	SinLerp      Slerp{};

	int          SceneNumber{}; 

public:
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
		switch (SceneNumber) {
		case 0:
			if (timer.CheckMiliSec(1.0, 1, CHECK_AND_INTERPOLATE)) {
				soundUtil.Play(SysRes.INTRO_SOUND, IntroChannel);
				++SceneNumber;
			}
			break;
			

		case 1:
			if (timer.MiliSec() < 2.5) {
				LogoPosition = Slerp.Update(LogoPosition, 0.0, AnimationSpeed, FrameTime);
				LogoOpacity = Slerp.Update(LogoOpacity, 1.0, AnimationSpeed, FrameTime);
			}

			if (timer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
				LogoOpacity -= FrameTime * 2;
				EX.ClampValue(LogoOpacity, 0.0, CLAMP_LESS);
			}

			if (timer.CheckMiliSec(4.0, 1, CHECK_AND_INTERPOLATE)) {
				LogoPosition = -0.3;
				Slerp.Reset();
				++SceneNumber;
			}
			break;


		case 2:
			if (timer.MiliSec() < 2.5) {
				LogoPosition = Slerp.Update(LogoPosition, 0.0, AnimationSpeed, FrameTime);
				LogoOpacity = Slerp.Update(LogoOpacity, 1.0, AnimationSpeed, FrameTime);
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

	void RenderFunc() {
		Begin(RENDER_TYPE_STATIC);
		transform.Move(MoveMatrix, 0.0, LogoPosition);

		switch (SceneNumber) {
		case 1:
			imageUtil.Render(SysRes.SDK_LOGO, LogoOpacity);
			break;

		case 2: case 3:
			imageUtil.Render(SysRes.FMOD_LOGO, LogoOpacity);
			break;
		}
	}
};