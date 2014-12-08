#include "MeshObject.h"

using namespace Pro;

MeshObject::MeshObject(MeshObject&& rhs) {
	name = std::move(rhs.name);
	start = rhs.start;
	size = rhs.size;
}
MeshObject::MeshObject() {
	name = "";
	start = 0;
	size = 0;
}

MeshObject::~MeshObject() {
}