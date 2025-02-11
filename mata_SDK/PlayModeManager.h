#pragma once
#include <Scene.h>
#include "Cover.h"
#include "GameOverMode.h"

class PlayModeManager : public GameObject {
private:
	TimerUtil Timer1{}, Timer2{};

	void UpdateFunc(float FrameTime) {
		// 3�� �� ȭ���� ��ο�����
		if (Glb.GameOver) {
			Timer1.Update(FrameTime);
			if (Timer1.Sec() >= 4) {
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
	}
};