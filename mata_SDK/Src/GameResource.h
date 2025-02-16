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
	// 이전 캐릭터 프레임 및 캐릭터 프레임 사용 가능 여부
	int PrevChFrame[10]{};
	bool CreateAvailable[10]{ true, true, true, true, true, true, true, true, true, true };

	// 타이틀 인트로 재생 여부
	bool TitleIntroPlayed{};

	// 게임 오버 상태
	bool GameOver{};

	// 점수
	int Score{};

	// 파괴하거나 때린 숫자
	int DestroyedItems[6]{};

	// 게임오버 엔딩
	int Ending{};

	// Bgm 채널
	SoundChannel BGMChannel{};

	// 유저 설정
	float BGMVolume{};
	float SFXVolume{};

	// 설정 표시용 볼륨 값
	//int BGMVolumeInt{};
	//int SFXVolumeInt{};

	// 전체화면 여부
	bool FullscreenAcvivated{};

	// 유저 하이스코어
	int HighScore{};

	//  유저 최대 배상금
	int MaxRep{};

	// 이전 최고 기록 달성 여부
	bool IsHighScore{};
	bool IsHighRep{};
};

// Declare the global text object here.
class TextResource {
public:

};

// 이드 프레임
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

// 시온 프레임
enum XionStateEnum {
	Xion_Nervous,
	Xion_Cry1,
	Xion_Curious,
	Xion_Blocking,
	Xion_Cry2,
	Xion_Hurt,
	Xion_Happy
};

// 커피 종류
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