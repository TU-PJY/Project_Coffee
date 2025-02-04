#pragma once
#include "SDKHeader.h"

class EX_Util {
public:
	void SwitchBool(bool& Value);
	void SwitchValue(float& TargetValue, float Value1, float Value2);
	void SwitchValue(int& TargetValue, int Value1, int Value2);
	void ClampValue(float& Value, float Dest, int Type);
	void ClampValue(int& Value, int Dest, int Type);
	void ClampValue(int& Value, int Min, int Max, int Type);
	void ClampValue(float& Value, float Min, float Max, int Type);
	bool CheckClampValue(float& Value, float Dest, int Type);
	bool CheckClampValue(int& Value, int Dest, int Type);
	void InputBoolSwitch(int SwitchingOption, int EventType, bool& DestBoolValue);
};
extern EX_Util EX;