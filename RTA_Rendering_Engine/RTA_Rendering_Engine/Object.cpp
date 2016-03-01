#include "Object.h"

Object* Object::Create(){

	return new Object();
}

void Object::Destroy(){
	for (unsigned int i = 0; i < children.size(); ++i){
		children[i]->Destroy();
	}

	delete mesh;

	delete this;
}