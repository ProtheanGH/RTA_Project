#pragma once

#include <fbxsdk.h>
#include "Object.h"

class FBXConverter{

private:
	FbxManager* fbxManager = nullptr;

	void LoadFBX(FbxNode* _rootNode, Object* rootObject);

	//Load the fbx data into the objects mesh
	void LoadMesh(FbxMesh* _mesh, Object* object);

	void LoadNormal(FbxMesh* _mesh, int _controlPointIndex, int _vertexCounter, DirectX::XMFLOAT3& _outNormal);

public:
	FBXConverter();
	~FBXConverter();
	
	void LoadFBX(const char* fileName, Object* object);

	//Saves the object as a binary file 
	void SaveObject(const char* fileName, Object& object);
};