#pragma once
#include <Scene.h>

#include "PlayMode.h"

class TitleScreen : public GameObject {
private:
	// ��Ʈ�� �� ����
	bool IntroEnd{};

	// �޴� �ؽ�Ʈ �� ���� ���õ� �ؽ�Ʈ
	TextUtil Text{};
	bool MenuFocused[3]{};
	int MenuIndex{};

	bool QuestionToDesktop{};
	bool QuestionFocused[2]{};

	bool SettingState{};
	bool SettingFocused[5]{};
	int MenuIndex2{};

	std::wstring MenuItems[3] = {
		L"����!",
		L"ȯ�漳��",
		L"����ȭ������\n������"
	};

	std::wstring QuestionItems[2] = {
		L"��",
		L"�ƴϿ�"
	};

public:
	TitleScreen(bool IntroFlag) {
		if (IntroFlag)
			IntroEnd = true;
		else
			IntroEnd = false;

		MenuFocused[0] = true;
		SettingFocused[0] = true;
		QuestionFocused[0] = true;

		Text.Init(L"�ȼ��κ��κ�ü", FW_DONTCARE);
		Text.SetAlign(ALIGN_LEFT);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.SetUseShadow(true);
		Text.SetShadow(0.1, glm::vec3(0.0, 0.0, 0.0), 0.8);
	}

	void InputKey(KeyEvent& Event) {
		if (Event.Type == SPECIAL_KEY_DOWN) {
			switch (Event.SpecialKey) {
			case SK_ARROW_UP:
				if (!SettingState && !QuestionToDesktop)
					MenuIndex--;
				else if(SettingState || QuestionToDesktop)
					MenuIndex2--;
				break;

			case SK_ARROW_DOWN:
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
					switch (MenuIndex) {
					case 0:
						scene.SwitchMode(PlayMode.Start);
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

	}

	void RenderFunc() {
		// �޴� ǥ��
		if (!SettingState && !QuestionToDesktop) {
			for (int i = 0; i < 3; i++) {
				if (MenuFocused[i])
					Text.SetColorRGB(255, 213, 80);
				else
					Text.SetColor(1.0, 1.0, 1.0);

				Text.Render(ASP(1.0) - 0.1, 0.25 - 0.25 * i, 0.1, MenuItems[i].c_str());
			}
		}

		if (SettingState) {
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
					if(Glb.FullscreenAcvivated)
						Text.Render(ASP(1.0) - 0.1, 0.5 - i * 0.25, 0.1, L"ȭ�� ���: ��üȭ��");
					else
						Text.Render(ASP(1.0) - 0.1, 0.5 - i * 0.25, 0.1, L"ȭ�� ���: â");
					break;
				case 2:
					Text.Render(ASP(1.0) - 0.1, 0.5 - i * 0.25, 0.1, L"������� ����: %.1f", Glb.BGMVolume);
					break;
				case 3:
					Text.Render(ASP(1.0) - 0.1, 0.5 - i * 0.25, 0.1, L"ȿ���� ����: %.1f", Glb.SFXVolume);
					break;
				case 4:
					Text.Render(ASP(1.0) - 0.1, 0.5 - i * 0.25, 0.1, L"ũ����");
					break;
				}
			}
		}

		if (QuestionToDesktop) {
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
};