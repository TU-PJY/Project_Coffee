#pragma once
#include "SDKHeader.h"
#include "Imageutil.h"
#include "ScriptUtil.h"
#include "FileUtil.h"

// Declare the sprite resource here. 
class ImageResource {
public:
	SpriteSheet ED{};

	SpriteSheet Coffee{};
	SpriteSheet DestroyedCoffee{};
	SpriteSheet Shelf{};
	SpriteSheet Other{};
	SpriteSheet Drop{};
	Image Stick{};

	SpriteSheet Cart{};
};

// Declare the sound resource here. 
class SoundResource {
public:
	Sound Whoosh{};
	Sound NextWhoosh{};

	Sound BoxBreak[3]{}, GlassBreak[3]{}, CanBreak[3]{};
};

// Data Format list
// Write your data format here.
// {  Data Type, Category Name, Data Name, Digit Value, String Value }
class DataFormatResource {
public:

};

// Decalre the data resource here.
class DataResource {
public:

};

// Declare the global value here.
class GlobalResource {
public:

};

// Declare the global text object here.
class TextResource {
public:
};

// 이드 상태 열거형, 스프라이트 시트 프레임 결정에 사용한다.
enum ED_StateEnum {
	Idle,
	HitHigh1,
	HitHigh2,
	HitLow1,
	HitLow2,
	HitPeople,
	GameOver
};

// 커피 종류 열거형
enum CoffeeTypeEnum {
	Box,
	Glass,
	Can
};

// 물건 구조체
typedef struct {
	// 커피 종류
	int Type;

	// 커피 위치
	glm::vec2 Position;

	// 위 아래 위치 여부
	bool IsUpside;

	// 파괴 상태
	bool Destroyed;
}ItemStruct;

extern ImageResource Img;
extern SoundResource Snd;
extern DataFormatResource Fmt;
extern DataResource Dat;
extern GlobalResource Glb;
extern TextResource Txt;

DWORD WINAPI ImageResourceLoader(LPVOID lpParam);
DWORD WINAPI SoundResourceLoader(LPVOID lpParam);
DWORD WINAPI DataResourceLoader(LPVOID lpParam);
DWORD WINAPI FontResourceLoader(LPVOID lpParam);
void InitText();