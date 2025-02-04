#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Library/tinyxml/tinyxml.h"
#include "Library/cryptopp/aes.h" 
#include "Library/cryptopp/filters.h"
#include "Library/cryptopp/modes.h"
#include "Library/cryptopp/osrng.h"
#include "Library/cryptopp/hex.h"
#include "Config.h"
#include "SDKHeader.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace CryptoPP;

// read only
class ScriptUtil {
private:
	TiXmlDocument Doc{};
	TiXmlElement* Root{};
	bool          FileExist{};

	std::string   CategorySearch{};
	std::string   DataSearch{};

public:
	ScriptUtil() {}

	void Load(std::string FileName);
	void ImportSecure(std::string FileName);
	float LoadDigitData(std::string CategoryName, std::string DataName);
	std::string LoadStringData(std::string CategoryName, std::string DataName);
	std::wstring LoadWstringData(std::string CategoryName, std::string DataName);
	DigitDataVec LoadCategoryDigitData(std::string CategoryName);
	StringDataVec LoadCategoryStringData(std::string CategoryName);
	void Release();

	int CountCategory();

private:
	TiXmlElement* FindCategory(std::string CategoryName);
	std::string FindData(std::string CategoryName, std::string DataName);
	std::string Decrypt(const std::string& CipherText, const byte Key[], const byte IV[]);
	float GetDigitData(TiXmlElement* CategoryVar, std::string DataName);
	std::string GetStringData(TiXmlElement* CategoryVar, std::string DataName);
};