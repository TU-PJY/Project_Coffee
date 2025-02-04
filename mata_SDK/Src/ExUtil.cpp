#include "ExUtil.h"

EX_Util EX;

void EX_Util::SwitchBool(bool& Value) {
	if (!Value)
		Value = true;
	else
		Value = false;
}

void EX_Util::SwitchValue(float& TargetValue, float Value1, float Value2) {
	if (TargetValue == Value1)
		TargetValue = Value2;
	else
		TargetValue = Value1;
}

void EX_Util::SwitchValue(int& TargetValue, int Value1, int Value2) {
	if (TargetValue == Value1)
		TargetValue = Value2;
	else
		TargetValue = Value1;
}

void EX_Util::ClampValue(float& Value, float Dest, int Type) {
	switch (Type) {
	case CLAMP_GREATER:
		if (Value > Dest) Value = Dest;
		break;

	case CLAMP_LESS:
		if (Value < Dest) Value = Dest;
		break;
	}
}

void EX_Util::ClampValue(int& Value, int Dest, int Type) {
	switch (Type) {
	case CLAMP_GREATER:
		if (Value > Dest) Value = Dest;
		break;

	case CLAMP_LESS:
		if (Value < Dest) Value = Dest;
		break;
	}
}

void EX_Util::ClampValue(int& Value, int Min, int Max, int Type) {
	switch (Type) {
	case CLAMP_FIXED:
		if (Value < Min)
			Value = Min;
		else if (Value > Max)
			Value = Max;
		break;

	case CLAMP_RETURN:
		if (Value < Min)
			Value = Max;
		else if (Value > Max)
			Value = Min;
	}
}

void EX_Util::ClampValue(float& Value, float Min, float Max, int Type) {
	switch (Type) {
	case CLAMP_FIXED:
		if (Value < Min)
			Value = Min;
		else if (Value > Max)
			Value = Max;
		break;

	case CLAMP_RETURN:
		if (Value < Min)
			Value = Max;
		else if (Value > Max)
			Value = Min;
	}
}

bool EX_Util::CheckClampValue(float& Value, float Dest, int Type) {
	EX_Util::ClampValue(Value, Dest, Type);
	if (Value == Dest)
		return true;

	return false;
}

bool EX_Util::CheckClampValue(int& Value, int Dest, int Type) {
	EX_Util::ClampValue(Value, Dest, Type);
	if (Value == Dest)
		return true;

	return false;
}

void EX_Util::InputBoolSwitch(int SwitchingOption, int EventType, bool& DestBoolValue) {
	if (SwitchingOption == TRUE_KEY_DOWN) {
		if (EventType == NORMAL_KEY_DOWN || EventType == SPECIAL_KEY_DOWN)
			DestBoolValue = true;

		else if (EventType == NORMAL_KEY_UP || EventType == SPECIAL_KEY_UP)
			DestBoolValue = false;
	}

	else if (SwitchingOption == FALSE_KEY_DOWN) {
		if (EventType == NORMAL_KEY_DOWN || EventType == SPECIAL_KEY_DOWN)
			DestBoolValue = false;

		else if (EventType == NORMAL_KEY_UP || EventType == SPECIAL_KEY_UP)
			DestBoolValue = true;
	}
}