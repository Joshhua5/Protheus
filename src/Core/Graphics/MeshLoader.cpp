#include "MeshLoader.h"
#include "extern/lib3ds/lib3ds.h" 

using namespace Pro;
using namespace Graphics;
using namespace Util;
using namespace IO;

MODEL_FORMAT MeshLoader::QueryFormat(const Buffer& buffer) {

	if (buffer.Empty())
		return MODEL_FORMAT::UNDEFINED;

	if (buffer.size() > 4 &&
		buffer.data<char>()[0] == 'M' &&
		buffer.data<char>()[1] == 'M')
		return MODEL_FORMAT::_3DS;
	

	//Check if type is OBJ
	//Scan for a line with "v "
	for (unsigned x = 0; x < buffer.size(); x++)
		if (buffer.data<char>()[x] == 'v' && buffer.data<char>()[x + 1] == ' ')
			return MODEL_FORMAT::OBJ;  
		 
	return MODEL_FORMAT::UNDEFINED;
} 

std::shared_ptr<Mesh> MeshLoader::LoadModel(Buffer&& buffer) {
    Buffer temp(std::move(buffer));
    return LoadModel(temp);
}

std::shared_ptr<Mesh> MeshLoader::LoadModel(Buffer& buffer) {
    std::shared_ptr<Mesh> model = nullptr;

	if (buffer.Empty()) {
		global_log.Report<LogCode::FAULT>("Empty buffer passed to MeshLoader did file load correctly?", __FUNCTION__, __LINE__);
		return nullptr;
	}

	switch (QueryFormat(buffer)) {
	case MODEL_FORMAT::OBJ:
		model = LoadOBJ(&buffer);
		break; 
	case MODEL_FORMAT::_3DS:
		global_log.Report<LogCode::FAULT>("Unable to load .3DS files from a buffer", __FUNCTION__, __LINE__);
	case MODEL_FORMAT::UNDEFINED:
		global_log.Report<LogCode::FAULT>("Unable to load model: unknown format", __FUNCTION__, __LINE__);
		return nullptr;
	}
    
	if (model == nullptr)
		global_log.Report<LogCode::FAULT>("Unable to load model: Load fail", __FUNCTION__, __LINE__);

	return model;
} 

std::shared_ptr<Mesh> MeshLoader::LoadModel(Buffer* file){
	return LoadModel(*file);
}
 
std::shared_ptr<Mesh> MeshLoader::Load3DSModel(const std::string& path) {
	Lib3dsFile* file = lib3ds_file_open(path.data());
	unsigned vertex_count = 0;
	for (int x = 0; x < file->nmeshes; ++x)
		vertex_count += file->meshes[x]->nvertices;

	Buffer vertex_data(vertex_count * sizeof(float) * 5);
	BufferWriter writer(&vertex_data);

	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	Mesh mesh(vertex_buffer, 0);

	for (int x = 0; x < file->nmeshes; ++x) {
		unsigned p_start = writer.head();
		unsigned p_size = file->meshes[x]->nvertices;
		writer.WriteArray<float>(file->meshes[x]->vertices[0], p_size * 3);  
		MeshObject object(file->meshes[x]->name, p_start, p_size);
		mesh.AttachObject(std::move(object));
	}
	 
	glBufferData(GL_ARRAY_BUFFER, writer.head(), vertex_data.data<char>(), GL_STATIC_DRAW); 
	return nullptr;
}