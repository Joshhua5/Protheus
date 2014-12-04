#include "SpriteBatcher.h"

using namespace Pro;
using namespace Util; 
using namespace Graphics; 

SpriteBatcher::SpriteBatcher() {
	static bool first_init = true;
	if (first_init) {
		vertex_shader.init("", GL_VERTEX_SHADER);
		fragment_shader.init("", GL_FRAGMENT_SHADER); 
	} 
	verticies = new CBuffer(1024);
	writer = new BufferWriter(verticies);
}

SpriteBatcher::~SpriteBatcher() {
	delete writer;
	delete verticies;
}

void init_shaders() { 
}

void SpriteBatcher::push(const Sprite&,
	Vector2<float>& position,
	Vector2<float> dimensions,
	const  float scale,
	const  float rotate) {
	 
	dimensions *= scale;

	writer->write<float>(position.x);
	writer->write<float>(position.y);  

	writer->write<float>(position.x);
	writer->write<float>(position.y + dimensions.y);

	writer->write<float>(position.x + dimensions.x);
	writer->write<float>(position.y + dimensions.y);

	writer->write<float>(position.x + dimensions.x);
	writer->write<float>(position.y);
	 
	// Figure out how to apply rotation and a sprite
	/*details.sprite = _s;
	details.rotation = rotate; */
}

void SpriteBatcher::flush() {
	// Create the VBO 
	 
	GLuint buffer_id;
	glGenBuffers(1, &buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	glBufferData(GL_ARRAY_BUFFER, verticies->size(), verticies->data(), GL_STATIC_DRAW);

 
}
