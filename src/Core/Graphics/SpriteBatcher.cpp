#include "SpriteBatcher.h"

using namespace Pro;
using namespace Util;
using namespace Graphics;

 
SpriteBatcher::SpriteBatcher() {
	static bool first_init = true;
	if (first_init) {
		// Breakpoint to make sure only run once
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &max_sprites);
		glGetIntegerv(GL_MAX_TEXTURE_UNITS, &max_textures);

		vertex_shader.init("", GL_VERTEX_SHADER);
		fragment_shader.init("", GL_FRAGMENT_SHADER);
		batch_program.attachShader(vertex_shader);
		batch_program.attachShader(fragment_shader);
		batch_program.link();

		// initialize tex_coord buffer
		CBuffer tex_coord_buffer(max_sprites * sizeof(GLfloat) * 12);
		BufferWriter tex_coord_writer(&tex_coord_buffer);
		float tex_coords[] = { 0, 0, 0, 1, 1, 1, 1, 0 };

		for (int x = 0; x < max_sprites; ++x) 
			tex_coord_writer.write_elements<float>(tex_coords, 8); 

		vao.preservedBind();

		glGenBuffers(1, &tex_coord_buffer_id);
		glGenBuffers(1, &vertex_buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, max_sprites * (sizeof(GLfloat) + sizeof(GLubyte)) * 8,
			tex_coord_buffer.data(), GL_STATIC_DRAW);
		
		// CHECK OFFSETS
		batch_program.setVertexAttribute("tex_coord", 2, GL_FLOAT,
			GL_FALSE, 5 * sizeof(GLfloat) + sizeof(GLubyte),  0);

		CBuffer vertex_buffer(tex_coord_buffer, false);
		BufferWriter vertex_buffer_writer(&vertex_buffer);

		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, max_sprites * sizeof(GLfloat) * 12, verticies->data(), GL_STREAM_DRAW);
		
		batch_program.setVertexAttribute("texture_id", 1, GL_UNSIGNED_BYTE, GL_FALSE,
			5 * sizeof(GLfloat) + sizeof(GLubyte), 12 * sizeof(GLfloat));

		batch_program.setVertexAttribute("vertex", 3, GL_FLOAT,
			GL_FALSE, 5 * sizeof(GLfloat) + sizeof(GLubyte), 0);
		 
		GLint* samplers = new GLint[max_textures];
		for (int x = 0; x < max_textures; ++x)
			samplers[x] = x;

		batch_program.setUniform("samplers", samplers, max_textures);

		// TEST if it's safe to delete samplers
		delete[] samplers;

		vao.preservedUnbind();
		first_init = false;
	}
	verticies = new CBuffer(1024);
	writer = new BufferWriter(verticies);
}

SpriteBatcher::~SpriteBatcher() {
	delete writer;
	delete verticies;

	glDeleteBuffers(1, &tex_coord_buffer_id);
} 

void SpriteBatcher::push(int texture,
	Vector3<float>& position,
	Vector2<float> dimensions,
	const  float scale,
	const  float rotate) {
	if (texture < 0)
		return;

	// reorder sprites so that relevant texture's are 
	// rendered next to each other

	dimensions *= scale;

	writer->write<float>(position.x);
	writer->write<float>(position.y);
	writer->write<float>(position.z); 

	writer->write<float>(position.x);
	writer->write<float>(position.y + dimensions.y);
	writer->write<float>(position.z); 

	writer->write<float>(position.x + dimensions.x);
	writer->write<float>(position.y + dimensions.y);
	writer->write<float>(position.z); 

	writer->write<float>(position.x + dimensions.x);
	writer->write<float>(position.y);
	writer->write<float>(position.z); 

	writer->write<int>(texture & max_textures);

	++current_sprite_count;
	// Figure out how to apply rotation and a sprite
	/*details.sprite = _s;
	details.rotation = rotate; */
}

int SpriteBatcher::attachTexture(const Texture* tex) {
	++current_texture_count;
	textures.push_back(tex);
	tex->addReference();
	return textures.size();
}

void SpriteBatcher::removeTexture(int texture_id) {
	if (texture_id < 0 || texture_id > textures.size())
		return; 
	free_textures.push(texture_id);

	textures.at(texture_id)->removeReference();
	textures.at(texture_id) = nullptr;
	--current_texture_count;
}

void SpriteBatcher::flush() {
	// Create the VBO 
	vao.preservedBind();

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, writer->getPosition(), verticies->data());

	// Apply textures to texture units
	// WARNING, textures may have gaps from removeTexture(int)
	for (unsigned x = 0; x < current_texture_count % max_textures; ++x) {
		textures.at(x)->bind();
		glActiveTexture(GL_TEXTURE0 + x);
	}

	glDrawArrays(GL_QUADS, 0, current_sprite_count);

	// SWAP out textues if there's more than 18
	 
	writer->reset();
	current_sprite_count = 0;
	   
	vao.preservedUnbind();
}
