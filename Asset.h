#pragma once
#ifndef __Asset
#define __Asset
#include "GL.h"

class AssetData {
public:
	vector<vec3> vertexSet;

	vector<ivec3> indexSet;
};

class Asset {
public:
	string fileName;
	char* srcBIN;

	Asset(string fileName);

	char* ReadBinaryFile(string filePath, int size);

	void LoadAsset();
};

#endif