#pragma once
#pragma warning(disable: 4244)
#pragma warning(disable: 4305)
#pragma warning(disable: 26444)
#pragma comment(lib, "winmm.lib")
#include "Config.h"
#include "glew.h"
#include "freeglut.h"
#include "freeglut_ext.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <locale>
#include <iostream>
#include <vector>
#include <string>
#include <mmsystem.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
using namespace DirectX;

#include "fmod.hpp"
#include "fmod_errors.h"

extern float DestFPS;
extern int FPSLimit;
extern bool FullscreenState;

extern const glm::mat4 IdentityMatrix;

typedef void(*START_MODE_PTR)(void);
using ShaderLocation = unsigned int;

using Sound          = FMOD::Sound*;
using SoundChannel   = FMOD::Channel*;
using SoundChannelGroup = std::vector <FMOD::Channel*>;

using StringDataVec  = std::vector<std::string>;
using DigitDataVec   = std::vector<float>;


enum KeyState {
	NORMAL_KEY_DOWN,
	NORMAL_KEY_UP,
	SPECIAL_KEY_DOWN,
	SPECIAL_KEY_UP
};

enum WheelState {
	WHEEL_DOWN,
	WHEEL_UP
};

enum ButtonState {
	LEFT_BUTTON_DOWN,
	LEFT_BUTTON_UP,
	RIGHT_BUTTON_DOWN,
	RIGHT_BUTTON_UP,
	MIDDLE_BUTTON_DOWN,
	MIDDLE_BUTTON_UP
};

enum SpecialKeySet {
	SK_ARROW_UP    = GLUT_KEY_UP,
	SK_ARROW_DOWN  = GLUT_KEY_DOWN,
	SK_ARROW_RIGHT = GLUT_KEY_RIGHT,
	SK_ARROW_LEFT  = GLUT_KEY_LEFT,
	SK_PAGE_UP     = GLUT_KEY_PAGE_UP,
	SK_PAGE_DOWN   = GLUT_KEY_PAGE_DOWN,
	SK_HOME        = GLUT_KEY_HOME,
	SK_END         = GLUT_KEY_END,
	SK_INSERT      = GLUT_KEY_INSERT,
	SK_DELETE      = GLUT_KEY_DELETE,
	SK_F1          = GLUT_KEY_F1,
	SK_F2          = GLUT_KEY_F2,
	SK_F3          = GLUT_KEY_F3,
	SK_F4          = GLUT_KEY_F4,
	SK_F5          = GLUT_KEY_F5,
	SK_F6          = GLUT_KEY_F6,
	SK_F7          = GLUT_KEY_F7,
	SK_F8          = GLUT_KEY_F8,
	SK_F9          = GLUT_KEY_F9,
	SK_F10         = GLUT_KEY_F10,
	SK_F11         = GLUT_KEY_F11,
	SK_F12         = GLUT_KEY_F12,
	SK_SHIFT_RIGHT = GLUT_KEY_SHIFT_R,
	SK_SHIFT_LEFT  = GLUT_KEY_SHIFT_L,
	SK_CTRL_RIGHT  = GLUT_KEY_CTRL_R,
	SK_CTRL_LEFT   = GLUT_KEY_CTRL_L,
	SK_ALT_RIGHT   = GLUT_KEY_ALT_R,
	SK_ALT_LEFT    = GLUT_KEY_ALT_L
};

enum NormalKeySet {
	NK_SPACE     = 32,
	NK_ENTER     = 13,
	NK_ESCAPE    = 27,
	NK_TAB       = 9,
	NK_BACKSPACE = 8
};

enum ZoomType{
	ZOOM_IN, 
	ZOOM_OUT
};

enum RenderTypeEnum {
	RENDER_TYPE_DEFAULT,
	RENDER_TYPE_STATIC
};

enum ShaderTypeEnum {
	SHADER_TYPE_IMAGE,
	SHADER_TYPE_TEXT,
	SHADER_TYPE_SHAPE
};

enum ImageTypeEnum {
	IMAGE_TYPE_LINEAR,
	IMAGE_TYPE_NEAREST
};

enum DataTypeEnum {
	DATA_TYPE_DIGIT,
	DATA_TYPE_STRING
};

enum ImageFlipEnum {
	FLIP_TYPE_NONE,
	FLIP_TYPE_X,
	FLIP_TYPE_Y,
	FLIP_TYPE_XY
};

enum TextAlignEnum {
	ALIGN_DEFAULT,
	ALIGN_MIDDLE,
	ALIGN_LEFT
};

enum TextHeightClampEnum {
	HEIGHT_ALIGN_DEFAULT,
	HEIGHT_ALIGN_MIDDLE,
	HEIGHT_ALIGN_UNDER
};

enum CheckTimeOptionEnum {
	CHECK_AND_RESUME,
	CHECK_AND_RESET,
	CHECK_AND_INTERPOLATE,
	CHECK_AND_STOP
};

enum UIClampEdgeEnum {
	EDGE_LEFT_DOWN,
	EDGE_LEFT_UP,
	EDGE_RIGHT_UP,
	EDGE_RIGHT_DOWN
};

enum LineSTypeEnum {
	LINE_TYPE_RECT,
	LINE_TYPE_ROUND
};

enum EXClampTypeEnum {
	CLAMP_GREATER,
	CLAMP_LESS,
	CLAMP_FIXED,
	CLAMP_RETURN
};

enum DeleteRangeEnum {
	DELETE_RANGE_SINGLE,
	DELETE_RANGE_ALL
};

enum ModeTypeEnum {
	MODE_TYPE_NONE,
	MODE_TYPE_DEFAULT,
	MODE_TYPE_FLOATING
};

enum ObjectTypeEnum {
	OBJECT_TYPE_NONE,
	OBJECT_TYPE_STATIC,
	OBJECT_TYPE_STATIC_SINGLE,
	OBJECT_TYPE_FLOATING
};

enum DistTypeEnum {
	RANDOM_TYPE_REAL,
	RANDOM_TYPE_INT
};

enum SwitchTypeEnum {
	TRUE_KEY_DOWN,
	FALSE_KEY_DOWN
};

namespace Preset {
	constexpr float MaxPositive  = XM_PI / 2.0;
	constexpr float MaxNegative  = -XM_PI / 2.0;
	constexpr float HalfPositive = XM_PI / 6.0;
	constexpr float HalfNegative = -XM_PI / 6.0;
}

// ray vector struct
typedef struct {
	XMVECTOR Origin;
	XMVECTOR Direction;
	GLfloat Distance;
	GLfloat Length;
}RayVector;

// data set struct
struct FileData {
	int         DataType;
	std::string CategoryName;
	std::string DataName;
	float       DigitValue;
	std::string StringValue;
};
using DataFormat = std::vector<FileData>;

// image struct
typedef struct {
	unsigned int Texture;
	int          Width, Height;
}Image;

// sprite sheet struct
typedef struct {
	std::vector<unsigned int> Texture;
	int                Width, Height;
	int                Frame;
}SpriteSheet;

// pre load image struct
typedef struct {
	Image*         ImagePtr;
	unsigned char* TextureData;
	int            ImageType;
}PreLoadInfo;

// pre load sprite sheet struct
typedef struct {
	SpriteSheet*   SpriteSheetPtr;
	std::vector<unsigned char*> TextureData;
	int            ImageType;
}PreLoadSpriteSheetInfo;

// corner position of display
struct ViewportRect {
	GLfloat lx, ly, rx, ry;
};
extern ViewportRect WindowRect;

// key event
typedef struct {
	int Type;
	unsigned char NormalKey;
	int SpecialKey;
}KeyEvent;


// display width, height and apsect ratio
extern int WIDTH, HEIGHT;
extern int PREV_WIDTH, PREV_HEIGHT;
extern GLfloat ASPECT;

// viewport background color
extern glm::vec3 BackColor;

// start mode function ptr
extern START_MODE_PTR START_MODE;

// global scope shader
extern GLuint IMAGE_SHADER;
extern GLuint TEXT_SHADER;
extern GLuint SHAPE_SHADER;
extern GLuint MATRIX_COMPT_SHADER;

// global SSBO I/O
extern GLuint SSBO_MATRIX_INPUT, SSBO_MATRIX_OUTPUT;

// global shader location
extern ShaderLocation IMAGE_OPACITY_LOCATION, IMAGE_COLOR_LOCATION, IMAGE_MODEL_LOCATION;
extern ShaderLocation IMAGE_PROJECTION_LOCARION, IMAGE_VIEW_LOCATION, IMAGE_VIEW_POSITION_LOCATION;
extern ShaderLocation BLUR_STRENGTH_LOCATION, BLUR_STATE_LOCATION, TEXTURE_SIZE_LOCATION;

extern ShaderLocation TEXT_OPACITY_LOCATION, TEXT_COLOR_LOCATION, TEXT_MODEL_LOCATION;
extern ShaderLocation TEXT_PROJECTION_LOCATION, TEXT_VIEW_LOCATION, TEXT_VIEW_POSITION_LOCATION;

extern ShaderLocation SHAPE_OPACITY_LOCATION, SHAPE_COLOR_LOCATION, SHAPE_MODEL_LOCATION;
extern ShaderLocation SHAPE_PROJECTION_LOCATION, SHAPE_VIEW_LOCATION, SHAPE_VIEW_POSITION_LOCATION;

class SDKSystem {
private:
	bool  UpdateActivateCommand{};
	float PrevTime{}, CurrentTime{}, DeltaTime{};

	float DestFPS{};
	int   FPSLimit{};

	bool  FullscreenState{};

public:
	static SDKSystem* S_Inst;

	SDKSystem() {
		S_Inst = this;
	}

	static GLvoid Main();
	static GLvoid DisplayReshape(int w, int h);
	void SwitchScreenState();
	void ChangeScreenSize(int ScreenWidth, int ScreenHeight);
	void SetFrameLimit(int FrameLimit);
	void HideCursor();
	void ShowCursor();
	void SetBackColor(GLfloat R, GLfloat G, GLfloat B);
	void SetBackColorRGB(int R, int G, int B);
	void SetupSystem(int argc, char** argv);
	void InitSystem();
	void SetGlOption();
	void SetupWindow();
	void LoadShader();
	void Exit();
};
extern SDKSystem System;

// Multiply the value by the window aspect ratio.
GLfloat ASP(GLfloat Value);

enum SystemErrorTypeEnum {
	ERROR_TYPE_IMAGE_LOAD,
	ERROR_TYPE_AUDIO_LOAD,
	ERROR_TYPE_FONT_LOAD,
	ERROR_TYPE_SCRIPT_LOAD,
	ERROR_TYPE_SCRIPT_PARSE,
	ERROR_TYPE_SCRIPT_DATA,
	ERROR_TYPE_SCRIPT_CATEGORY,
	ERROR_TYPE_DATA_FILE_PARSE,
	ERROR_TYPE_DATA_FILE_DATA_LOAD,
	ERROR_TYPE_DATA_FILE_DATA_WRITE,
	ERROR_TYPE_DATA_FILE_CATEGORY,
};