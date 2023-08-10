#pragma once
#include "GL.h"


class AssetData {
public:
	vector<vec3> vertexSet;

	vector<ivec3> indexSet;
};

class Asset {
public:
	string fileName;
	string* srcGLTF;
	string* srcBIN;

	string* readFile(string file);

	void LoadAsset();

	void ParseBlock(int strIdx = 0, int objectDepth = 0, int arrayIdx = 0);
};