#include "GameMode1.h"

//////////////////////////////////// Start Mode Option
// Specifies the mode to run first when the program starts.
//You must include the mode header file, and declare the mode class and mode's instance.
GameMode1 Mode1;
GameMode1* GameMode1::M_Inst;

START_MODE_PTR START_MODE = Mode1.Start;