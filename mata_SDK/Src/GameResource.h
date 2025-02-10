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

	// ���� ���� ����
	bool GameOver{};

	// ����
	int Score{};

	// �ı��ϰų� ���� ����
	int BoxCoffeeBreakCount{};
	int CanCoffeeBreakCount{};
	int GlassCoffeeBreakCount{};
	int PeopleHitCount{};

	// ���İ� ���� ����
	int ShelfCount{};
};

// Declare the global text object here.
class TextResource {
public:
	TextUtil PixelText{};
};

// �̵� ������
enum ED_StateEnum {
	Idle,
	HitHigh1,
	HitHigh2,
	HitLow1,
	HitLow2,
	KickPeople,
	GameOver
};

// �ÿ� ������
enum XionStateEnum {
	Nervous,
	Cry1,
	Curious,
	Blocking,
	Cry2,
	Hurt
};

// Ŀ�� ����
enum CoffeeTypeEnum {
	Box,
	Glass,
	Can
};

// ���� ����ü
typedef struct {
	// Ŀ�� ����
	int Type;

	// Ŀ�� ��ġ
	glm::vec2 Position;

	// �� �Ʒ� ��ġ ����
	bool IsUpside;

	// �ı� ����
	bool Destroyed;

	// �տ� ����� ���ִ��� ����
	bool IsPeopleFront;

	// �ÿ��� ���Ƽ� ��ġ���� ����
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