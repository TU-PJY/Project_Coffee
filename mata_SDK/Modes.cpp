#include "PlayMode.h"
#include "GameOverMode.h"

//////////////////////////////////// Start Mode Option
// Specifies the mode to run first when the program starts.
//You must include the mode header file, and declare the mode class and mode's instance.
Play_Mode PlayMode;
Play_Mode* Play_Mode::M_Inst;

GameOver_Mode GameOverMode;
GameOver_Mode* GameOver_Mode::M_Inst;

START_MODE_PTR START_MODE = PlayMode.Start;