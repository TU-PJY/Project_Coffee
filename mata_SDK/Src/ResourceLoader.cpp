#include "GameResource.h"
#include "Config.h"

#include "ImageUtil.h"
#include "SoundUtil.h"
#include "FontUtil.h"

// Use LoadT(), LoadClipT(), and LoadSpriteSheetT() to load image files and sprite sheets in this thread.
DWORD WINAPI ImageResourceLoader(LPVOID lpParam) {
	imageUtil.SetSpriteSheetSize(256, 256, 9, 1);
	imageUtil.LoadSpriteSheetT(Img.ED, "Assets//Sprite//sheet-ed.png");

	imageUtil.SetSpriteSheetSize(256, 256, 7, 1);
	imageUtil.LoadSpriteSheetT(Img.Xion, "Assets//Sprite//sheet-xion.png");
	imageUtil.LoadT(Img.XionBack, "Assets//Sprite//xion-back.png");

	imageUtil.SetSpriteSheetSize(256, 256, 2, 1);
	imageUtil.LoadSpriteSheetT(Img.ED_GameOver, "Assets//Sprite//sheet-ed-gameover.png");
	imageUtil.LoadSpriteSheetT(Img.Chloe, "Assets//Sprite//sheet-chloe.png");

	imageUtil.SetSpriteSheetSize(256, 256, 3, 1);
	imageUtil.LoadSpriteSheetT(Img.Shelf, "Assets//Sprite//sheet-shelf.png");

	imageUtil.SetSpriteSheetSize(128, 128, 3, 1);
	imageUtil.LoadSpriteSheetT(Img.Coffee, "Assets//Sprite//sheet-coffee.png");
	imageUtil.LoadSpriteSheetT(Img.CoffeeBack, "Assets//Sprite//sheet-coffee-back.png");
	imageUtil.LoadSpriteSheetT(Img.DestroyedCoffee, "Assets//Sprite//sheet-destroyed.png");
	imageUtil.LoadSpriteSheetT(Img.Other, "Assets//Sprite//sheet-other.png");

	imageUtil.LoadT(Img.DestroyedCan, "Assets//Sprite//can-destroyed.png");

	imageUtil.SetSpriteSheetSize(30, 30, 2, 1);
	imageUtil.LoadSpriteSheetT(Img.Drop, "Assets//Sprite//sheet-drop.png");

	imageUtil.LoadT(Img.Stick, "Assets//Sprite//stick.png");

	imageUtil.SetSpriteSheetSize(256, 256, 2, 1);
	imageUtil.LoadSpriteSheetT(Img.Cart, "Assets//Sprite//sheet-cart.png");

	imageUtil.SetSpriteSheetSize(256, 256, 10, 2);
	imageUtil.LoadSpriteSheetT(Img.People, "Assets//Sprite//sheet-people.png");
	imageUtil.LoadSpriteSheetT(Img.PeopleBack, "Assets//Sprite//sheet-people-back.png");

	imageUtil.LoadT(Img.Smoke, "Assets//Sprite//smoke.png");
	imageUtil.LoadT(Img.FloorTile, "Assets//Sprite//floor.png");

	imageUtil.LoadT(Img.GameOverBackGround, "Assets//Sprite//background-bad.png");
	imageUtil.LoadT(Img.GameOverBackGround2, "Assets//Sprite//background-good.png");
	imageUtil.LoadT(Img.BackGroundShelf, "Assets//Sprite//background-shelf.png");
	imageUtil.LoadT(Img.Pillar, "Assets//Sprite//pillar.png");

	return 0;
}

DWORD WINAPI SoundResourceLoader(LPVOID lpParam) {
	soundUtil.Load(Snd.Whoosh, "Assets//Sound//whoosh.mp3");
	soundUtil.Load(Snd.MissWhoosh, "Assets//Sound//whoosh-miss.mp3");
	soundUtil.Load(Snd.NextWhoosh, "Assets//Sound//next-shelf.mp3");

	for (int i = 0; i < 3; ++i) {
		std::string FileName = "Assets//Sound//box-hit-";
		FileName += std::to_string(i + 1) + ".mp3";
		soundUtil.Load(Snd.BoxBreak[i], FileName);
	}

	for (int i = 0; i < 3; ++i) {
		std::string FileName = "Assets//Sound//glass-hit-";
		FileName += std::to_string(i + 1) + ".mp3";
		soundUtil.Load(Snd.Bottle[i], FileName);
	}

	for (int i = 0; i < 3; ++i) {
		std::string FileName = "Assets//Sound//metal-hit-";
		FileName += std::to_string(i + 1) + ".mp3";
		soundUtil.Load(Snd.CanBreak[i], FileName);
	}

	soundUtil.Load(Snd.CanDrop, "Assets//Sound//metal-drop.mp3");
	soundUtil.Load(Snd.PeopleHit, "Assets//Sound//people-hit.mp3");
	soundUtil.Load(Snd.CartCrash, "Assets//Sound//cart-crash.mp3");
	soundUtil.Load(Snd.Explode, "Assets//Sound//explode.ogg");

	soundUtil.Load(Snd.CountDownReady, "Assets//Sound//countdown-ready.mp3");
	soundUtil.Load(Snd.CountDownGo, "Assets//Sound//countdown-go.mp3");
	soundUtil.Load(Snd.AddTime, "Assets//Sound//time-add.mp3");
	soundUtil.Load(Snd.GameOver, "Assets//Sound//gameover.mp3");
	soundUtil.Load(Snd.HitCheek, "Assets//Sound//hit-cheek.mp3");

	soundUtil.Load(Snd.GameEnd[0], "Assets//Sound//gameover-timeout.mp3");
	soundUtil.Load(Snd.GameEnd[1], "Assets//Sound//gameover-hit-xion.mp3");
	soundUtil.Load(Snd.GameEnd[2], "Assets//Sound//gameover-suppress.mp3");

	soundUtil.Load(Snd.Rep, "Assets//Sound//rep.mp3");
	soundUtil.Load(Snd.RepTotal, "Assets//Sound//rep-total.mp3");

	soundUtil.Load(Snd.PlayBgm, "Assets//Sound//play-bgm.mp3", FMOD_LOOP_NORMAL);
	soundUtil.Load(Snd.TitleBgm, "Assets//Sound//title-bgm.mp3", FMOD_LOOP_NORMAL);
	soundUtil.Load(Snd.MenuSelect, "Assets//Sound//menu-select.mp3");

	return 0;
}

DWORD WINAPI DataResourceLoader(LPVOID lpParam) {
	Dat.HighscoreData.Load("GameData//HigeScore", Fmt.HighScore);
	Dat.UserSettingData.Load("GameData//UserSettings", Fmt.UserSettings);

	Glb.FullscreenAcvivated = Dat.UserSettingData.LoadDigitData("Setting", "Fullscreen");
	Glb.BGMVolume = Dat.UserSettingData.LoadDigitData("Setting", "BGMVolume");
	Glb.SFXVolume = Dat.UserSettingData.LoadDigitData("Setting", "SFXVolume");
	Glb.HighScore = Dat.HighscoreData.LoadDigitData("HighScore", "Score");
	Glb.MaxRep = Dat.HighscoreData.LoadDigitData("HighScore", "Rep");

	return 0;
}

DWORD WINAPI FontResourceLoader(LPVOID lpParam) {
	fontUtil.LoadT("Assets//Font//pixelroborobo.otf");

	return 0;
}

void InitText() {
}