#pragma once
#include "SDKHeader.h"

class FontUtil {
public:
	bool Load(const std::string& FontFilePath);
	bool LoadT(const std::string& FontFilePath);
};
extern FontUtil fontUtil;
