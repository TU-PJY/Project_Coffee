#include "PlayMode.h"
#include "GameOverMode.h"
#include "TitleMode.h"
#include "CreditMode.h"

//////////////////////////////////// Start Mode Option
// Specifies the mode to run first when the program starts.
//You must include the mode header file, and declare the mode class and mode's instance.
GameOver_Mode GameOverMode;
GameOver_Mode* GameOver_Mode::M_Inst;

Pause_Mode PauseMode;
Pause_Mode* Pause_Mode::M_Inst;

Title_Mode TitleMode;
Title_Mode* Title_Mode::M_Inst;

Play_Mode PlayMode;
Play_Mode* Play_Mode::M_Inst;

Credit_Mode CreditMode;
Credit_Mode* Credit_Mode::M_Inst;

MODE_PTR PlayModePtr = PlayMode.Start;
MODE_PTR CreditModePtr = CreditMode.Start;

START_MODE_PTR START_MODE = TitleMode.Start;