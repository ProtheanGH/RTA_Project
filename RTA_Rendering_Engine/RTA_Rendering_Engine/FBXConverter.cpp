#include "FBXConverter.h"

FBXConverter::FBXConverter(){
	fbxManager = FbxManager::Create();
}

FBXConverter::~FBXConverter(){
	fbxManager->Destroy();
}

void FBXConverter::LoadFBX(const char* _fileName, Object* _rootObject){

	if (fbxManager == nullptr){
		fbxManager = FbxManager::Create();
		FbxIOSettings* ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
		fbxManager->SetIOSettings(ioSettings);
	}

	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "");

	bool result = fbxImporter->Initialize(_fileName, -1, fbxManager->GetIOSettings());

	if (!result) return;

	result = fbxImporter->Import(fbxScene);

	if (!result) return;

	fbxImporter->Destroy();

	FbxNode* rootNode = fbxScene->GetRootNode();

	if (rootNode){
		LoadFBX(rootNode, _rootObject);
	}

}

void FBXConverter::LoadFBX(FbxNode* _rootNode, Object* _rootObject){

	int child_count = _rootNode->GetChildCount();

	//Transform& root_transform = _rootObject->GetTransform();
	//FbxDouble3 translation = _rootNode->LclTranslation.Get();
	//FbxDouble3 rotation = _rootNode->LclRotation.Get();
	//FbxDouble3 scale = _rootNode->LclScaling.Get();

	for (int i = 0; i < child_count; ++i){
		FbxNode* child_node = _rootNode->GetChild(i);

		if (child_node->GetNodeAttribute() == NULL) continue;

		FbxNodeAttribute::EType attributeType = child_node->GetNodeAttribute()->GetAttributeType();
		if (attributeType != FbxNodeAttribute::eMesh) continue;

		Object* child_obj = Object::Create();
		LoadFBX(child_node, child_obj);

		FbxMesh* mesh = (FbxMesh*)child_node->GetNodeAttribute();
		LoadMesh(mesh, child_obj);

		child_obj->SetParent(_rootObject);
		_rootObject->GetChildren().push_back(child_obj);
	}
}

void FBXConverter::LoadMesh(FbxMesh* _mesh, Object* _object){

	FbxVector4* vertices = _mesh->GetControlPoints();
	unsigned int vertexCount = 0;
	Mesh* object_mesh = new Mesh();
	_object->SetMesh(object_mesh);

	//Polygons are triangles
	for (int i = 0; i < _mesh->GetPolygonCount(); ++i){
		int numVertices = _mesh->GetPolygonSize(i);
		if (numVertices < 3) continue;

		for (int j = 0; j < 3; ++j){
			int controlPointIndex = _mesh->GetPolygonVertex(i, j);
			Vertex_POSNORMUV vertex;
			vertex.pos[0] = (float)vertices[controlPointIndex].mData[0];
			vertex.pos[1] = (float)vertices[controlPointIndex].mData[1];
			vertex.pos[2] = (float)vertices[controlPointIndex].mData[2];

			DirectX::XMFLOAT3 normal;
			LoadNormal(_mesh, controlPointIndex, vertexCount, normal);
			vertex.normal[0] = normal.x;
			vertex.normal[1] = normal.y;
			vertex.normal[2] = normal.z;

			DirectX::XMFLOAT2 uv;
			LoadUV(_mesh, controlPointIndex, _mesh->GetTextureUVIndex(i, j), uv);
			vertex.uv[0] = uv.x;
			vertex.uv[1] = uv.y;

			object_mesh->GetVerts().push_back(vertex);

			++vertexCount;
		}
	}

	//Vertices are indices
	int* indices = _mesh->GetPolygonVertices();
	for (int i = 0; i < _mesh->GetPolygonVertexCount(); ++i){
		int index = indices[i];
		object_mesh->GetIndices().push_back(index);
	}

}

void FBXConverter::LoadNormal(FbxMesh* _mesh, int _controlPointIndex, int _vertexCounter, DirectX::XMFLOAT3& _outNormal){

	if (_mesh->GetElementNormalCount() < 1) return;

	FbxGeometryElementNormal* vertex_normal = _mesh->GetElementNormal();

	if (vertex_normal->GetMappingMode() == FbxGeometryElement::eByControlPoint){
		if (vertex_normal->GetReferenceMode() == FbxGeometryElement::eDirect){
			_outNormal.x = (float)(vertex_normal->GetDirectArray().GetAt(_controlPointIndex).mData[0]);
			_outNormal.y = (float)(vertex_normal->GetDirectArray().GetAt(_controlPointIndex).mData[1]);
			_outNormal.z = (float)(vertex_normal->GetDirectArray().GetAt(_controlPointIndex).mData[2]);
		}
		else if (vertex_normal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect){
			int index = vertex_normal->GetIndexArray().GetAt(_controlPointIndex);
			_outNormal.x = (float)(vertex_normal->GetDirectArray().GetAt(index).mData[0]);
			_outNormal.y = (float)(vertex_normal->GetDirectArray().GetAt(index).mData[1]);
			_outNormal.z = (float)(vertex_normal->GetDirectArray().GetAt(index).mData[2]);
		}
	}
	else if (vertex_normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex){
		if (vertex_normal->GetReferenceMode() == FbxGeometryElement::eDirect){
			_outNormal.x = (float)(vertex_normal->GetDirectArray().GetAt(_vertexCounter).mData[0]);
			_outNormal.y = (float)(vertex_normal->GetDirectArray().GetAt(_vertexCounter).mData[1]);
			_outNormal.z = (float)(vertex_normal->GetDirectArray().GetAt(_vertexCounter).mData[2]);
		}
		else if (vertex_normal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect){
			int index = vertex_normal->GetIndexArray().GetAt(_vertexCounter);
			_outNormal.x = (float)(vertex_normal->GetDirectArray().GetAt(index).mData[0]);
			_outNormal.y = (float)(vertex_normal->GetDirectArray().GetAt(index).mData[1]);
			_outNormal.z = (float)(vertex_normal->GetDirectArray().GetAt(index).mData[2]);
		}
	}
}

void FBXConverter::LoadUV(FbxMesh* _mesh, int _controlPointIndex, int _textureUVIndex, DirectX::XMFLOAT2& _outUV){

	if (_mesh->GetElementUVCount() < 1) return;

	FbxGeometryElementUV* vertex_uv = _mesh->GetElementUV();

	if (vertex_uv->GetMappingMode() == FbxGeometryElement::eByControlPoint){
		if (vertex_uv->GetReferenceMode() == FbxGeometryElement::eDirect){
			_outUV.x = (float)vertex_uv->GetDirectArray().GetAt(_controlPointIndex).mData[0];
			_outUV.y = (float)vertex_uv->GetDirectArray().GetAt(_controlPointIndex).mData[1];

		}
		else if (vertex_uv->GetReferenceMode() == FbxGeometryElement::eIndexToDirect){
			int index = vertex_uv->GetIndexArray().GetAt(_controlPointIndex);
			_outUV.x = (float)vertex_uv->GetDirectArray().GetAt(index).mData[0];
			_outUV.y = (float)vertex_uv->GetDirectArray().GetAt(index).mData[1];
		}
	}
	else if (vertex_uv->GetMappingMode() == FbxGeometryElement::eByPolygonVertex){
		if (vertex_uv->GetReferenceMode() == FbxGeometryElement::eIndexToDirect){
			_outUV.x = (float)vertex_uv->GetDirectArray().GetAt(_textureUVIndex).mData[0];
			_outUV.y = (float)vertex_uv->GetDirectArray().GetAt(_textureUVIndex).mData[1];
		}
	}
}


void FBXConverter::SaveObject(const char* _fileName, Object& _object){

}

