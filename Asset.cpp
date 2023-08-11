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

char* Asset::ReadBinaryFile(string filePath, int size)
{
	std::ifstream input(filePath, std::ios::binary);

	char* bytes = new char[size];

	input.read(bytes, size);

	input.close();

	return bytes;
}

void Asset::LoadAsset()
{
	string filePath = fileDir + this->fileName + "/";

	ondemand::parser parser;
	padded_string json = padded_string::load(filePath + this->fileName + ".gltf");
	ondemand::document gltfJson = parser.iterate(json);

	//Load Buffer Files
	vector<char*> buffers;

	auto jBuffersArray = gltfJson.find_field("buffers").get_array();
	int jBuffersLen = jBuffersArray.count_elements();

	for (int i = 0; i < jBuffersLen; i++) {
		auto jBuffer = jBuffersArray.at(i);
		buffers.push_back(
			ReadBinaryFile(
				filePath + (string)jBuffer.find_field("uri").get_string().value(),
				jBuffer.find_field("byteLength").get_int64()
			));
	}

	//Construct Buffer Views
	vector<char*> bufferViews;

	auto jBufferViewsArray = gltfJson.find_field_unordered("bufferViews").get_array();

	for (auto jBufferView : jBufferViewsArray) {
		int buff = jBufferView.find_field("buffer").get_int64();
		int buffLen = jBufferView.find_field("byteLength").get_int64();
		int buffOffset = jBufferView.find_field("byteOffset").get_int64();

		bufferViews.push_back(&buffers[buff][buffOffset]);
	}
}
