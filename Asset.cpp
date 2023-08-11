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
		auto bufferSize = accessor.find_field_unordered("count").get_int64().value();

		auto bufferView = &bufferViews[bufferIdx][bufferOffset];

		auto buff = new BufferAccessors();

		buff->buff = bufferView;
		buff->size = bufferSize;

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

		//for (int i = 0; i < idxBuff->size; i++) {
		for (int i=idxBuff->size-1;i>=0;i--){
			int idx = idxBuff->buff[i];

			m->vertexData.push_back(((vec3*)posBuff->buff)[idx]);
			m->normalData.push_back(((vec3*)normBuff->buff)[idx]);

			if (!_tex.error())
				m->texCooData.push_back(((vec2*)texBuff->buff)[idx]);
		}

		meshes->push_back(m);
	}
}

void Asset::Build()
{
	for (auto m : *this->meshes) {
		m->Build();
	}
}

void Asset::Render(GLuint programHandle,SceneObjects* sceneObjects)
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, vec3(position));

	model = glm::rotate(model, rotation[0], vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, rotation[1], vec3(0.0f, 1.0f, 1.0f));
	model = glm::rotate(model, rotation[2], vec3(0.0f, 0.0f, 1.0f));

	model = glm::scale(model, scale);

	GLuint modelRef = glGetUniformLocation(programHandle, "ModelMatrix");
	glUniformMatrix4fv(modelRef, 1, GL_FALSE, glm::value_ptr(model));

	GLuint viewRef = glGetUniformLocation(programHandle, "ViewMatrix");
	glUniformMatrix4fv(viewRef, 1, GL_FALSE, glm::value_ptr(sceneObjects->cam.viewMatrix));

	GLuint projRef = glGetUniformLocation(programHandle, "ProjectionMatrix");
	glUniformMatrix4fv(projRef, 1, GL_FALSE, glm::value_ptr(sceneObjects->cam.projectionMatrix));

	glm::mat4 mv = sceneObjects->cam.viewMatrix * model;

	GLuint modelViewRef = glGetUniformLocation(programHandle, "ModelViewMatrix");
	glUniformMatrix4fv(modelViewRef, 1, GL_FALSE, glm::value_ptr(mv));

	GLuint normRef = glGetUniformLocation(programHandle, "NormalMatrix");
	glm::mat3 normalMatrix = mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2]));
	glUniformMatrix3fv(normRef, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	for (auto m : *this->meshes) {
		m->Render();
	}
}

Mesh::Mesh(string name)
{
	this->name = name;
}

void Mesh::Build()
{
	glGenVertexArrays(1, &this->vaoBuffer);
	glBindVertexArray(this->vaoBuffer);

	glGenBuffers(bufferCount, this->buffers);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, this->vertexData.size() * sizeof(vec3), this->vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, this->normalData.size() * sizeof(vec3), this->normalData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, this->texCooData.size() * sizeof(vec2), this->texCooData.data(), GL_STATIC_DRAW);

	glBindVertexBuffer(0, this->buffers[0], 0, sizeof(vec3));
	glBindVertexBuffer(1, this->buffers[1], 0, sizeof(vec3));
	glBindVertexBuffer(2, this->buffers[2], 0, sizeof(vec2));

	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 0);

	glVertexAttribBinding(0, 0);
	glVertexAttribBinding(1, 1);
	glVertexAttribBinding(2, 2);

	glBindVertexArray(0);
}

void Mesh::Render()
{
	glBindVertexArray(this->vaoBuffer);
	glDrawArrays(GL_TRIANGLES, 0, this->vertexData.size());

	glBindVertexArray(0);
}