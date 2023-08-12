#include "Asset.h"
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

class BufferView {
public:
	char* buff;
	int byteStride = 0;
};

enum ComponentType {
	_int,
	_short
};

class BufferAccessors {
public:
	BufferView* bufferView;
	int byteOffset;
	int size;
	int componentType;

	void* getValue(int idx, size_t typeSize) {
		return &bufferView->buff[(idx * typeSize/* * bufferView->byteStride*/) + byteOffset];
	}

	ComponentType getType() {
		switch (componentType)
		{
		case 5125:
			return _int;
		case 5123:
			return _short;
		}
	}
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
		string fName = filePath + (string)jBuffer.find_field_unordered("uri").get_string().value();
		int fLen = jBuffer.find_field_unordered("byteLength").get_int64();
		buffers.push_back(
			ReadBinaryFile(fName, fLen));
	}

	//Construct Buffer Views
	vector<BufferView*> bufferViews;

	auto jBufferViewsArray = gltfJson.find_field_unordered("bufferViews").get_array();

	for (auto jBufferView : jBufferViewsArray) {
		int buff = jBufferView.find_field_unordered("buffer").get_int64();
		int buffLen = jBufferView.find_field_unordered("byteLength").get_int64();

		auto buffOff = jBufferView.find_field_unordered("byteOffset");
		int buffOffset = buffOff.error() ? 0 : buffOff.get_int64().value();

		auto buffStri = jBufferView.find_field_unordered("byteStride");
		int buffStride = buffStri.error() ? 0 : buffStri.get_int64().value();

		auto buffView = new BufferView();
		buffView->buff = &buffers[buff][buffOffset];
		buffView->byteStride = buffStride;

		bufferViews.push_back(buffView);
	}

	//Construct Accessors
	vector<BufferAccessors*> accessors;

	auto jAccessorsArray = gltfJson.find_field_unordered("accessors").get_array();

	for (auto accessor : jAccessorsArray) {
		auto bufferIdx = accessor.find_field_unordered("bufferView").get_int64().value();
		auto bufferSize = accessor.find_field_unordered("count").get_int64().value();
		auto bufferType = accessor.find_field_unordered("componentType").get_int64().value();

		auto buffOff = accessor.find_field_unordered("byteOffset");
		int bufferOffset = buffOff.error() ? 0 : buffOff.get_int64().value();

		auto bufferView = bufferViews[bufferIdx];

		auto buff = new BufferAccessors();

		buff->componentType = bufferType;
		buff->byteOffset = bufferOffset;
		buff->bufferView = bufferView;
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
		auto idxType = idxBuff->getType();

		auto posBuff = accessors[posAccessor];
		auto normBuff = accessors[normAccessor];
		auto texBuff = _tex.error() ? 0x0 : accessors[texAccessor];

		//for (int i = 0; i < idxBuff->size; i++) {
		for (int i = idxBuff->size - 1; i >= 0; i--) {
			int idx = idxType == _short ? ((uint16*)idxBuff->bufferView->buff)[i] : ((int*)idxBuff->bufferView->buff)[i];

			m->vertexData.push_back((*(vec3*)posBuff->getValue(idx, sizeof(vec3))));
			m->normalData.push_back((*(vec3*)normBuff->getValue(idx, sizeof(vec3))));

			if (!_tex.error())
				m->texCooData.push_back((*(vec2*)texBuff->getValue(idx, sizeof(vec2))));
		}

		meshes->push_back(m);
	}
}

void Asset::Build()
{
	for (auto m : *this->meshes) {
		m->Build();
	}
	this->Update();
}

void Asset::Render(GLuint programHandle, SceneObjects* sceneObjects)
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

	if (textures.size() > 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glUniform1i(glGetUniformLocation(programHandle, "faceTexture"), 0);
		glUniform1i(glGetUniformLocation(programHandle, "hasTexture"), 1);
	}
	else {
		glUniform1i(glGetUniformLocation(programHandle, "hasTexture"), 0);
	}

	glUniform1i(glGetUniformLocation(programHandle, "hovered"), state.hovered);

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

const int ColourChanels[]{ 0 , GL_R, GL_RG, GL_RGB, GL_RGBA };

void loadTexture(GLuint& texture, std::string texturepath)
{
	// load and create a texture
// -------------------------

// texture 1
// ---------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	GLint width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(texturepath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, ColourChanels[nrChannels], GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Asset::AddTexture(string filePath)
{
	GLuint tex = 0;

	loadTexture(tex, fileDir + this->fileName + "/" + filePath);

	textures.push_back(tex);
}

void Asset::Update()
{
	vec3 min = vec3(), max = vec3();
	for (auto m : *meshes) {
		for (auto v : m->vertexData) {
			for (int i = 0; i < 3; i++) {
				if (v[i] > max[i])
					max[i] = v[i];

				if (v[i] < min[i])
					min[i] = v[i];
			}
		}
	}

	auto rot = mat4(1);

	rot = glm::rotate(rot, rotation[0], vec3(1.0f, 0.0f, 0.0f));
	rot = glm::rotate(rot, rotation[1], vec3(0.0f, 1.0f, 1.0f));
	rot = glm::rotate(rot, rotation[2], vec3(0.0f, 0.0f, 1.0f));

	min = vec4(min, 1) * rot;
	max = vec4(max, 1) * rot;

	boundMin = (min * scale) + position;
	boundMax = (max * scale) + position;
}

bool Asset::BeamCollides(vec3 origin, vec3 dir)
{
	dir = -normalize(dir);

	if (all(greaterThanEqual(boundMin, origin)) && all(lessThanEqual(boundMax, origin)))
		return true;

	auto dirTo1 = normalize(origin - boundMin);
	auto dirTo2 = normalize(origin - boundMax);

	/*if (all(greaterThanEqual(dirToMin, dir)) && all(lessThanEqual(dirToMax, dir)))
		return true;*/

	const float sizeAdjust = 0.8f;
	int outOfBounds = 0;

	for (int i = 0; i < 3; i++) {
		if (dirTo1[i] > dirTo2[i]) {
			if (dirTo1[i] * sizeAdjust < dir[i] || dirTo2[i] * sizeAdjust > dir[i])
				outOfBounds++;
		}
		else {
			if (dirTo2[i] * sizeAdjust < dir[i] || dirTo1[i] * sizeAdjust > dir[i])
				outOfBounds++;
		}
	}

	return outOfBounds < 2;
}