#pragma once
#include <ModeHeader.h>
#include <CameraController.h>

#include "ED.h"
#include "Shelf.h"
#include "Cart.h"
#include "Xion.h"
#include "Floor.h"
#include "TimeWatch.h"
#include "Score.h"
#include "CountDown.h"
#include "PlayModeManager.h"
#include "BackgroundShelf.h"
#include "Pillar.h"
// render order

// floor, Background
// Shelf
// ED, People
// FellDown Cart
// Object
// FellDown People
// UI

class Play_Mode {
public:
	// define mode name and mode type here
	std::string ModeName { "PlayMode" };
	int         ModeType { MODE_TYPE_DEFAULT };

	// type object tag to input device event
	std::vector<std::string> InputObjectTag
	{
		"ed",
		"play_mode_manager"
	};

	/////////////////////////////////////////////////////////////

	static void Start() {
		System.SetBackColorRGB(122, 138, 154);

		// 이전에 파괴했던 개수 초기화
		for (int i = 0; i < 6; i++)
			Glb.DestroyedItems[i] = 0;

		// 점수 초기화
		Glb.Score = 0;

		// 사람 생성 가능 여부 초기화
		for (int i = 0; i < 10; i++) {
			Glb.PrevChFrame[i] = 0;
			Glb.CreateAvailable[i] = true;
		}

		// 게임 오버 상태 초기화
		Glb.GameOver = false;


		scene.AddObject(new PlayModeManager, "play_mode_manager", LAYER1);

		scene.AddObject(new Pillar(1.0), "pillar", LAYER_BG);

		// 타일 추가
		GLfloat Position = ASP(-1.0) - 1.6;
		while (true) {
			scene.AddObject(new Floor(Position, true), "floor", LAYER_BG);
			Position += 0.8;

			if (Position > ASP(1.0) + 0.8) {
				scene.AddObject(new Floor(Position, false), "floor", LAYER_BG);
				break;
			}
		}

		scene.AddObject(new BackgroundShelf(0.0), "background_shelf", LAYER1);

		scene.AddObject(new Shelf(2, 1.75), "shelf", LAYER2);
		scene.AddObject(new Xion(-1.0, 0.0, false, Xion_Cry1), "xion", LAYER3);
		scene.AddObject(new ED, "ed", LAYER3);
		scene.AddObject(new Cart(true, glm::vec2(0.8, -0.35)), "cart", LAYER4);

		scene.AddObject(new TimeWatch, "time_watch", LAYER7);
		scene.AddObject(new ScoreIndicator, "score_indicator", LAYER7);
		scene.AddObject(new CountDown, "countdown", LAYER7);

		cameraControl.MoveCamera(0.5, 0.0);

		SetUp();
	}

	static void Destructor() {
		// Add task here
	}

	/////////////////////////////////////////////////////////////
	// Fold here
#pragma region FoldRegion 
	// this is a container that stores object pointers for accessing object controllers.
	// a pointer to the object corresponding to the tag entered in InputObjectTag is added when the mode starts.
	std::vector<GameObject*> InputObject{};
	static Play_Mode* M_Inst;

	Play_Mode() {
		M_Inst = this;
	}

	static void SetUp() {
		M_Inst->InputObject.clear();

		for (auto const& Tag : M_Inst->InputObjectTag) {
			if (auto Object = scene.Find(Tag); Object)
				M_Inst->InputObject.emplace_back(Object);
		}

		scene.RegisterModeName(M_Inst->ModeName);
		scene.RegisterDestructor(Destructor);
		scene.RegisterInputObjectList(M_Inst->InputObject);
		scene.RegisterController(Controller, M_Inst->ModeType);
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
extern Play_Mode PlayMode;