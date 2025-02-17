#pragma once
#include <Scene.h>

#include "TitleMode.h"
#include "GameOverMode.h"
#include "PauseMode.h"

#include "Cover.h"

class PlayModeManager : public GameObject {
private:
	TimerUtil Timer1{}, Timer2{};
	bool GameStart{};

	// Ÿ��Ʋ ȭ������ ���ư���
	bool GoToTitle{};
	bool CoverAdded{};

public:
	// �Ͻ����� ���� ��ȯ
	void InputKey(KeyEvent& Event) {
		if (GameStart && Event.Type == NORMAL_KEY_DOWN && Event.NormalKey == NK_ESCAPE)
			scene.StartFloatingMode(PauseMode.Start, true);
	}

	void UpdateFunc(float FrameTime) {
		// 3�� �� ȭ���� ��ο�����
		if (Glb.GameOver) {
			Timer1.Update(FrameTime);
			if (Timer1.Sec() >= 3) {
				scene.AddObject(new Cover, "cover", LAYER7);
				Timer1.Reset();
				Timer1.Stop();
			}

			//  ȭ���� ������ ��ο��� �� 1�� �� ���ӿ��� ��ũ������ ��ȯ�Ѵ�
			if (auto Cover = scene.Find("cover"); Cover) {
				if (Cover->GetState()) {
					Timer2.Update(FrameTime);

					if (Timer2.Sec() >= 1) {
						// ���ӿ��� ���� ��ȯ
						scene.SwitchMode(GameOverMode.Start);
					}
				}
			}
		}

		else if (GoToTitle) {
			if (!CoverAdded) {
				scene.AddObject(new Cover, "cover", LAYER7);
				CoverAdded = true;
			}

			//  ȭ���� ������ ��ο��� �� 1�� �� Ÿ��Ʋ ȭ������ ��ȯ�Ѵ�
			if (auto Cover = scene.Find("cover"); Cover) {
				if (Cover->GetState()) {
					Timer2.Update(FrameTime);

					if (Timer2.Sec() >= 1) {
						// Ÿ��Ʋ ���� ��ȯ
						scene.SwitchMode(TitleMode.Start);
					}
				}
			}
		}
	}

	// Bgm ���
	void PlayBGM() {
		int RandomIndex = randomUtil.Gen(RANDOM_TYPE_INT, 0, 2);
		soundUtil.Play(Snd.PlayBgm[RandomIndex], Glb.BGMChannel);
		Glb.BGMChannel->setVolume(Glb.BGMVolume);
		GameStart = true;
	}

	// Bgm ����
	void StopBGM() {
		soundUtil.Stop(Glb.BGMChannel);
		GameStart = false;
	}

	void SetGoToTitle() {
		GoToTitle = true;
	}
};