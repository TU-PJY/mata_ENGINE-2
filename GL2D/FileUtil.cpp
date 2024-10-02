#include "FileUtil.h"


void FileUtil::Init(const char* FolderName, const char* FileName, std::vector<FileData> List, FileType Type) {
	FType = Type;
	std::string TempFileName = FileName;

	if (FType == FileType::Script)
		TempFileName += ".xml";

	std::filesystem::path FolderPath = FolderName;
	if (!std::filesystem::exists(FolderName)) {
		if (!std::filesystem::create_directory(FolderPath)) {
			std::cout << "Falied to find folder" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	FilePath = FolderPath / TempFileName;
	FilePathStr = FilePath.string();

	DataListBuffer = List;

	if (!LoadDataFile(FilePathStr.c_str()))
		SetupData(List);
	else {
		FileExist = true;
		std::cout << "File Util Opened file: " << FilePathStr << std::endl;
		CheckDataVersion();
	}
}

void FileUtil::UpdateIntData(const char* CategoryName, const char* DataName, int Value) {
	WriteIntData(FindCategory(CategoryName), DataName, Value);
	UpdateDataFile();
}

void FileUtil::UpdateFloatData(const char* CategoryName, const char* DataName, float Value) {
	WriteFloatData(FindCategory(CategoryName), DataName, Value);
	UpdateDataFile();
}

void FileUtil::UpdateStringData(const char* CategoryName, const char* DataName, const char* Value) {
	WriteStringData(FindCategory(CategoryName), DataName, Value);
	UpdateDataFile();
}

int FileUtil::LoadIntData(const char* CategoryName, const char* DataName) {
	return static_cast<int>(GetIntData(FindCategory(CategoryName), DataName));
}

float FileUtil::LoadFloatData(const char* CategoryName, const char* DataName) {
	return GetFloatData(FindCategory(CategoryName), DataName);
}

const char* FileUtil::LoadStringData(const char* CategoryName, const char* DataName) {
	return GetStringData(FindCategory(CategoryName), DataName);
}

void FileUtil::ResetData() {
	Doc.Clear();
	SetupData(DataListBuffer);
}


//////////////////////////////// private
void FileUtil::SetupData(std::vector<FileData> List) {
	if (!FileExist) {
		CreateDec(APPLICATION_VERSION);
		AddRoot("Data");
	}

	for (auto const& D : List) {
		if(!FindCategory(D.CategoryName))
			AddCategory(D.CategoryName);
	}

	for (auto const& D : List) {
		if (!FindData(D.CategoryName, D.DataName)) {
			switch (D.DType) {
			case DataType::Int:
				AddIntData(D.CategoryName, D.DataName, D.IValue);
				break;

			case DataType::Float:
				AddFloatData(D.CategoryName, D.DataName, D.FValue);
				break;

			case DataType::String:
				AddStringData(D.CategoryName, D.DataName, D.StringValue);
				break;
			}
		}
	}

	UpdateDataFile();

	if (!FileExist) {
		std::cout << "Created new data file: " << FilePathStr << std::endl;
		FileExist = true;
	}
}

void FileUtil::CheckDataVersion() {
	TiXmlNode* DeclNode = Doc.FirstChild();
	TiXmlDeclaration* Decl = DeclNode->ToDeclaration();

	if (Decl) {
		const char* Version = Decl->Version();
		if (std::stof(Version) < APPLICATION_VERSION) {
			UpdateDataVersion(APPLICATION_VERSION);
		}
	}
	else {
		std::cout << "Failed to find data version" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void FileUtil::UpdateDataVersion(float VersionValue) {
	TiXmlNode* DeclNode = Doc.FirstChild();
	TiXmlDeclaration* Decl = DeclNode->ToDeclaration();

	const char* Version = Decl->Version();
	const char* Encoding = Decl->Encoding();
	const char* StandAlone = Decl->Standalone();

	if (std::stof(Version) < VersionValue) {
		std::ostringstream OSS;
		OSS << std::fixed << std::setprecision(1) << VersionValue;
		std::string NewVersionStr = OSS.str();

		TiXmlDeclaration NewDecl(NewVersionStr.c_str(), Encoding ? Encoding : "", StandAlone ? StandAlone : "");

		Doc.ReplaceChild(DeclNode, NewDecl);

		SetupData(DataListBuffer);
	}
}

void FileUtil::CreateDec(float VersionValue) {
	std::ostringstream OSS;
	OSS << std::fixed << std::setprecision(1) << VersionValue;
	std::string NewVersionStr = OSS.str();
	Doc.LinkEndChild(new TiXmlDeclaration(NewVersionStr.c_str(), "", ""));
}

void FileUtil::AddRoot(const char* RootName) {
	Doc.LinkEndChild(new TiXmlElement(RootName));
}

void FileUtil::AddCategory(const char* CategoryName) {
	FindRoot()->LinkEndChild(new TiXmlElement(CategoryName));
}

void FileUtil::AddFloatData(const char* CategoryName, const char* DataName, float Value) {
	FindCategory(CategoryName)->SetDoubleAttribute(DataName, Value);
}

void FileUtil::AddIntData(const char* CategoryName, const char* DataName, int Value) {
	FindCategory(CategoryName)->SetDoubleAttribute(DataName, Value);
}

void FileUtil::AddStringData(const char* CategoryName, const char* DataName, const char* Value) {
	FindCategory(CategoryName)->SetAttribute(DataName, Value);
}

void FileUtil::WriteIntData(TiXmlElement* CategoryVar, const char* DataName, int Value) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		CategoryVar->SetAttribute(DataName, Value);
	else {
		std::cout << "Failed to update data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void FileUtil::WriteFloatData(TiXmlElement* CategoryVar, const char* DataName, float Value) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		CategoryVar->SetDoubleAttribute(DataName, Value);
	else {
		std::cout << "Failed to update data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void FileUtil::WriteStringData(TiXmlElement* CategoryVar, const char* DataName, const char* Value) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		CategoryVar->SetAttribute(DataName, Value);
	else {
		std::cout << "Failed to update string data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int FileUtil::GetIntData(TiXmlElement* CategoryVar, const char* DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		return std::stoi(DataValue);
	else {
		std::cout << "Failed to find data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

float FileUtil::GetFloatData(TiXmlElement* CategoryVar, const char* DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		return std::stof(DataValue);
	else {
		std::cout << "Failed to find data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

const char* FileUtil::GetStringData(TiXmlElement* CategoryVar, const char* DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		return DataValue;
	else {
		std::cout << "Failed to find data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

TiXmlElement* FileUtil::FindRoot() {
	return Doc.RootElement();
}

TiXmlElement* FileUtil::FindCategory(const char* CategoryName) {
	return FindRoot()->FirstChildElement(CategoryName);
}

const char* FileUtil::FindData(const char* CategoryName, const char* DataName) {
	return FindCategory(CategoryName)->Attribute(DataName);
}

bool FileUtil::LoadDataFile(const char* FileName) {
	if (FType == FileType::Secure) {
		std::ifstream EncryptedFile(FileName, std::ios::binary);
		if (!EncryptedFile)
			return false;

		std::string EncryptedContent((std::istreambuf_iterator<char>(EncryptedFile)), std::istreambuf_iterator<char>());
		EncryptedFile.close();

		std::string DecryptedXML = Decrypt(EncryptedContent, AES_KEY, IV_KEY);
		Doc.Parse(DecryptedXML.c_str());

		if (Doc.Error()) {
			std::cout << "Failed to parse data file" << std::endl;
			exit(EXIT_FAILURE);
		}
		std::cout << "File Util Opened file: " << FilePathStr << std::endl;

		return true;
	}

	else if (FType == FileType::Script)
		return Doc.LoadFile(FileName);
}

void FileUtil::UpdateDataFile() {
	if (FType == FileType::Secure) {
		TiXmlPrinter Printer;
		Doc.Accept(&Printer);

		std::string XML_String = Printer.CStr();
		std::string EncryptedXML = Encrypt(XML_String, AES_KEY, IV_KEY);

		std::ofstream ExportFile(FilePathStr, std::ios::binary);
		ExportFile.write(EncryptedXML.c_str(), EncryptedXML.size());
		ExportFile.close();

		Doc.Clear();
		LoadDataFile(FilePathStr.c_str());
	}

	else if (FType == FileType::Script) {
		Doc.SaveFile(FilePathStr.c_str());
	}
}

std::string FileUtil::Encrypt(const std::string& plainText, const byte key[], const byte iv[]) {
	std::string CipherText;

	try {
		CBC_Mode<AES>::Encryption Encryption;
		Encryption.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
		StringSource(plainText, true, new StreamTransformationFilter(Encryption, new StringSink(CipherText)));
	}
	catch (const Exception& E) {
		std::cerr << E.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	return CipherText;
}

std::string FileUtil::Decrypt(const std::string& cipherText, const byte key[], const byte iv[]) {
	std::string PlainText;

	try {
		CBC_Mode<AES>::Decryption Decryption;
		Decryption.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
		StringSource(cipherText, true, new StreamTransformationFilter(Decryption, new StringSink(PlainText)));
	}
	catch (const Exception& E) {
		std::cerr << E.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	return PlainText;
}