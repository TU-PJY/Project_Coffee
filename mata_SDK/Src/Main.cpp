#include "SDKHeader.h"
#include "CameraController.h"
#include "FPSInd.h"
#include "Scene.h"
#include <thread>
#include <chrono>

SDKSystem System;
SDKSystem* SDKSystem::S_Inst;

GLvoid SDKSystem::Main() {
	if (S_Inst->FPSLimit > 0) {
		std::chrono::duration<double> Duration(S_Inst->DestFPS);
		std::this_thread::sleep_for(Duration);
	}

	glClearColor(BackColor.r, BackColor.g, BackColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (S_Inst->UpdateActivateCommand) {
		scene.Update();
		scene.Render();
		scene.CompleteCommand();
#ifdef USE_SOUND_SYSTEM
		soundUtil.Update();
#endif
	}

	if (SHOW_FPS && Indicator)
		Indicator->RenderIndicator();

	S_Inst->CurrentTime = float(glutGet(GLUT_ELAPSED_TIME));
	S_Inst->DeltaTime = (S_Inst->CurrentTime - S_Inst->PrevTime) / 1000.0;
	scene.InputFrameTime(S_Inst->DeltaTime);

	S_Inst->PrevTime = S_Inst->CurrentTime;

	glutSwapBuffers();
	glutPostRedisplay();

	S_Inst->UpdateActivateCommand = true;
}

void main(int argc, char** argv) {
	std::locale::global(std::locale(""));
	std::wcout.imbue(std::locale());
	std::cout.imbue(std::locale());

	if (SHOW_CONSOLE) {
		if (AllocConsole()) {
			FILE* FP{};

			freopen_s(&FP, "CONOUT$", "w", stdout);
			freopen_s(&FP, "CONIN$", "r", stdin);
			freopen_s(&FP, "CONERR$", "w", stderr);

			std::cout << "Console initialized successfully." << std::endl;
		}
	}

	System.SetupSystem(argc, argv);
	glutDisplayFunc(System.Main);
	glutReshapeFunc(System.DisplayReshape);
	glutMainLoop();
}

void SDKSystem::Exit() {
	glutDestroyWindow(1);
}

GLfloat ASP(GLfloat Value) {
	return Value * ASPECT;
}