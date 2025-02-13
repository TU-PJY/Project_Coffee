#pragma once
#include <Scene.h>

class TitleScreen : public GameObject {
private:
	// ��Ʈ�� �� ����
	bool IntroEnd{};

	bool RenderText{};

	// �޴� �ؽ�Ʈ �� ���� ���õ� �ؽ�Ʈ
	TextUtil Text{};
	bool MenuFocused[3]{};
	int MenuIndex{};

	bool QuestionToDesktop{};
	bool QuestionFocused[2]{};

	bool SettingState{};
	bool SettingFocused[5]{};
	int MenuIndex2{};

	TimerUtil StartTimer{};
	bool GameStart{};

	std::wstring MenuItems[3] = {
		L"����!",
		L"ȯ�漳��",
		L"����ȭ������\n������"
	};

	std::wstring QuestionItems[2] = {
		L"��",
		L"�ƴϿ�"
	};

	GLfloat TextOpacity{};

	SoundChannel SndChannel{};

	TimerUtil MoveDelayTimer{};
	GLfloat TitleCameraPosition = -1.3;
	GLfloat TitleCameraZoom = 2.0;

	SinLoop XionLoop{};
	SinLoop EDLoop{};
	GLfloat XionSize{};
	GLfloat EDSize{};
	TimerUtil ShakeTimer{};
	glm::vec2 ShakePosition{};

public:
	TitleScreen(bool IntroFlag) {
		if (IntroFlag) {
			IntroEnd = true;
			RenderText = true;
			TextOpacity = 1.0;
		}
		else
			IntroEnd = false;

		MenuFocused[0] = true;
		SettingFocused[0] = true;
		QuestionFocused[0] = true;

		Text.Init(L"�ȼ��κ��κ�ü", FW_DONTCARE);
		Text.SetAlign(ALIGN_LEFT);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.SetShadow(0.1, glm::vec3(0.0, 0.0, 0.0), 0.8);

		soundUtil.Play(Snd.TitleBgm, Glb.BGMChannel);
	}

	void InputKey(KeyEvent& Event) {
		if (!RenderText)
			return;

		if (Event.Type == SPECIAL_KEY_DOWN) {
			switch (Event.SpecialKey) {
			case SK_ARROW_UP:
				soundUtil.Stop(SndChannel);
				soundUtil.Play(Snd.MenuSelect, SndChannel);

				if (!SettingState && !QuestionToDesktop)
					MenuIndex--;
				else if(SettingState || QuestionToDesktop)
					MenuIndex2--;
				break;

			case SK_ARROW_DOWN:
				soundUtil.Stop(SndChannel);
				soundUtil.Play(Snd.MenuSelect, SndChannel);

				if (!SettingState && !QuestionToDesktop) 
					MenuIndex++;
				else if (SettingState || QuestionToDesktop)
					MenuIndex2++;
				break;
			}

			EX.ClampValue(MenuIndex, 0, 2, CLAMP_RETURN);
			if(SettingState)
				EX.ClampValue(MenuIndex2, 0, 4, CLAMP_RETURN);
			if(QuestionToDesktop)
				EX.ClampValue(MenuIndex2, 0, 1, CLAMP_RETURN);

			for (int i = 0; i < 3; i++)
				MenuFocused[i] = false;

			if (SettingState) {
				for (int i = 0; i < 5; i++)
					SettingFocused[i] = false;
				SettingFocused[MenuIndex2] = true;
			}

			else if (QuestionToDesktop) {
				for (int i = 0; i < 2; i++)
					QuestionFocused[i] = false;
				QuestionFocused[MenuIndex2] = true;
			}

			MenuFocused[MenuIndex] = true;
		}

		else if (Event.Type == NORMAL_KEY_DOWN) {
			switch (Event.NormalKey) {
			case NK_ENTER:
				if (!SettingState && !QuestionToDesktop) {
					soundUtil.Stop(SndChannel);
					if (MenuIndex == 0) {
						soundUtil.Stop(Glb.BGMChannel);
						soundUtil.Play(Snd.CartCrash, SndChannel);
						GameStart = true;
						scene.DeleteInputObject(this);
					}

					else 
						soundUtil.Play(Snd.MenuSelect, SndChannel);

					switch (MenuIndex) {
					case 0:
						break;
					case 1:
						SettingState = true;
						break;
					case 2:
						QuestionToDesktop = true;
						break;
					}
					break;
				}

				if (SettingState) {
					if (MenuIndex2 == 0 || MenuIndex2 == 4) {
						soundUtil.Stop(SndChannel);
						soundUtil.Play(Snd.MenuSelect, SndChannel);
					}

					switch (MenuIndex2) {
					case 0:
						SettingState = false;
						for (int i = 0; i < 5; i++)
							SettingFocused[i] = false;
						SettingFocused[0] = true;
						MenuIndex2 = 0;
						break;
					}
					break;
				}

				if (QuestionToDesktop) {
					soundUtil.Stop(SndChannel);
					soundUtil.Play(Snd.MenuSelect, SndChannel);

					switch (MenuIndex2) {
					case 0:
						System.Exit();
						break;
					case 1:
						QuestionToDesktop = false;
						QuestionFocused[1] = false;
						QuestionFocused[0] = true;
						MenuIndex2 = 0;
						break;
					}
					break;
				}
				break;

			case NK_ESCAPE:
				soundUtil.Stop(SndChannel);
				soundUtil.Play(Snd.MenuSelect, SndChannel);

				if (!SettingState && !QuestionToDesktop) 
					QuestionToDesktop = true;

				else if (QuestionToDesktop) {
					QuestionToDesktop = false;
					QuestionFocused[1] = false;
					QuestionFocused[0] = true;
					MenuIndex2 = 0;
				}

				else if (SettingState) {
					SettingState = false;
					for (int i = 0; i < 5; i++)
						SettingFocused[i] = false;
					SettingFocused[0] = true;
					MenuIndex2 = 0;
				}
				break;
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		// ȭ���� ������ �� 2�� �� �÷��� ���� ��ȯ�Ѵ�
		if (GameStart) {
			StartTimer.Update(FrameTime);
			if (StartTimer.MiliSec() >= 1)
				scene.SwitchMode(PlayModePtr);
		}

		if (!IntroEnd) {
			if (!GameStart) {
				MoveDelayTimer.Update(FrameTime);
				if (MoveDelayTimer.Sec() >= 3) {
					mathUtil.Lerp(TitleCameraPosition, 0.7, 0.4, FrameTime);
					mathUtil.Lerp(TitleCameraZoom, 1.5, 0.4, FrameTime);
				}
				cameraControl.MoveCamera(TitleCameraPosition, 0.4);
				cameraControl.ChangeCameraZoom(TitleCameraZoom);

				if (TitleCameraPosition > 0.3)
					RenderText = true;
			}	
		}

		if(GameStart)
			cameraControl.ChangeCameraZoom(1.0);

		if(RenderText) {
			TextOpacity += FrameTime;
			EX.ClampValue(TextOpacity, 1.0, CLAMP_GREATER);
		}

		XionSize = XionLoop.Update(0.03, 15.0, FrameTime);
	//	EDSize = EDLoop.Update(0.03, 4.0, FrameTime);
		ShakeTimer.Update(FrameTime);
		if (ShakeTimer.CheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE)) {
			ShakePosition.x = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
			ShakePosition.y = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
		}
	}

	void RenderFunc() {
		if (GameStart) {
			Begin(RENDER_TYPE_STATIC);
			transform.Scale(MoveMatrix, ASP(2.0), 2.0);
			imageUtil.Render(SysRes.COLOR_TEXTURE);
		}

		else {
			// �ÿ�
			Begin();
			transform.Move(MoveMatrix, -1.3, XionSize * 0.5);
			transform.Scale(MoveMatrix, 2.0, 2.0 + XionSize);
			imageUtil.RenderStaticSpriteSheet(Img.Xion, Xion_Nervous);

			// �̵�
			Begin();
			transform.Move(MoveMatrix, ShakePosition.x, EDSize * 0.5 + ShakePosition.y);
			transform.Scale(MoveMatrix, 2.0, 2.0 + EDSize);
			imageUtil.RenderStaticSpriteSheet(Img.ED, ED_Title);

			// īƮ
			Begin();
			transform.Move(MoveMatrix, 0.7, -0.3);
			transform.Scale(MoveMatrix, 1.5, 1.5);
			imageUtil.RenderStaticSpriteSheet(Img.Cart, 0);

			if (RenderText) {
				Text.SetOpacity(TextOpacity);

				// �޴� ǥ��
				if (!SettingState && !QuestionToDesktop) {
					Text.SetUseShadow(true);

					for (int i = 0; i < 3; i++) {
						if (MenuFocused[i])
							Text.SetColorRGB(255, 213, 80);
						else
							Text.SetColor(1.0, 1.0, 1.0);

						Text.Render(ASP(1.0) - 0.1, 0.25 - 0.25 * i, 0.1, MenuItems[i].c_str());
					}
				}

				else if (SettingState) {
					Begin(RENDER_TYPE_STATIC);
					transform.Scale(MoveMatrix, ASP(2.0), 2.0);
					imageUtil.Render(SysRes.COLOR_TEXTURE, 0.7);

					Text.SetUseShadow(false);
					Text.SetColor(1.0, 1.0, 1.0);
					Text.SetAlign(ALIGN_MIDDLE);
					Text.Render(0.0, 0.8, 0.15, L"ȯ�漳��");

					Text.SetAlign(ALIGN_LEFT);
					for (int i = 0; i < 5; i++) {
						if (SettingFocused[i])
							Text.SetColorRGB(255, 213, 80);
						else
							Text.SetColor(1.0, 1.0, 1.0);

						switch (i) {
						case 0:
							Text.Render(ASP(1.0) - 0.1, 0.5 - i * 0.25, 0.1, L"Ÿ��Ʋ�� ���ư���");
							break;
						case 1:
							if (Glb.FullscreenAcvivated)
								Text.Render(ASP(1.0) - 0.1, 0.5 - i * 0.25, 0.1, L"<   ȭ�� ���: ��üȭ��   >");
							else
								Text.Render(ASP(1.0) - 0.1, 0.5 - i * 0.25, 0.1, L"<   ȭ�� ���: â   >");
							break;
						case 2:
							Text.Render(ASP(1.0) - 0.1, 0.5 - i * 0.25, 0.1, L"<   ������� ����: %.1f   >", Glb.BGMVolume);
							break;
						case 3:
							Text.Render(ASP(1.0) - 0.1, 0.5 - i * 0.25, 0.1, L"<   ȿ���� ����: %.1f   >", Glb.SFXVolume);
							break;
						case 4:
							Text.Render(ASP(1.0) - 0.1, 0.5 - i * 0.25, 0.1, L"ũ����");
							break;
						}
					}
				}

				else if (QuestionToDesktop) {
					Begin(RENDER_TYPE_STATIC);
					transform.Scale(MoveMatrix, ASP(2.0), 2.0);
					imageUtil.Render(SysRes.COLOR_TEXTURE, 0.7);

					Text.SetUseShadow(false);
					Text.SetColor(1.0, 1.0, 1.0);
					Text.SetAlign(ALIGN_MIDDLE);
					Text.Render(0.0, 0.8, 0.15, L" ����ȭ������ �������?");

					Text.SetAlign(ALIGN_LEFT);
					for (int i = 0; i < 2; i++) {
						if (QuestionFocused[i])
							Text.SetColorRGB(255, 213, 80);
						else
							Text.SetColor(1.0, 1.0, 1.0);

						Text.Render(ASP(1.0) - 0.1, 0.125 - i * 0.25, 0.1, QuestionItems[i].c_str());
					}
				}
			}
		}

		//RenderStart = true;
	}
};