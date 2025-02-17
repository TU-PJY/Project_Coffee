#pragma once
#include <Scene.h>

class Tutorial : public GameObject {
private:
	TimerUtil StartTimer{};
	bool GameStart{};

	SoundChannel SndChannel{};

public:
	Tutorial() {
		// �� �� Ʃ�丮���� �а� ���� ������ �ʱ�ȭ �� ���� �������� �ʴ´�
		Glb.NeedTutorial = false;
		Dat.HighscoreData.UpdateDigitData("TutorialNeed", "Bool", 0);
	}

	void InputKey(KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN && Event.NormalKey == NK_ENTER) {
			scene.DeleteInputObject(this);
			GameStart = true;

			soundUtil.Play(Snd.CartCrash, SndChannel);
			SndChannel->setVolume(Glb.SFXVolume);
		}
	}

	void UpdateFunc(float FrameTime) {
		if (GameStart) {
			StartTimer.Update(FrameTime);
			if (StartTimer.Sec() >= 1) {
				cameraControl.ChangeCameraZoom(1.0);
				scene.SwitchMode(PlayModePtr);
			}
		}
	}

	void RenderFunc() {
		Begin(RENDER_TYPE_STATIC);
		transform.Scale(MoveMatrix, ASP(2.0), 2.0);
		imageUtil.Render(SysRes.COLOR_TEXTURE);

		if (!GameStart) {
			Begin(RENDER_TYPE_STATIC);
			GLfloat SizeX = 2.0 * ASPECT;
			GLfloat SizeY = 2.0 * ASPECT; 
			GLfloat ImgAspect = 2560.0 / 1440.0;

			if (SizeY >= 2.0 * ImgAspect) {
				SizeY = 2.0 * ImgAspect;
				SizeX = 2.0 * ImgAspect;
			}

			transform.Scale(MoveMatrix, SizeX, SizeY);
			imageUtil.Render(Img.Tutorial);
		}
	}
};