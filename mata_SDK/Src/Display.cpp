#include "SDKHeader.h"

GLvoid SDKSystem::DisplayReshape(int w, int h) {
	glViewport(0, 0, w, h);
	WIDTH = w;
	HEIGHT = h;
}

void SDKSystem::SetBackColor(GLfloat R, GLfloat G, GLfloat B) {
	BackColor.r = R;
	BackColor.g = G;
	BackColor.b = B;
}

void SDKSystem::SetBackColorRGB(int R, int G, int B) {
	BackColor.r = (1.0f / 255.0f) * (GLfloat)R;
	BackColor.g = (1.0f / 255.0f) * (GLfloat)G;
	BackColor.b = (1.0f / 255.0f) * (GLfloat)B;
}

void SDKSystem::SetFrameLimit(int FrameLimit) {
	FPSLimit = FrameLimit;
	DestFPS = 1000.0 / (float)FPSLimit;
}

void SDKSystem::HideCursor() {
	glutSetCursor(GLUT_CURSOR_NONE);
}

void SDKSystem::ShowCursor() {
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void SDKSystem::SwitchScreenState() {
	if (!FullscreenState) {
		glutFullScreen();
		WIDTH = GetSystemMetrics(SM_CXSCREEN);
		HEIGHT = GetSystemMetrics(SM_CYSCREEN);
		FullscreenState = true;
	}
	else {
		RECT DisplayArea{};

		if (SystemParametersInfo(SPI_GETWORKAREA, 0, &DisplayArea, 0)) {
			int DisplayWidth = DisplayArea.right - DisplayArea.left;
			int DisplayHeight = DisplayArea.bottom - DisplayArea.top;
			WIDTH = DisplayWidth;
			HEIGHT = DisplayHeight;
			glutReshapeWindow(WIDTH, HEIGHT);
			glutPositionWindow(0, 0);
			FullscreenState = false;
		}
	}
}

void SDKSystem::ChangeScreenSize(int ScreenWidth, int ScreenHeight) {
	WIDTH = ScreenWidth;
	HEIGHT = ScreenHeight;
	glutReshapeWindow(WIDTH, HEIGHT);
	glutPositionWindow(0, 0);
	FullscreenState = false;
}
