#include "SpriteBatcher.h"

using namespace Pro; 
using namespace Graphics;

Pro::Shader Pro::Graphics::SpriteBatcher::vertex_shader;
Pro::Shader Pro::Graphics::SpriteBatcher::fragment_shader;
Pro::Program Pro::Graphics::SpriteBatcher::batch_program(false);

GLint Pro::Graphics::SpriteBatcher::max_sprites = 0;
GLint Pro::Graphics::SpriteBatcher::max_textures = 0;


const char* source_vertex_shader =
"#version 420							\n"
"in vec3 vertex;						\n"
"in vec2 tex_coord;						\n"
"in int texture_id;						\n"
"										\n"
"out vec2 out_tex_coord;				\n"
"flat out int out_texture_id;			\n"
"										\n"
"void main() {							\n"
"	out_texture_id = texture_id;		\n"
"	out_tex_coord = tex_coord;			\n"
"	gl_Position = vec4(vertex, 1);		\n"
"}\0									\n"
;


const char* source_fragment_shader =
"#version 420   														\n"
"out vec4 out_color;  													\n"
"																		\n"
"in vec2 out_tex_coord; 												\n"
"flat in int out_texture_id;											\n"
"																		\n"
"layout(binding = 0) uniform sampler2D sampler1;											\n"
"layout(binding = 1) uniform sampler2D sampler2;											\n"
"layout(binding = 2) uniform sampler2D sampler3;											\n"
"layout(binding = 3) uniform sampler2D sampler4;											\n"
"																		\n"
"void main() {															\n"
"	if(out_texture_id == 1)												\n"
"		out_color = texture(sampler1, out_tex_coord);					\n"
"   else if(out_texture_id == 2)										\n"
"		out_color = texture(sampler2, out_tex_coord);					\n"
"	else if(out_texture_id == 3)										\n"
"		out_color = texture(sampler3, out_tex_coord);					\n"
"	else																\n"
"		out_color = texture(sampler4, out_tex_coord);					\n"
"}\0																	\n"
;

SpriteBatcher::SpriteBatcher() {
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &max_sprites);
	max_textures = 4;

	verticies = new CBuffer((max_sprites * 3 * sizeof(GLfloat) + sizeof(GLubyte)) * 4);
	writer = new BufferWriter(verticies);

	static bool first_init = true;
	if (first_init) {
		batch_program.init();
		// Breakpoint to make sure only run once


		vertex_shader.init(source_vertex_shader, GL_VERTEX_SHADER);
		fragment_shader.init(source_fragment_shader, GL_FRAGMENT_SHADER);
		batch_program.attachShader(vertex_shader);
		batch_program.attachShader(fragment_shader);
		batch_program.link();

		if (batch_program.hasError()) {
			first_init = true;
			return;
		}

		const unsigned stride_size = sizeof(GLfloat) * 5 + sizeof(GLint);


		vao.preservedBind();

		// CHECK OFFSETS 
		CBuffer vertex_buffer(max_sprites * stride_size * 6, false);
		BufferWriter vertex_buffer_writer(&vertex_buffer);

		glGenBuffers(1, &vertex_buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, verticies->size(), verticies->data(), GL_STREAM_DRAW);

		vao.setVertexAttribute(batch_program, "vertex", 3, GL_FLOAT,
			GL_FALSE, stride_size, 0);

		vao.setVertexAttribute(batch_program, "texture_id", 1, GL_INT, GL_FALSE,
			stride_size, 3 * sizeof(GLfloat));

		vao.setVertexAttribute(batch_program, "tex_coord", 2, GL_FLOAT, GL_FALSE,
			stride_size, 3 * sizeof(GLfloat) + sizeof(GLint));
  
		//batch_program.setUniform("sampler1", 0); 
		//batch_program.setUniform("sampler2", 1); 
		//batch_program.setUniform("sampler3", 2);
		//batch_program.setUniform("sampler4", 3);
 
		vao.preservedUnbind();
		first_init = false;
	}
}

SpriteBatcher::~SpriteBatcher() {
	delete writer;
	delete verticies;

	glDeleteBuffers(1, &vertex_buffer_id);
}

void SpriteBatcher::push(int texture,
	Vector3<float> position,
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
	writer->write<int>(texture % max_textures);
	writer->write<float>(0);
	writer->write<float>(0);

	writer->write<float>(position.x);
	writer->write<float>(position.y + dimensions.y);
	writer->write<float>(position.z);
	writer->write<int>(texture % max_textures);
	writer->write<float>(1);
	writer->write<float>(0);

	writer->write<float>(position.x + dimensions.x);
	writer->write<float>(position.y + dimensions.y);
	writer->write<float>(position.z);
	writer->write<int>(texture % max_textures);
	writer->write<float>(1);
	writer->write<float>(1);

	writer->write<float>(position.x + dimensions.x);
	writer->write<float>(position.y);
	writer->write<float>(position.z);
	writer->write<int>(texture % max_textures);
	writer->write<float>(0);
	writer->write<float>(1);

	++current_sprite_count;
	// Figure out how to apply rotation and a sprite
	/*details.sprite = _s;
	details.rotation = rotate; */
}

int SpriteBatcher::attachTexture(smart_pointer<Texture> tex) {
	if (tex._ptr == nullptr)
		return -1;
	++current_texture_count;
	textures.push_back(std::move(tex)); 
	return textures.size();
}

void SpriteBatcher::removeTexture(int texture_id) {
	if (texture_id < 0 || texture_id > (int)textures.size())
		return;
	free_textures.push(texture_id);
	 
	textures.at(texture_id) = nullptr;
	--current_texture_count;
}

void SpriteBatcher::flush() {
	// Create the VBO 
	vao.preservedBind();

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, writer->getPosition(), verticies->data());

	batch_program.use();

	// Apply textures to texture units
	// WARNING, textures may have gaps from removeTexture(int)
	for (unsigned x = 0; x < current_texture_count % max_textures; ++x) {
		glActiveTexture(GL_TEXTURE0 + x); 
		textures.at(x)->bind(); 
	} 

	glDrawArrays(GL_QUADS, 0, current_sprite_count * 6);

	// SWAP out textues if there's more than max

	writer->reset();
	current_sprite_count = 0;

	vao.preservedUnbind();
}
