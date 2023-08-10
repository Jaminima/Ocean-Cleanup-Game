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

	void LoadAsset();
};

#endif