#include "Asset.h"
#include <fstream>
#include <sstream>

const string fileDir = "./assets/";

string* Asset::readFile(string fileName)
{
	ifstream file(fileDir + this->fileName);
	string str;
	if (file) {
		ostringstream ss;
		ss << file.rdbuf(); // reading data
		str = ss.str();
	}

	return new string(str);
}

void Asset::LoadAsset()
{
	this->srcGLTF = readFile(this->fileName + ".gltf");
	this->srcBIN = readFile(this->fileName + ".bin");

}
