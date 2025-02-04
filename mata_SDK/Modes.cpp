#include "PlayMode.h"

//////////////////////////////////// Start Mode Option
// Specifies the mode to run first when the program starts.
//You must include the mode header file, and declare the mode class and mode's instance.
Play_Mode Mode1;
Play_Mode* Play_Mode::M_Inst;

START_MODE_PTR START_MODE = Mode1.Start;