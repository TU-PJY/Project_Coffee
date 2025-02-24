#include "SDKHeader.h"
#include "Scene.h"
#include "CameraUtil.h"
#include "CameraController.h"
#include "ShaderUtil.h"
#include "Config.h"
#include "GameResource.h"
#include "SystemText.h"
#include <windows.h>

#include "LoadingMode.h"


int WIDTH = WINDOW_WIDTH;
int HEIGHT = WINDOW_HEIGHT;
int PREV_WIDTH, PREV_HEIGHT;

std::wstring LocaleType;

const glm::mat4 IdentityMatrix = glm::mat4(1.0f);

Camera camera;
GameObject* Indicator;

GLfloat ASPECT;
ViewportRect WindowRect;
glm::vec3 BackColor;

TextUtil SystemText;

SystemResource SysRes;
ImageResource Img;
SoundResource Snd;
DataFormatResource Fmt;
DataResource Dat;
GlobalResource Glb; 
TextResource Txt;

void SDKSystem::SetupSystem(int argc, char** argv) {
	glutInit(&argc, argv);
	SetupWindow();
	LoadShader();
	SetGlOption();
	InitSystem();
}


void SDKSystem::SetupWindow() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GL_MULTISAMPLE);

	glutInitWindowPosition(GetSystemMetrics(SM_CXSCREEN) / 2 - WIDTH / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - HEIGHT / 2);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow(WINDOW_NAME);

	if (FULL_SCREEN_OPTION) {
		glutFullScreen();
		WIDTH = GetSystemMetrics(SM_CXSCREEN);
		HEIGHT = GetSystemMetrics(SM_CYSCREEN);
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Unable to initialize GLEW\n\n";
		exit(EXIT_FAILURE);
	}

	const GLubyte* Version = glGetString(GL_VERSION);
	std::cout << Version << std::endl;

	RegisterHotKey(NULL, 1, MOD_ALT, VK_MENU);  // Left Alt
	RegisterHotKey(NULL, 2, MOD_ALT | MOD_NOREPEAT, VK_MENU);  // Right Alt
}

void SDKSystem::LoadShader() {
	shaderUtil.LoadVertexShader("SDKResource//GLSL//Vertex.glsl");
	shaderUtil.LoadFragmentShader("SDKResource//GLSL//Fragment_Image.glsl");
	shaderUtil.CreateShader(IMAGE_SHADER);

	shaderUtil.LoadVertexShader("SDKResource//GLSL//Vertex.glsl");
	shaderUtil.LoadFragmentShader("SDKResource//GLSL//Fragment_Text.glsl");
	shaderUtil.CreateShader(TEXT_SHADER);

	shaderUtil.LoadVertexShader("SDKResource//GLSL//Vertex.glsl");
	shaderUtil.LoadFragmentShader("SDKResource//GLSL//Fragment_Shape.glsl");
	shaderUtil.CreateShader(SHAPE_SHADER);

	shaderUtil.LoadComputeShader("SDKResource//GLSL//ComputeMatrix.glsl");
	shaderUtil.CreateComputeShader(MATRIX_COMPT_SHADER);

	shaderUtil.CreateShaderLocation();
	shaderUtil.CreateSSBO();
}

void SDKSystem::SetGlOption() {
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
}

void SDKSystem::InitSystem() {
	FPSLimit = FRAME_LIMITS;
	if (FPSLimit > 0)
		DestFPS = 1000.0 / (float)FPSLimit;

	scene.Init(LoadingMode.Start);
}