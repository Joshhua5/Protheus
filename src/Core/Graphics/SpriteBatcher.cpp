#include "SpriteBatcher.h"

using namespace Pro;
using namespace Graphics;

Pro::Shader Pro::Graphics::SpriteBatcher::vertex_shader;
Pro::Shader Pro::Graphics::SpriteBatcher::fragment_shader; 
Pro::Shader Pro::Graphics::SpriteBatcher::geometry_shader;
Pro::Program Pro::Graphics::SpriteBatcher::batch_program(false);

GLint Pro::Graphics::SpriteBatcher::max_sprites = 0;
GLint Pro::Graphics::SpriteBatcher::max_textures = 0;


const char* source_vertex_shader =
"#version 420							\n"
"in vec3 vertex;						\n"
"in vec4 tex_coord;		 				\n"
"in vec2 dim;							\n"
"out vec4 v_tex_coord;		 			\n"
"out vec2 g_dim;						\n"
"void main() {					 		\n"
"	g_dim = dim;						\n"
"	v_tex_coord = tex_coord;			\n"
"	gl_Position = vec4(vertex, 1);		\n"
"}\0									\n"
;


const char* source_fragment_shader =
"#version 420   														\n"
"out vec4 out_color;  													\n"
"																		\n"
"smooth in vec2 f_tex_coord; 											\n" 
"																		\n"
"layout(binding = 0) uniform sampler2D sampler1;						\n"
"																		\n"
"void main() {				 											\n"
"	out_color = texture(sampler1, f_tex_coord); 						\n"
"}\0																	\n"
;

const char* source_geomerty_shader = "									 \n"
"#version 420   														 \n"
"layout(points) in;														 \n"
"layout(triangle_strip, max_vertices = 4) out;							 \n"
"																		 \n"
"smooth out vec2 f_tex_coord;											 \n"
"																		 \n"
"in vec4 v_tex_coord[];													 \n"
"in vec2 g_dim[];														 \n"
"																		 \n"
"void main() {															 \n"
"	gl_Position = gl_in[0].gl_Position + vec4(0, g_dim[0].y, 0, 0);		 \n"
"	f_tex_coord = v_tex_coord[0].xy + vec2(0, v_tex_coord[0].w);			 \n"
"	EmitVertex();														 \n"
"																		 \n"
"	gl_Position = gl_in[0].gl_Position;									 \n"
"	f_tex_coord = v_tex_coord[0].xy;										 \n"
"	EmitVertex();														 \n"
"																		 \n"
"	gl_Position = gl_in[0].gl_Position + vec4(g_dim[0], 0, 0);			 \n"
"	f_tex_coord = v_tex_coord[0].xy +  v_tex_coord[0].zw;						 \n"
"	EmitVertex();														 \n"
"																		 \n"
"	gl_Position = gl_in[0].gl_Position + vec4(g_dim[0].x, 0, 0, 0);		 \n"
"	f_tex_coord = v_tex_coord[0].xy + vec2(v_tex_coord[0].z, 0);				 \n"
"	EmitVertex();														 \n"
"	EndPrimitive();														 \n"
"}\0"
;

SpriteBatcher::SpriteBatcher() {
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &max_sprites);
	max_textures = 4;

	verticies = new CBuffer(max_sprites * 5 * sizeof(GLfloat));
	writer = new BufferWriter(verticies);

	static bool first_init = true;
	if (first_init) {
		batch_program.init();
		// Breakpoint to make sure only run once


		vertex_shader.init(source_vertex_shader, GL_VERTEX_SHADER);
		fragment_shader.init(source_fragment_shader, GL_FRAGMENT_SHADER);
		geometry_shader.init(source_geomerty_shader, GL_GEOMETRY_SHADER);

		batch_program.attachShader(vertex_shader);
		batch_program.attachShader(fragment_shader);
		batch_program.attachShader(geometry_shader);
		batch_program.link();

		if (batch_program.hasError()) {
			first_init = true;
			return;
		}

		const unsigned stride_size = sizeof(GLfloat) * 9;
		 
		vao.preservedBind();

		// CHECK OFFSETS 
		//CBuffer vertex_buffer(max_sprites * stride_size * 6, false);
		//BufferWriter vertex_buffer_writer(&vertex_buffer);

		glGenBuffers(1, &vertex_buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, verticies->size(), verticies->data(), GL_STREAM_DRAW);

		glGenBuffers(1, &element_buffer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticies->size() / 5, verticies->data(), GL_STREAM_DRAW);

		vao.setVertexAttribute(batch_program, "vertex", 3, GL_FLOAT,
			GL_FALSE, stride_size, 0);
		  
		vao.setVertexAttribute(batch_program, "dim", 2, GL_FLOAT,
			GL_FALSE, stride_size, 3 * sizeof(GLfloat));

		vao.setVertexAttribute(batch_program, "tex_coord", 4, GL_FLOAT, GL_FALSE,
			stride_size, 5 * sizeof(GLfloat));

		batch_program.setUniform("sampler1", 0);
		batch_program.setUniform("sampler2", 1);
		batch_program.setUniform("sampler3", 2);
		batch_program.setUniform("sampler4", 3);

		sprite_count.resize(max_sprites);

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

	dimensions *= scale;

	writer->write<float>(position.x);
	writer->write<float>(position.y);
	writer->write<float>(position.z);
	writer->write<float>(dimensions.x);
	writer->write<float>(dimensions.y);
	writer->write<float>(0);
	writer->write<float>(0);
	writer->write<float>(1);
	writer->write<float>(1);
	sprite_indicies.at(texture).push_back(current_sprite_count); 

	++sprite_count.at(texture);
	++current_sprite_count;

	// Figure out how to apply rotation and a sprite
	/*details.sprite = _s;
	details.rotation = rotate; */
}

int SpriteBatcher::attachTexture(smart_pointer<Texture> tex) {
	if (tex._ptr == nullptr)
		return -1;
	++current_texture_count;
	sprite_indicies.push_back(std::vector<unsigned>());
	textures.push_back(std::move(tex));
	return textures.size() - 1;
}

void SpriteBatcher::removeTexture(int texture_id) {
	if (texture_id < 0 || texture_id >(int)textures.size())
		return;
	free_textures.push(texture_id);

	textures.at(texture_id) = nullptr;
	--current_texture_count;
}

void SpriteBatcher::render() { 
	vao.preservedBind();

	// Copy in new vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, writer->getPosition(), verticies->data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);
	batch_program.use();

	unsigned offset = 0;

	// Apply textures to texture units
	// WARNING, textures may have gaps from removeTexture(int)
	for (unsigned x = 0; x < current_texture_count % max_textures; ++x) {
		glActiveTexture(GL_TEXTURE0);
		textures.at(x)->bind(); 

		// Copy in new element data
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sprite_indicies.at(x).size() * sizeof(GLint), sprite_indicies.at(x).data()); 
		glDrawElements(GL_POINTS, sprite_count.at(x), GL_UNSIGNED_INT, 0);
		 
		offset += sprite_count.at(x) * sizeof(GLint); 
	} 

	vao.preservedUnbind();

	GLuint err = glGetError();
	if (err != GL_NO_ERROR)
		error.reportError((char*)glewGetErrorString(err));
}

void SpriteBatcher::reset() {
	// CONSIDER making a class to allow
	// resetting of the vector without deallocating the internal array.
	for (unsigned x = 0; x < current_texture_count % max_textures; ++x)
		sprite_indicies.at(x) = std::vector<unsigned>();
	 
	writer->reset();
	current_sprite_count = 0;   
}

void SpriteBatcher::flush() { 
	render();
	reset(); 
}
