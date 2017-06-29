#include "MeshLoader.h"

using namespace Pro;
using namespace Graphics;
using namespace Util;

template<typename T> inline void LoadHeightVerticies(
	const T* map_array,
	BufferWriter& verticies,
	unsigned stride,
	unsigned offset,
	const Vector3<float>& delta,
	const Vector2<unsigned>& dim) {
	// A BufferReader doesn't support the type of access we need from the texture  
	for (unsigned y = 0; y < dim.y; ++y) {
		for (unsigned x = 0; x < dim.x; ++x) {
			verticies.Write<Vector3<float>>({ x * delta.x, *(map_array + offset) * delta.z, y * delta.y });
			map_array += stride / sizeof(T); // Skip over the other channel data 
		}
	}
}

std::shared_ptr<Mesh> MeshLoader::LoadMeshFromHeightMap(const Util::Buffer &map_data, const Math::Vector2<unsigned>& map_dimensions, unsigned stride, unsigned offset, unsigned size) {

	unsigned width = map_dimensions.x;
	unsigned height = map_dimensions.y;

	if (width == 0 || height == 0) {
		global_log.Report<LogCode::FAULT>("Unable to load heightmap, {0,0} map_dimensions", __FUNCTION__, __LINE__);
		return nullptr;
	}

	if (map_data.Empty()) {
		global_log.Report<LogCode::FAULT>("Unable to load heightmap, empty map_data", __FUNCTION__, __LINE__);
		return nullptr;
	}

	if (width * height * size >= map_data.size()) {
		global_log.Report<LogCode::FAULT>("Size mismatch between dimensions and buffer, Expected: " +
			std::to_string(width * height * size) +
			", Received: " + std::to_string(map_data.size()),
			__FUNCTION__, __LINE__);
		return nullptr;
	}

	// A rough estimation on the map size
	Util::Buffer mesh_data(map_data.size() * sizeof(Vector3<float>));
	Util::Buffer element_data(map_data.size() * sizeof(unsigned));
	BufferWriter verticies(&mesh_data);
	BufferWriter elements(&element_data);

	Vector3<float> delta({
		1.f / map_dimensions.x,
		1.f / map_dimensions.y,
		1.f / powf(2.f, (float)size)
	});


	switch (size) {
	case 1:
		LoadHeightVerticies<char>(map_data.data<char>(), verticies, stride, offset, delta, map_dimensions);
		break;
	case 2:
		LoadHeightVerticies<short>(map_data.data<short>(), verticies, stride, offset, delta, map_dimensions);
		break;
	case 4:
		LoadHeightVerticies<unsigned>(map_data.data<unsigned>(), verticies, stride, offset, delta, map_dimensions);
		break;
	default:
		global_log.Report<LogCode::FAULT>("Unsupported stride size", __FUNCTION__, __LINE__);
	}

	for (unsigned y = 0; y < height - 1; y += 2) {
		int pos = y * width;
		for (unsigned x = 0; x < width; ++x, ++pos) {
			elements.Write<unsigned>(pos >= 16 ? 15 : pos);
			elements.Write<unsigned>(pos + width >= 16 ? 16 : pos + width);
		}
		pos += width;
		for (unsigned x = width; x != 0; --x, --pos) {
			elements.Write<unsigned>(pos >= 16 ? 15 : pos);
			elements.Write<unsigned>(pos - width);
		}
	}

	// Create VBO in OpenGL

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, verticies.head(), mesh_data.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		global_log.Report<LogCode::FAULT>("Unable to load OBJ Model: Create Array Buffer", __FUNCTION__, __LINE__);
		return nullptr;
	}

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.head(), element_data.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	err = glGetError();
	if (err != GL_NO_ERROR) {
		global_log.Report<LogCode::FAULT>("Unable to load OBJ Model: Create Element Array, " + glGetErrorString(err), __FUNCTION__, __LINE__);
		return nullptr;
	}

	// TODO Check the UML of Mesh and MeshObject to see what's duplicated
	MeshObject obj;
	obj.vertex_per_face = 3;
	obj.floats_per_vertex = 3;
	obj.face_format = FACE_FORMAT::VERTEX;
	obj.size = static_cast<GLuint>(elements.head() / sizeof(unsigned));
	obj.start = 0;
	obj.name = "HeightMap";

	std::shared_ptr<Mesh> m = std::make_shared<Mesh>(vbo, ebo);
	m->AttachObject(std::move(obj));
	return std::move(m);
}