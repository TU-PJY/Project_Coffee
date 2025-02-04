#include "ScriptUtil.h"
#include "Scene.h"
#include "StringUtil.h"

void ScriptUtil::Load(std::string FileName) {
	if (FileExist)
		return;

	if (!Doc.LoadFile(FileName.c_str(), TIXML_ENCODING_UTF8)) {
		scene.SetErrorScreen(ERROR_TYPE_SCRIPT_LOAD, FileName);
		return;
	}

	Root = Doc.RootElement();
	std::cout << "Script util opened file: " << FileName << std::endl;
	FileExist = true;
}

void ScriptUtil::ImportSecure(std::string FileName) {
	std::ifstream EncryptedFile(FileName, std::ios::binary);
	if (!EncryptedFile) {
		scene.SetErrorScreen(ERROR_TYPE_SCRIPT_LOAD, FileName);
		return;
	}

	std::string EncryptedContent((std::istreambuf_iterator<char>(EncryptedFile)), std::istreambuf_iterator<char>());
	EncryptedFile.close();

	std::string DecryptedXML = Decrypt(EncryptedContent, AES_KEY, IV_KEY);
	Doc.Parse(DecryptedXML.c_str(), nullptr, TIXML_ENCODING_UTF8);

	if (Doc.Error()) {
		scene.SetErrorScreen(ERROR_TYPE_SCRIPT_PARSE, FileName);
		return;
	}

	Root = Doc.RootElement();
	FileExist = true;
}

void ScriptUtil::Release() {
	Doc.Clear();
	FileExist = false;
}

int ScriptUtil::CountCategory() {
	int CatCount{};
	for (TiXmlElement* Element = Root->FirstChildElement(); Element != nullptr; Element = Element->NextSiblingElement())
		CatCount++;

	return CatCount;
}

float ScriptUtil::LoadDigitData(std::string CategoryName, std::string DataName) {
	CategorySearch = CategoryName;
	DataSearch = DataName;
	return GetDigitData(FindCategory(CategoryName), DataName);
}

std::string ScriptUtil::LoadStringData(std::string CategoryName, std::string DataName) {
	CategorySearch = CategoryName;
	DataSearch = DataName;
	return GetStringData(FindCategory(CategoryName), DataName);
}

std::wstring ScriptUtil::LoadWstringData(std::string CategoryName, std::string DataName) {
	return stringUtil.Wstring(LoadStringData(CategoryName, DataName));
}

DigitDataVec ScriptUtil::LoadCategoryDigitData(std::string CategoryName) {
	DigitDataVec LoadedData{};
	TiXmlElement* Category = FindCategory(CategoryName);

	if (!Category) {
		scene.SetErrorScreen(ERROR_TYPE_SCRIPT_CATEGORY, CategoryName);
		return {};
	}

	TiXmlAttribute* Attribute = Category->FirstAttribute();
	while (Attribute) {
		LoadedData.emplace_back(std::stof(Attribute->Value()));
		Attribute = Attribute->Next();
	}

	return LoadedData;
}

StringDataVec ScriptUtil::LoadCategoryStringData(std::string CategoryName) {
	StringDataVec LoadedData{};
	TiXmlElement* Category = FindCategory(CategoryName);

	if (!Category) {
		scene.SetErrorScreen(ERROR_TYPE_SCRIPT_CATEGORY, CategoryName);
		return {};
	}

	TiXmlAttribute* Attribute = Category->FirstAttribute();
	while (Attribute) {
		LoadedData.emplace_back((std::string)Attribute->Value());
		Attribute = Attribute->Next();
	}

	return LoadedData;
}

//////////////////////////////// private
float ScriptUtil::GetDigitData(TiXmlElement* CategoryVar, std::string DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName.c_str());
	if (DataValue)
		return std::stof(DataValue);
	else {
		scene.SetErrorScreen(ERROR_TYPE_SCRIPT_DATA, CategorySearch, DataSearch);
		return 0.0;
	}
}

std::string ScriptUtil::GetStringData(TiXmlElement* CategoryVar, std::string DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName.c_str());
	if (DataValue)
		return (std::string)DataValue;
	else {
		scene.SetErrorScreen(ERROR_TYPE_SCRIPT_DATA, CategorySearch, DataSearch);
		return "";
	}
}

TiXmlElement* ScriptUtil::FindCategory(std::string CategoryName) {
	return Root->FirstChildElement(CategoryName.c_str());
}

std::string ScriptUtil::FindData(std::string CategoryName, std::string DataName) {
	TiXmlElement* FoundCategory = FindCategory(CategoryName);
	if (!FoundCategory) {
		scene.SetErrorScreen(ERROR_TYPE_SCRIPT_CATEGORY, CategorySearch);
		return "";
	}
	else {
		const char* DataValue = FoundCategory->Attribute(DataName.c_str());
		return (std::string)DataValue;
	}
}

std::string ScriptUtil::Decrypt(const std::string& CipherText, const byte Key[], const byte IV[]) {
	std::string PlainText;

	try {
		CBC_Mode<AES>::Decryption Decryption;
		Decryption.SetKeyWithIV(Key, AES::DEFAULT_KEYLENGTH, IV);
		StringSource(CipherText, true, new StreamTransformationFilter(Decryption, new StringSink(PlainText)));
	}
	catch (const Exception& E) {
		std::cerr << E.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	return PlainText;
}