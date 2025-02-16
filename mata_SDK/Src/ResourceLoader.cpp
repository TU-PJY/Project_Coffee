#include "GameResource.h"
#include "Config.h"

#include "ImageUtil.h"
#include "SoundUtil.h"
#include "FontUtil.h"

// Use LoadT(), LoadClipT(), and LoadSpriteSheetT() to load image files and sprite sheets in this thread.
DWORD WINAPI ImageResourceLoader(LPVOID lpParam) {
	imageUtil.SetSpriteSheetSize(256, 256, 9, 1);
	imageUtil.LoadSpriteSheetT(Img.ED, "Assets//Sprite//People//sheet-ed.png");

	imageUtil.SetSpriteSheetSize(256, 256, 7, 1);
	imageUtil.LoadSpriteSheetT(Img.Xion, "Assets//Sprite//People//sheet-xion.png");
	imageUtil.LoadT(Img.XionBack, "Assets//Sprite//People//xion-back.png");

	imageUtil.SetSpriteSheetSize(256, 256, 2, 1);
	imageUtil.LoadSpriteSheetT(Img.ED_GameOver, "Assets//Sprite//People//sheet-ed-gameover.png");
	imageUtil.LoadSpriteSheetT(Img.Chloe, "Assets//Sprite//People//sheet-chloe.png");

	imageUtil.SetSpriteSheetSize(256, 256, 10, 2);
	imageUtil.LoadSpriteSheetT(Img.People, "Assets//Sprite//People//sheet-people.png");
	imageUtil.LoadSpriteSheetT(Img.PeopleBack, "Assets//Sprite//People//sheet-people-back.png");

	imageUtil.SetSpriteSheetSize(256, 256, 4, 1);
	imageUtil.LoadSpriteSheetT(Img.CreditED, "Assets//Sprite//People//sheet-ed-credit.png");

	imageUtil.SetSpriteSheetSize(256, 256, 2, 1);
	imageUtil.LoadSpriteSheetT(Img.CreditXion, "Assets//Sprite//People//sheet-xion-credit.png");




	imageUtil.SetSpriteSheetSize(256, 256, 3, 1);
	imageUtil.LoadSpriteSheetT(Img.Shelf, "Assets//Sprite//Object//sheet-shelf.png");

	imageUtil.SetSpriteSheetSize(128, 128, 3, 1);
	imageUtil.LoadSpriteSheetT(Img.Coffee, "Assets//Sprite//Object//sheet-coffee.png");
	imageUtil.LoadSpriteSheetT(Img.CoffeeBack, "Assets//Sprite//Object//sheet-coffee-back.png");
	imageUtil.LoadSpriteSheetT(Img.DestroyedCoffee, "Assets//Sprite//Object//sheet-destroyed.png");
	imageUtil.LoadSpriteSheetT(Img.Other, "Assets//Sprite//Object//sheet-other.png");

	imageUtil.SetSpriteSheetSize(30, 30, 2, 1);
	imageUtil.LoadSpriteSheetT(Img.Drop, "Assets//Sprite//Object//sheet-drop.png");

	imageUtil.LoadT(Img.DestroyedCan, "Assets//Sprite//Object//can-destroyed.png");
	imageUtil.LoadT(Img.Stick, "Assets//Sprite//Object//stick.png");

	imageUtil.SetSpriteSheetSize(256, 256, 2, 1);
	imageUtil.LoadSpriteSheetT(Img.Cart, "Assets//Sprite//Object//sheet-cart.png");
	imageUtil.LoadT(Img.Smoke, "Assets//Sprite//Object//smoke.png");

	imageUtil.SetSpriteSheetSize(512, 256, 3, 1);
	imageUtil.LoadSpriteSheetT(Img.Sofa, "Assets//Sprite//Object//sheet-sofa.png");

	imageUtil.LoadT(Img.TV, "Assets///Sprite//Object//TV.png");
	imageUtil.LoadT(Img.TVLight, "Assets//Sprite//Object//TV-light.png");
	imageUtil.LoadT(Img.Chair, "Assets//Sprite//Object//chair.png");



	imageUtil.LoadT(Img.FloorTile, "Assets//Sprite//Background//floor.png");
	imageUtil.LoadT(Img.GameOverBackGround, "Assets//Sprite//Background//background-bad.png");
	imageUtil.LoadT(Img.GameOverBackGround2, "Assets//Sprite//Background//background-good.png");
	imageUtil.LoadT(Img.BackGroundShelf, "Assets//Sprite//Background//background-shelf.png");
	imageUtil.LoadT(Img.Pillar, "Assets//Sprite//Background//pillar.png");

	imageUtil.LoadT(Img.mataLogo, "SDKResource//Image//Common//mata_LOGO.png", IMAGE_TYPE_LINEAR);

	imageUtil.LoadT(Img.ZZZ, "Assets//Sprite//Object//z.png");

	return 0;
}

DWORD WINAPI SoundResourceLoader(LPVOID lpParam) {
	soundUtil.Load(Snd.Whoosh, "Assets//Sound//SFX//whoosh.mp3");
	soundUtil.Load(Snd.MissWhoosh, "Assets//Sound//SFX//whoosh-miss.mp3");
	soundUtil.Load(Snd.NextWhoosh, "Assets//Sound//SFX//next-shelf.mp3");

	for (int i = 0; i < 3; ++i) {
		std::string FileName = "Assets//Sound//SFX//box-hit-";
		FileName += std::to_string(i + 1) + ".mp3";
		soundUtil.Load(Snd.BoxBreak[i], FileName);
	}

	for (int i = 0; i < 3; ++i) {
		std::string FileName = "Assets//Sound//SFX//glass-hit-";
		FileName += std::to_string(i + 1) + ".mp3";
		soundUtil.Load(Snd.Bottle[i], FileName);
	}

	for (int i = 0; i < 3; ++i) {
		std::string FileName = "Assets//Sound//SFX//metal-hit-";
		FileName += std::to_string(i + 1) + ".mp3";
		soundUtil.Load(Snd.CanBreak[i], FileName);
	}
	soundUtil.Load(Snd.CanDrop, "Assets//Sound//SFX//metal-drop.mp3");

	soundUtil.Load(Snd.PeopleHit, "Assets//Sound//SFX//people-hit.mp3");
	soundUtil.Load(Snd.CartCrash, "Assets//Sound//SFX//cart-crash.mp3");
	soundUtil.Load(Snd.Explode, "Assets//Sound//SFX//explode.ogg");

	soundUtil.Load(Snd.CountDownReady, "Assets//Sound//SFX//countdown-ready.mp3");
	soundUtil.Load(Snd.CountDownGo, "Assets//Sound//SFX//countdown-go.mp3");
	soundUtil.Load(Snd.AddTime, "Assets//Sound//SFX//time-add.mp3");

	soundUtil.Load(Snd.HitCheek, "Assets//Sound//SFX//hit-cheek.mp3");
	soundUtil.Load(Snd.Rep, "Assets//Sound//SFX//rep.mp3");
	soundUtil.Load(Snd.RepTotal, "Assets//Sound//SFX//rep-total.mp3");

	soundUtil.Load(Snd.MenuSelect, "Assets//Sound//SFX//menu-select.mp3");

	soundUtil.Load(Snd.ShutDown, "Assets//Sound//SFX//shutdown.mp3");


	soundUtil.Load(Snd.GameOver, "Assets//Sound//BGM//gameover.mp3");
	soundUtil.Load(Snd.GameEnd[0], "Assets//Sound//BGM//gameover-timeout.mp3");
	soundUtil.Load(Snd.GameEnd[1], "Assets//Sound//BGM//gameover-hit-xion.mp3");
	soundUtil.Load(Snd.GameEnd[2], "Assets//Sound//BGM//gameover-suppress.mp3");
	soundUtil.Load(Snd.PlayBgm, "Assets//Sound//BGM//play-bgm.mp3", FMOD_LOOP_NORMAL);
	soundUtil.Load(Snd.TitleBgm, "Assets//Sound//BGM//title-bgm.mp3", FMOD_LOOP_NORMAL);
	soundUtil.Load(Snd.CreditBGM, "Assets//Sound//BGM//credit-bgm.mp3", FMOD_LOOP_NORMAL);

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