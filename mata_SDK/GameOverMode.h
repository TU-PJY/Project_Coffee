#pragma once
#include <ModeHeader.h>

#include <CameraController.h>
#include "GameOverScreen.h"

class GameOver_Mode {
public:
	std::string ModeName{ "GameOverMode" };
	int         ModeType{ MODE_TYPE_DEFAULT };

	std::vector<std::string> InputObjectTag
	{
		"game_over_screen"
	};

	/////////////////////////////////////////////////////////////

	static void Start() {
		cameraControl.MoveCamera(0.0, 0.0);
		cameraControl.ChangeCameraZoom(1.0);
		scene.AddObject(new GameOverScreen, "game_over_screen", LAYER1);
		SetUp();
	}

	static void Destructor() {
	}

	/////////////////////////////////////////////////////////////
	// Fold here
#pragma region FoldRegion 
	std::vector<GameObject*> InputObject{};
	static GameOver_Mode* M_Inst;

	GameOver_Mode() {
		M_Inst = this;
	}

	static void SetUp() {

		M_Inst->InputObject.clear();

		for (auto const& Tag : M_Inst->InputObjectTag) {
			if (auto Object = scene.Find(Tag); Object)
				M_Inst->InputObject.emplace_back(Object);
		}

		scene.RegisterDestructor(Destructor);
		scene.RegisterInputObjectList(M_Inst->InputObject);
		scene.RegisterController(Controller, M_Inst->ModeType);
		scene.RegisterModeName(M_Inst->ModeName);
	}

	static void ProcessKeyEvent(KeyEvent& Event) {
		for (auto const& Object : M_Inst->InputObject)
			if (Object)  Object->InputKey(Event);

	}
	static void KeyDown(unsigned char KEY, int X, int Y) {
#ifdef ENABLE_DEV_EXIT
		if (KEY == NK_ESCAPE)
			System.Exit();
#endif
		KeyEvent Event{ NORMAL_KEY_DOWN, KEY, NULL };
		ProcessKeyEvent(Event);
	}

	static void KeyUp(unsigned char KEY, int X, int Y) {
		KeyEvent Event{ NORMAL_KEY_UP, KEY, NULL };
		ProcessKeyEvent(Event);
	}

	static void SpecialKeyDown(int KEY, int X, int Y) {
		KeyEvent Event{ SPECIAL_KEY_DOWN, NULL, KEY };
		ProcessKeyEvent(Event);
	}

	static void SpecialKeyUp(int KEY, int X, int Y) {
		KeyEvent Event{ SPECIAL_KEY_UP, NULL, KEY };
		ProcessKeyEvent(Event);
	}

	static void MouseMotion(int X, int Y) {
		mouse.ConvertPosition(X, Y);
	}

	static void MousePassiveMotion(int X, int Y) {
		mouse.ConvertPosition(X, Y);
	}

	static void MouseWheel(int Button, int Wheel, int X, int Y) {
		int WheelEvent{};

		if (Wheel > 0)
			WheelEvent = WHEEL_UP;
		else if (Wheel < 0)
			WheelEvent = WHEEL_DOWN;

		for (auto const& Object : M_Inst->InputObject)
			if (Object)  Object->InputScroll(WheelEvent);
	}

	static void MouseButton(int Button, int State, int X, int Y) {
		int ButtonEvent{};

		switch (State) {
		case GLUT_DOWN:
			switch (Button) {
			case GLUT_LEFT_BUTTON:
				ButtonEvent = LEFT_BUTTON_DOWN;   break;
			case GLUT_RIGHT_BUTTON:
				ButtonEvent = RIGHT_BUTTON_DOWN;  break;
			case GLUT_MIDDLE_BUTTON:
				ButtonEvent = MIDDLE_BUTTON_DOWN; break;
			}
			break;

		case GLUT_UP:
			switch (Button) {
			case GLUT_LEFT_BUTTON:
				ButtonEvent = LEFT_BUTTON_UP;   break;
			case GLUT_RIGHT_BUTTON:
				ButtonEvent = RIGHT_BUTTON_UP;  break;
			case GLUT_MIDDLE_BUTTON:
				ButtonEvent = MIDDLE_BUTTON_UP; break;
			}
			break;
		}

		for (auto const& Object : M_Inst->InputObject)
			if (Object)  Object->InputMouse(ButtonEvent);
	}

	static void Controller() {
		glutMotionFunc(MouseMotion);
		glutPassiveMotionFunc(MousePassiveMotion);
		glutKeyboardFunc(KeyDown);
		glutKeyboardUpFunc(KeyUp);
		glutMouseWheelFunc(MouseWheel);
		glutMouseFunc(MouseButton);
		glutSpecialFunc(SpecialKeyDown);
		glutSpecialUpFunc(SpecialKeyUp);
	}
#pragma endregion
};
extern GameOver_Mode GameOverMode;