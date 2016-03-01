#pragma once

#include "Transform.h"
#include "Mesh.h"

class Object{

protected:
	Object() = default;
	~Object() = default;

private:
	Transform transform;
	Mesh* mesh;
	Object* parent;
	std::vector<Object*> children;

public:

	inline Transform& GetTransform()  { return transform; }
	inline std::vector<Object*>& GetChildren() { return children; }

	inline Mesh* GetMesh() { return mesh; }
	inline void SetMesh(Mesh* _mesh){ mesh = _mesh; }

	inline Object* GetParent(){ return parent; }
	inline void SetParent(Object* _parent){ parent = _parent; }

	void Destroy();

	static Object* Create();
};