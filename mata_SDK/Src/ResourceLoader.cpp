#include "GameResource.h"
#include "Config.h"
#include "ImageUtil.h"

// Use LoadT(), LoadClipT(), and LoadSpriteSheetT() to load image files and sprite sheets in this thread.
DWORD WINAPI ImageResourceLoader(LPVOID lpParam) {
	imageUtil.SetSpriteSheetSize(256, 256, 7, 1);
	imageUtil.LoadSpriteSheetT(Img.ED, "Assets//Sprite//sheet-ed.png");

	imageUtil.SetSpriteSheetSize(128, 128, 3, 1);
	imageUtil.LoadSpriteSheetT(Img.Coffee, "Assets//Sprite//sheet-coffee.png");

	return 0;
}

DWORD WINAPI SoundResourceLoader(LPVOID lpParam) {
	// load sound resources here

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