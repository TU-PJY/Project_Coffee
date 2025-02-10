#pragma once
#include "SDKHeader.h"
#include "Imageutil.h"
#include "ScriptUtil.h"
#include "FileUtil.h"
#include "TextUtil.h"

// Declare the sprite resource here. 
class ImageResource {
public:
	SpriteSheet ED{};
	SpriteSheet Xion{};

	SpriteSheet Coffee{};
	SpriteSheet DestroyedCoffee{};
	Image DestroyedCan{};
	SpriteSheet Shelf{};
	SpriteSheet Other{};
	SpriteSheet Drop{};
	Image Stick{};

	SpriteSheet Cart{};
	SpriteSheet People{};

	Image Smoke{};

	Image FloorTile{};
};

// Declare the sound resource here. 
class SoundResource {
public:
	Sound Whoosh{};
	Sound MissWhoosh{};
	Sound NextWhoosh{};

	Sound BoxBreak[3]{}, GlassBreak[3]{}, CanBreak[3]{};
	Sound CanDrop{};
	Sound PeopleHit{};
	
	Sound CartCrash{};
	Sound Explode{};

	Sound AddTime{};
	Sound GameOver{};
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
	int PrevChFrame[10]{};
	bool CreateAvailable[10]{ true, true, true, true, true, true, true, true, true, true };

	// 게임 오버 상태
	bool GameOver{};

	// 점수
	int Score{};

	// 파괴하거나 때린 숫자
	int BoxCoffeeBreakCount{};
	int CanCoffeeBreakCount{};
	int GlassCoffeeBreakCount{};
	int PeopleHitCount{};

	// 거쳐간 선반 개수
	int ShelfCount{};
};

// Declare the global text object here.
class TextResource {
public:
	TextUtil PixelText{};
};

// 이드 프레임
enum ED_StateEnum {
	Idle,
	HitHigh1,
	HitHigh2,
	HitLow1,
	HitLow2,
	KickPeople,
	GameOver
};

// 시온 프레임
enum XionStateEnum {
	Nervous,
	Cry1,
	Curious,
	Blocking,
	Cry2,
	Hurt
};

// 커피 종류
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

	// 앞에 사람이 서있는지 여부
	bool IsPeopleFront;

	// 시온이 막아설 위치인지 여부
	bool IsXionFront;
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