#pragma once
#include "SDKHeader.h"

class SystemResource {
public:
	const char* SDK_LOGO_SOUND_DIRECTORY = "SDKResource//Sound//SDK_Logo_Sound.wav";
	const char* SDK_LOGO_IMAGE_DIRECTORY = "SDKResource//Image//Common//SDK_Logo.png";
	const char* SDK_LOGO_ERROR_IMAGE_DIRECTORY = "SDKResource//Image//Common//SDK_Logo_Error.png";
	const char* SDK_LOADING_SPINNER_DIRECTORY = "SDKResource//Image//Common//SDK_Loading_Spinner.png";
	const char* FMOD_LOGO_DIRECTORY = "SDKResource//Image//Common//FMOD_Logo.png";
	const char* COLOR_TEXTURE_DIRECTORY = "SDKResource//Image//ShapeUtil//ColorTexture.png";
	const char* SYSTEM_FONT_DIRECTORY = "SDKResource//Font//Roboto-Regular.ttf";

	Image SDK_LOGO{};
	Image SDK_LOGO_ERROR{};
	Image FMOD_LOGO{};
	Image COLOR_TEXTURE{};
	Image LOADING_SPINNER{};

	GLUquadric* GLU_CIRCLE{};
	GLUquadric* GLU_LINE_CIRCLE{};

	Sound INTRO_SOUND{};
};
extern SystemResource SysRes;