#include "GameResource.h"
#include "Config.h"

#include "ImageUtil.h"
#include "SoundUtil.h"

// Use LoadT(), LoadClipT(), and LoadSpriteSheetT() to load image files and sprite sheets in this thread.
DWORD WINAPI ImageResourceLoader(LPVOID lpParam) {
	imageUtil.SetSpriteSheetSize(256, 256, 7, 1);
	imageUtil.LoadSpriteSheetT(Img.ED, "Assets//Sprite//sheet-ed.png");

	imageUtil.SetSpriteSheetSize(256, 256, 6, 1);
	imageUtil.LoadSpriteSheetT(Img.Xion, "Assets//Sprite//sheet-xion.png");

	imageUtil.SetSpriteSheetSize(256, 256, 3, 1);
	imageUtil.LoadSpriteSheetT(Img.Shelf, "Assets//Sprite//sheet-shelf.png");

	imageUtil.SetSpriteSheetSize(128, 128, 3, 1);
	imageUtil.LoadSpriteSheetT(Img.Coffee, "Assets//Sprite//sheet-coffee.png");
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

	imageUtil.LoadT(Img.Smoke, "Assets//Sprite//smoke.png");

	imageUtil.LoadT(Img.FloorTile, "Assets//Sprite//floor.png");

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
		soundUtil.Load(Snd.GlassBreak[i], FileName);
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

	return 0;
}

DWORD WINAPI DataResourceLoader(LPVOID lpParam) {
	// load data resources here

	return 0;
}

DWORD WINAPI FontResourceLoader(LPVOID lpParam) {
	// Load font resources here

	return 0;
}

void InitText() {
	// Init Text objects here

}