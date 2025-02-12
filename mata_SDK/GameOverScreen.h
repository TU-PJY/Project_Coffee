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
	// Ŭ�ο��� ���� ������ ���� Ÿ�̸�
	TimerUtil Timer{};

	// Ŭ�ο��� ���� ������ ����
	GLfloat CheekInterval{};

	// �ÿ��� ���� ���� Ÿ�̸�
	TimerUtil ShiverTimer{};

	// ���� �ִϸ��̼� ��ġ
	glm::vec2 ShiverValue{};
	glm::vec2 ShiverValue2{};

	// ���� ũ�� ��ġ
	GLfloat VerticalSize{};

	// �ؽ�Ʈ ũ��
	GLfloat TextSize{};

	// �ؽ�Ʈ ũ�� ����
	SinLoop TextSizeLoop{};

	// ������
	int ChloeFrame{};
	int EDFrame{};

	// �̵� ������ Ÿ�̸�
	TimerUtil EDFrameTimer{};

	// Ȩ��� ��ȯ ����
	bool ExitState{};

	std::wstring Str{};
	std::wstring Str2{};

	TextUtil Text{}, ScoreText{};

	// ���� 
	///////
	// ���� ǥ�õǴ� ����
	bool ShowRep{};

	// �� ���� ǥ�õǴ� ����
	bool ShowTotalRep{};
	 
	// ��� ǰ���� �����ϴ� ����
	std::vector <Reparations> RepVec{};
	size_t VecSize{};
	int CurrentSize{};

	// �� ����
	int TotalRep{};

	// ���ݾ��� ���������� �����ֱ� ���� Ÿ�̸�
	TimerUtil RepTimer{};

	// �� ���ݾ� �ؽ�Ʈ ��鸲 ��
	glm::vec2 TextShake{};
	GLfloat TextShakeValue{0.05};
	TimerUtil TextShakeTimer{};

	SoundChannel SndChannel1{};
	SoundChannel SndChannel2{};
	SoundChannel SndChannel3{};
	GLfloat Volume = 1.0;


	// Ÿ��Ʋ�� ������ ���� Ÿ�̸�
	TimerUtil DeleteTimer{};

public:
	GameOverScreen() {
		System.SetBackColorRGB(74, 78, 101);

		Text.Init(L"�ȼ��κ��κ�ü", FW_DONTCARE);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.SetUseShadow(true);
		Text.SetShadow(0.1, glm::vec3(0.0, 0.0, 0.0), 0.7);
		Text.SetColor(1.0, 1.0, 1.0);
		Text.Rotate(-10.0);

		ScoreText.Init(L"�ȼ��κ��κ�ü", FW_DONTCARE);
		ScoreText.SetAlign(ALIGN_MIDDLE);
		ScoreText.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		ScoreText.SetUseShadow(true);
		ScoreText.SetShadow(0.1, glm::vec3(0.0, 0.0, 0.0), 0.7);
		ScoreText.SetColor(1.0, 1.0, 1.0);
		ScoreText.SetLineGap(0.02);

		if (Glb.Ending == GameOver_TimeOut) {
			CheekInterval = 0.7;
			Str = L"����!!!";
			Str2 = L"���ƾ�?!!";
		}

		else if (Glb.Ending == GameOver_HitXion) {
			CheekInterval = 0.25;
			Str = L"��¥ ����!!!";
			Str2 = L"��ϸ� �Ⱦ� ��?!!";
		}

		else if (Glb.Ending == GameOver_Suppressed) {
			Str = L"����!!!";
			Str2 = L"�̵�! �س±���!";
		}

		soundUtil.Play(Snd.GameEnd[Glb.Ending], SndChannel1);

		// �ı��� ǰ���� �����ϸ� ���Ϳ� �����Ѵ�
		for (int i = 0; i < 6; i++) {
			Reparations Rep{};
			if (Glb.DestroyedItems[i] > 0) {
				Rep.DestroyedType = i;
				Rep.NumDestroyed = Glb.DestroyedItems[i];
				RepVec.emplace_back(Rep);
			}
		}

		VecSize = RepVec.size();

		// �� ���� ���
		TotalRep += Glb.DestroyedItems[Item_BoxCoffee] * 25000;
		TotalRep += Glb.DestroyedItems[Item_BottleCoffee] * 2500;
		TotalRep += Glb.DestroyedItems[Item_CanCoffee] * 1200;
		TotalRep += Glb.DestroyedItems[Item_People] * 1000000;
		TotalRep += Glb.DestroyedItems[Item_Xion] * 1000000;
		TotalRep += Glb.DestroyedItems[Item_HouseDoc] * 100000;
	}

	void InputKey(KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN && Event.NormalKey == NK_ENTER) {
			// ù ���� �Է��� �� ���� ȭ������ ��ȯ�ϰ�, �� ��° �Է��� Ȩ���� ��ȯ�Ѵ�
			if (!ShowRep) {
				ShowRep = true;
				ScoreText.SetUseShadow(false);
			}
			else {
				ExitState = true;
				scene.AddObject(new Cover, "cover", LAYER7);
				// ������ ���¸� Ȱ��ȭ�ϰ� �� �̻� �Է��� ���� �ʵ��� �Ѵ�
				scene.DeleteInputObject(this);
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		if (Glb.Ending == GameOver_TimeOut || Glb.Ending == GameOver_HitXion) {
			Timer.Update(FrameTime);
			if (Timer.CheckMiliSec(CheekInterval, 2, CHECK_AND_INTERPOLATE)) {
				ChloeFrame++;
				
				// ���� �������� Ŭ�ο��� �̵��� ���� ������
				if (ChloeFrame == 1) {
					EDFrame = 1;
					EDFrameTimer.Reset();
					VerticalSize = 0.3;
					soundUtil.Stop(SndChannel2);
					soundUtil.Play(Snd.HitCheek, SndChannel2);
				}
				EX.ClampValue(ChloeFrame, 0, 1, CLAMP_RETURN);
			}

			// ���� �ִϸ��̼�
			ShiverTimer.Update(FrameTime);
			if (ShiverTimer.CheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE)) {
				ShiverValue.x = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				ShiverValue.y = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				ShiverValue2.x = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				ShiverValue2.y = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
			}

			// ���� ũ�� �ִϸ��̼�
			mathUtil.Lerp(VerticalSize, 0.0, 10.0, FrameTime);

			// �̵�� ���� ���� �� ���� �Ŀ� �ٽ� ���� ���������� �����Ѵ�
			if (EDFrame == 1) {
				EDFrameTimer.Update(FrameTime);
				if (EDFrameTimer.CheckMiliSec(CheekInterval * 0.5, 2, CHECK_AND_RESET))
					EDFrame = 0;
			}

			// �ؽ�Ʈ �ִϸ��̼�
			TextSize = TextSizeLoop.Update(0.01, 6.0, FrameTime);
		}

		// �� ������ ���������� �����ش�
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

			// ���� �Ѿ��� ��鸮�� �ִϸ��̼ǰ� �Բ� �����ش�
			if (ShowTotalRep) {
				TextShakeTimer.Update(FrameTime);
				if (TextShakeTimer.CheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE)) {
					TextShake.x = randomUtil.Gen(RANDOM_TYPE_REAL, -TextShakeValue, TextShakeValue);
					TextShake.y = randomUtil.Gen(RANDOM_TYPE_REAL, -TextShakeValue, TextShakeValue);
				}

				mathUtil.Lerp(TextShakeValue, 0.0, 5.0, FrameTime);
			}
		}

		// ȭ���� ��ο����鼭 Ȩ���� ��ȯ�Ѵ�
		// ������ �ε巴�� ���δ�
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
		// �������� �ÿ��� �������� �޶�����
		if (Glb.Ending == GameOver_TimeOut || Glb.Ending == GameOver_HitXion || Glb.Ending == GameOver_Suppressed) {
			// ��� ������
			Begin();
			transform.Scale(MoveMatrix, 3.0, 3.0);
			imageUtil.Render(Img.GameOverBackGround, 0.7);

			// �ÿ� ������
			Begin();
			transform.Move(MoveMatrix, ASP(-1.0) + 0.5 + ShiverValue.x, -0.3 + ShiverValue.y);
			transform.Scale(MoveMatrix, 1.5, 1.5);

			if(Glb.Ending == GameOver_HitXion)
				imageUtil.RenderStaticSpriteSheet(Img.Xion, Xion_Hurt);
			else if(Glb.Ending == GameOver_TimeOut)
				imageUtil.RenderStaticSpriteSheet(Img.Xion, Xion_Cry1);

			// �̵� ������
			Begin();
			transform.Move(MoveMatrix, ASP(1.0) - 1.03 + ShiverValue2.x - VerticalSize * 0.5, -0.7 - VerticalSize * 0.5 + ShiverValue2.y);
			transform.Scale(MoveMatrix, 2.0 + VerticalSize, 2.0 - VerticalSize);
			transform.Shear(MoveMatrix, -VerticalSize * 0.5, 0.0);
			imageUtil.RenderStaticSpriteSheet(Img.ED_GameOver, EDFrame);

			// Ŭ�ο� ������
			Begin();
			transform.Move(MoveMatrix, ASP(1.0) - 0.5 + ShiverValue2.x - VerticalSize * 0.5, -0.6 - VerticalSize * 0.5 + ShiverValue2.y);
			transform.Scale(MoveMatrix, 2.2 + VerticalSize, 2.2 - VerticalSize);
			transform.Shear(MoveMatrix, -VerticalSize * 0.5, 0.0);
			imageUtil.RenderStaticSpriteSheet(Img.Chloe, ChloeFrame);
			
			if (!ShowRep) {
				// �ؽ�Ʈ ���
				Text.Render(ASP(-1.0) + 0.8, 0.85, 0.2 + TextSize, Str.c_str());
				Text.Render(ASP(-1.0) + 0.8, 0.6, 0.1 + TextSize, Str2.c_str());

				// ���� ���
				ScoreText.Render(0.0, 0.5, 0.15, L"SCORE: %d", Glb.Score);
			}

			// ���� ���
			else {
				// �� ��� ���
				Begin();
				transform.Scale(MoveMatrix, ASP(2.0), 2.0);
				imageUtil.Render(SysRes.COLOR_TEXTURE, 0.6);

				// ���� ���
				GLfloat RenderHeight{0.8};

				for (int i = 0; i < CurrentSize; i++) {
					ScoreText.SetColor(1.0, 1.0, 1.0);

					switch (RepVec[i].DestroyedType) {
					case Item_BoxCoffee:
						ScoreText.Render(0.0, RenderHeight, 0.1, L"Ŀ�� ���� �ļ�: %d�� x 25000���", Glb.DestroyedItems[RepVec[i].DestroyedType]);
						break;

					case Item_BottleCoffee:
						ScoreText.Render(0.0, RenderHeight, 0.1, L"�� Ŀ�� �ļ�: %d�� x 2500���", Glb.DestroyedItems[RepVec[i].DestroyedType]);
						break;

					case Item_CanCoffee:
						ScoreText.Render(0.0, RenderHeight, 0.1, L"ĵ Ŀ�� �ļ�: %d�� x 1200���", Glb.DestroyedItems[RepVec[i].DestroyedType]);
						break;

					case Item_People:
						ScoreText.Render(0.0, RenderHeight, 0.1, L"Ÿ�� ����: %d�� x 1000000���", Glb.DestroyedItems[RepVec[i].DestroyedType]);
						break;

					case Item_Xion:
						ScoreText.Render(0.0, RenderHeight, 0.1, L"�ÿ� ġ���: 1000000���");
						break;

					case Item_HouseDoc:
						ScoreText.Render(0.0, RenderHeight, 0.1, L"�� ź ������: %d�� x 100000���", Glb.DestroyedItems[RepVec[i].DestroyedType]);
						break;
					}

					RenderHeight -= 0.2;
				}

				if (ShowTotalRep) {
					ScoreText.SetColorRGB(255, 213, 80);
					ScoreText.Render(0.0 + TextShake.x, -0.7 + TextShake.y, 0.15, L"�� ����: %d ���", TotalRep);
				}
			}
		}
	}
};