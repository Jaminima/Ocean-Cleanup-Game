#include "Asset.h"
#include <fstream>
#include <sstream>
#include "dependencies/simdjson.h"

using namespace simdjson;

const string fileDir = "./assets/";

Asset::Asset(string fileName)
{
	this->fileName = fileName;
	this->meshes = new vector<Mesh*>();
}

char* Asset::ReadBinaryFile(string filePath, int size)
{
	std::ifstream input(filePath, std::ios::binary);

	char* bytes = new char[size];

	input.read(bytes, size);

	input.close();

	return bytes;
}

struct BufferAccessors {
public:
	char* buff;
	int size;
};

void Asset::LoadAsset()
{
	string filePath = fileDir + this->fileName + "/";

	ondemand::parser parser;
	padded_string json = padded_string::load(filePath + this->fileName + ".gltf");
	ondemand::document gltfJson = parser.iterate(json);

	//Load Buffer Files
	vector<char*> buffers;

	auto jBuffersArray = gltfJson.find_field("buffers").get_array();

	for (auto jBuffer : jBuffersArray) {
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
		int buff = jBufferView.find_field_unordered("buffer").get_int64();
		int buffLen = jBufferView.find_field_unordered("byteLength").get_int64();
		int buffOffset = jBufferView.find_field_unordered("byteOffset").get_int64();

		bufferViews.push_back(&buffers[buff][buffOffset]);
	}

	//Construct Accessors
	vector<BufferAccessors*> accessors;

	auto jAccessorsArray = gltfJson.find_field_unordered("accessors").get_array();

	for (auto accessor : jAccessorsArray) {
		auto bufferIdx = accessor.find_field_unordered("bufferView").get_int64().value();
		auto bufferOffset = accessor.find_field_unordered("byteOffset").get_int64().value();
		auto bufferCount = accessor.find_field_unordered("count").get_int64().value();

		auto bufferView = &bufferViews[bufferIdx][bufferOffset];

		auto buff = new BufferAccessors();

		buff->buff = bufferView;
		buff->size = bufferCount;

		accessors.push_back(buff);
	}

	//Construct Meshes
	auto jMeshArray = gltfJson.find_field_unordered("meshes").get_array();

	for (auto jMesh : jMeshArray) {
		auto m = new Mesh((string)jMesh.find_field("name").get_string().value());

		auto prim = jMesh.find_field_unordered("primitives").at(0);
		int idxAccessor = prim.find_field_unordered("indices").get_int64();

		auto attr = prim.find_field_unordered("attributes");
		int posAccessor = attr.find_field_unordered("POSITION").get_int64();
		int normAccessor = attr.find_field_unordered("NORMAL").get_int64();

		auto _tex = attr.find_field_unordered("TEXCOORD_0");
		int texAccessor = _tex.error() ? -1 : _tex.get_int64().value();

		//Build Mesh

		auto idxBuff = accessors[idxAccessor];

		auto posBuff = accessors[posAccessor];
		auto normBuff = accessors[normAccessor];
		auto texBuff = _tex.error() ? 0x0 : accessors[texAccessor];

		for (int i = 0; i < idxBuff->size; i++) {
			int idx = idxBuff->buff[i];

			m->vertexData.push_back(((vec3*)posBuff->buff)[idx]);
			m->normalData.push_back(((vec3*)normBuff->buff)[idx]);

			if (!_tex.error())
				m->texCooData.push_back(((vec2*)texBuff->buff)[idx]);
		}

		meshes->push_back(m);
	}
}

Mesh::Mesh(string name)
{
	this->name = name;
}
