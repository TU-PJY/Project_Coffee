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
	Image XionBack{};
	SpriteSheet ED_GameOver{};
	SpriteSheet Chloe{};

	SpriteSheet Coffee{};
	SpriteSheet CoffeeBack{};
	SpriteSheet DestroyedCoffee{};
	Image DestroyedCan{};
	SpriteSheet Shelf{};
	SpriteSheet Other{};
	SpriteSheet Drop{};
	Image Stick{};

	SpriteSheet Cart{};
	SpriteSheet People{};
	SpriteSheet PeopleBack{};

	Image Smoke{};
	Image FloorTile{};

	Image GameOverBackGround{};
	Image GameOverBackGround2{};

	Image BackGroundShelf{};
	Image Pillar{};

	SpriteSheet CreditED{};
	SpriteSheet CreditXion{};
	SpriteSheet Sofa{};

	Image TV{};
	Image TVLight{};
	Image Chair{};

	Image Title{};

	Image LoadingED{};
	Image mataLogo{};

	Image ZZZ{};
};

// Declare the sound resource here. 
class SoundResource {
public:
	Sound Whoosh{};
	Sound MissWhoosh{};
	Sound NextWhoosh{};

	Sound BoxBreak[3]{}, Bottle[3]{}, CanBreak[3]{};
	Sound CanDrop{};
	Sound PeopleHit{};
	
	Sound CartCrash{};
	Sound Explode{};

	Sound CountDownReady{};
	Sound CountDownGo{};
	Sound AddTime{};
	Sound GameOver{};
	Sound HitCheek{};

	Sound GameEnd[3]{};
	Sound Rep{};
	Sound RepTotal{};

	Sound PlayBgm{};
	Sound TitleBgm{};
	Sound CreditBGM{};
	Sound ShutDown{};

	Sound MenuSelect{};
};

// Data Format list
// Write your data format here.
// {  Data Type, Category Name, Data Name, Digit Value, String Value }
class DataFormatResource {
public:
	DataFormat HighScore = {
		{DATA_TYPE_DIGIT, "HighScore", "Score", 0, ""},
		{DATA_TYPE_DIGIT, "HighScore", "Rep", 0, ""},
	};

	DataFormat UserSettings = {
		{DATA_TYPE_DIGIT, "Setting", "Fullscreen", 1, ""},
		{DATA_TYPE_DIGIT, "Setting", "BGMVolume", 1.0, ""},
		{DATA_TYPE_DIGIT, "Setting", "SFXVolume", 1.0, ""}
	};
};

// Decalre the data resource here.
class DataResource {
public:
	FileUtil HighscoreData{};
	FileUtil UserSettingData{};
};

// Declare the global value here.
class GlobalResource {
public:
	// ���� ĳ���� ������ �� ĳ���� ������ ��� ���� ����
	int PrevChFrame[10]{};
	bool CreateAvailable[10]{ true, true, true, true, true, true, true, true, true, true };

	// Ÿ��Ʋ ��Ʈ�� ��� ����
	bool TitleIntroPlayed{};

	// ���� ���� ����
	bool GameOver{};

	// ����
	int Score{};

	// �ı��ϰų� ���� ����
	int DestroyedItems[6]{};

	// ���ӿ��� ����
	int Ending{};

	// Bgm ä��
	SoundChannel BGMChannel{};

	// ���� ����
	float BGMVolume{};
	float SFXVolume{};

	// ���� ǥ�ÿ� ���� ��
	//int BGMVolumeInt{};
	//int SFXVolumeInt{};

	// ��üȭ�� ����
	bool FullscreenAcvivated{};

	// ���� ���̽��ھ�
	int HighScore{};

	//  ���� �ִ� ����
	int MaxRep{};

	// ���� �ְ� ��� �޼� ����
	bool IsHighScore{};
	bool IsHighRep{};
};

// Declare the global text object here.
class TextResource {
public:

};

// �̵� ������
enum ED_StateEnum {
	ED_Idle,
	ED_HitHigh1,
	ED_HitHigh2,
	ED_HitLow1,
	ED_HitLow2,
	ED_KickPeople,
	ED_GameOver,
	ED_Title,
	ED_Happy
};

// �ÿ� ������
enum XionStateEnum {
	Xion_Nervous,
	Xion_Cry1,
	Xion_Curious,
	Xion_Blocking,
	Xion_Cry2,
	Xion_Hurt,
	Xion_Happy
};

// Ŀ�� ����
enum CoffeeTypeEnum {
	Coffee_Box,
	Coffee_Bottle,
	Coffee_Can
};

enum DestroyedTypeEnum {
	Item_BoxCoffee,
	Item_BottleCoffee,
	Item_CanCoffee,
	Item_People,
	Item_Xion,
	Item_HouseDoc
};

enum GameOverEndingEnum {
	GameOver_TimeOut,
	GameOver_HitXion,
	GameOver_Suppressed
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