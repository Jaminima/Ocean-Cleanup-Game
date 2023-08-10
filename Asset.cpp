#include "Asset.h"
#include <fstream>
#include <sstream>
#include "dependencies/simdjson.h"

using namespace simdjson;

const string fileDir = "./assets/";

Asset::Asset(string fileName)
{
	this->fileName = fileName;
}

void Asset::LoadAsset()
{
	string filePath = fileDir + this->fileName + "/" + this->fileName;

	std::ifstream input(filePath + ".bin", std::ios::binary);

	std::vector<char> bytes(
		(std::istreambuf_iterator<char>(input)),
		(std::istreambuf_iterator<char>()));

	input.close();

	ondemand::parser parser;
	padded_string json = padded_string::load(filePath + ".gltf");
	ondemand::document gltfJson = parser.iterate(json);

	auto bufferViews = gltfJson.find_field_unordered("bufferViews");
	auto bufferViews_array = bufferViews.get_array();
	int bufferViews_count = bufferViews_array.count_elements();

	for (int i = 0; i < bufferViews_count; i++) {
		auto t_bufferView = bufferViews_array.at(i);

		int buff = t_bufferView.find_field("buffer").get_int64();
		int buffLen = t_bufferView.find_field("byteLength").get_int64();
		int buffOffset = t_bufferView.find_field("byteOffset").get_int64();

		char* d = new char[buffLen];
		memcpy(d,&bytes.data()[buffOffset], buffLen);
	}
}
