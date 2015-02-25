#include "SpriteBatcher.h"

using namespace Pro;
using namespace Graphics;

Pro::Graphics::Shader Pro::Graphics::SpriteBatcher::vertex_shader;
Pro::Graphics::Shader Pro::Graphics::SpriteBatcher::fragment_shader;
Pro::Graphics::Shader Pro::Graphics::SpriteBatcher::geometry_shader;
Pro::Graphics::Program Pro::Graphics::SpriteBatcher::batch_program(false);

 std::mutex Pro::Graphics::SpriteBatcher::lk;

GLint Pro::Graphics::SpriteBatcher::max_sprites = 0;
GLint Pro::Graphics::SpriteBatcher::max_textures = 0;


const char* source_vertex_shader =
"#version 420																			\n"
"in vec3 vertex;																		\n"
"in vec4 tex_coord;		 																\n"
"in vec2 dim;																			\n"
"out vec4 v_tex_coord;		 															\n"
"out vec2 g_dim;																		\n"
"uniform vec3 camera_window;															\n"
"uniform vec3 camera_position;															\n"
"void main() {					 														\n"
"	g_dim = dim / camera_window.xy;														\n"
"	v_tex_coord = tex_coord;															\n"
"	gl_Position = vec4(((vertex - camera_position ) / camera_window) - 1, 1);			\n"
"}\0																					\n"
;

const char* source_fragment_shader =
"#version 420   														\n"
"out vec4 out_color;  													\n"
"																		\n"
"smooth in vec2 f_tex_coord; 											\n"
"uniform vec3 alpha;													\n"
"layout(binding = 0) uniform sampler2D sampler1;						\n"
"																		\n"
"void main() {				 											\n"
"	vec4 color = texture(sampler1, f_tex_coord);						\n"
"	bvec3 eql = equal(color.rgb, alpha); 								\n"
"	if(eql.z && eql.x && eql.y){ discard; };							\n"
"	out_color = color; 													\n"
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
"	f_tex_coord = v_tex_coord[0].xy + vec2(0, v_tex_coord[0].w);		 \n"
"	EmitVertex();														 \n"
"																		 \n"
"	gl_Position = gl_in[0].gl_Position;									 \n"
"	f_tex_coord = v_tex_coord[0].xy;									 \n"
"	EmitVertex();														 \n"
"																		 \n"
"	gl_Position = gl_in[0].gl_Position + vec4(g_dim[0], 0, 0);			 \n"
"	f_tex_coord = v_tex_coord[0].xy +  v_tex_coord[0].zw;				 \n"
"	EmitVertex();														 \n"
"																		 \n"
"	gl_Position = gl_in[0].gl_Position + vec4(g_dim[0].x, 0, 0, 0);		 \n"
"	f_tex_coord = v_tex_coord[0].xy + vec2(v_tex_coord[0].z, 0);		 \n"
"	EmitVertex();														 \n"
"	EndPrimitive();														 \n"
"}\0"
;

SpriteBatcher::SpriteBatcher(const Vector2<float>& window_dimensions) {
	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &max_sprites);
	max_textures = TextureUnit::max_textures();

	if (max_sprites >= 100000)
		max_sprites = 100000;
	 
	verticies = new Buffer(max_sprites * 5 * sizeof(GLfloat));
	writer = new BufferWriter(verticies);

	static bool first_init = true;
	if (first_init) {
		batch_program.Init();
		// Breakpoint to make sure only run once
		 
		vertex_shader.Init(source_vertex_shader, GL_VERTEX_SHADER);
		fragment_shader.Init(source_fragment_shader, GL_FRAGMENT_SHADER);
		geometry_shader.Init(source_geomerty_shader, GL_GEOMETRY_SHADER);

		batch_program.AttachShader(vertex_shader);
		batch_program.AttachShader(fragment_shader);
		batch_program.AttachShader(geometry_shader);
		batch_program.link();

		batch_program.SetUniform("camera_window",
			Vector3<float>(window_dimensions.x / 2, window_dimensions.y / 2, 1.f));
		batch_program.SetUniform("camera_position",
			Vector3<float>(0.f, 0.f, 0.f));

		if (batch_program.HasError()) {
			first_init = true;
			return;
		}

		const unsigned stride_size = sizeof(GLfloat) * 9;

		vao.PreservedBind();
		  
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

		sprite_count.Resize(max_sprites);

		vao.PreservedUnbind();
		first_init = false;

		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			global_log.Report<LogCode::ERROR>(string((char*)glewGetErrorString(err)) + ": Unable to initialize sprite_batcher", __FUNCTION__, __LINE__);
			first_init = true;
			glDeleteBuffers(1, &vertex_buffer_id);
			glDeleteBuffers(1, &element_buffer_id);
		}
	}
}

SpriteBatcher::~SpriteBatcher() {
	delete writer;
	delete verticies;

	glDeleteBuffers(1, &vertex_buffer_id);
	glDeleteBuffers(1, &element_buffer_id);
}



SpriteBatcher::SpriteBatcher(SpriteBatcher&& rhs) {
	*this = std::move(rhs);
}

SpriteBatcher& SpriteBatcher::operator=(SpriteBatcher&& rhs) {
	verticies = rhs.verticies;
	writer = rhs.writer;

	vao = std::move(rhs.vao);
	current_sprite_count = rhs.current_sprite_count.load();
	textures = std::move(rhs.textures);
	sprite_count = std::move(rhs.sprite_count);
	sprite_indicies = std::move(rhs.sprite_indicies);
	free_textures = std::move(rhs.free_textures);


	vertex_buffer_id = rhs.vertex_buffer_id;
	element_buffer_id = rhs.element_buffer_id;
	texture_uniforms = rhs.texture_uniforms;

	rhs.vertex_buffer_id =
		rhs.element_buffer_id =
		rhs.texture_uniforms =
		rhs.current_texture_count =
		rhs.current_sprite_count = 0;

	rhs.verticies = nullptr;
	rhs.writer = nullptr;
	return *this;
}

void SpriteBatcher::Push(int texture,
	Vector3<float> position,
	Vector2<float> dimensions,
	const  float scale,
	const  float rotate) {
	if (texture < 0)
		return;

	dimensions *= scale;

	float values[9] = { position.x , position.y , position.z, dimensions.x, dimensions.y, 0, 0, 1, 1 };

	writer->WriteElements(values, 9);  

	sprite_indicies.At(texture).PushBack(current_sprite_count++); 
	++sprite_count.At(texture);   

	// Figure out how to apply rotation and a sprite
	/*details.sprite = _s;
	details.rotation = rotate; */
}

//void SpriteBatcher::BatchPush(int texture,
//	Vector3<float> position,
//	Vector2<float> dimensions,
//	const  float scale,
//	const  float rotate) {
//	if (texture < 0)
//		return;
//
//	dimensions *= scale;
//
//	float values[9] = { position.x, position.y, position.z, dimensions.x, dimensions.y, 0, 0, 1, 1 };
//
//	writer->WriteElements(values, 9); 
//
//	current_sprite_count++;
//	sprite_indicies.At(texture).PushBack(current_sprite_count);
//
//	// Figure out how to apply rotation and a sprite
//	/*details.sprite = _s;
//	details.rotation = rotate; */
//}
//
//void SpriteBatcher::BatchUpdate(int texture, unsigned count){
//	if (texture < 0)
//		return;
//	//current_sprite_count += count;
//	sprite_count.At(texture) += count;
//}

int SpriteBatcher::AttachTexture(smart_ptr<Texture> tex) {
	if (tex.IsNull())
		return -1;
	++current_texture_count;
	sprite_indicies.PushBack(Util::ArrayList<unsigned>());
	textures.PushBack(std::move(tex));
	return textures.size() - 1;
}


int SpriteBatcher::AttachTexture(ArrayList<int>& indicies, const ArrayList<smart_ptr<Texture>>& texs) {
	unsigned size = texs.size();
	if (size < 0)
		return -1; 
	indicies.Reserve(size);

	for (unsigned x = 0; x < size; ++x) {
		if (texs.At(x) == nullptr)
			continue;
		++current_texture_count;
		sprite_indicies.PushBack(Util::ArrayList<unsigned>());
		textures.PushBack(texs[x]);
		indicies.PushBack(textures.size() - 1);
	}
	return size;
}

void SpriteBatcher::RemoveTexture(int texture_id) {
	if (texture_id < 0 || texture_id >(int)textures.size())
		return;
	free_textures.push(texture_id);

	textures.At(texture_id) = nullptr;
	--current_texture_count;
}

void SpriteBatcher::Render() {
	vao.PreservedBind();

	// Copy in new vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, writer->head(), verticies->data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);
	batch_program.Use();

	unsigned offset = 0;

	// Apply textures to texture units
	// WARNING, textures may have gaps from removeTexture(int)
	for (unsigned x = 0; x < current_texture_count % max_textures; ++x) {
		if (sprite_count.At(x) == 0)
			continue;

		TextureUnit::Bind(0, textures.At(x));
		  
		// Copy in new element data
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sprite_indicies.At(x).size() * sizeof(GLint), sprite_indicies.At(x).Data());

		glDrawElements(GL_POINTS, sprite_count.At(x), GL_UNSIGNED_INT, (void*)offset);

		offset += sprite_count.At(x) * sizeof(GLint);
	}

	vao.PreservedUnbind();

	GLuint err = glGetError();
	if (err != GL_NO_ERROR)
		global_log.Report<LogCode::ERROR>(string((char*)glewGetErrorString(err)) + ": Unable to render the spritebatcher\0", __FUNCTION__, __LINE__);
}


void SpriteBatcher::Alpha(const Vector3<float>& color) {
	batch_program.SetUniform("alpha", color);
}

void SpriteBatcher::Reset() {
	// CONSIDER making a class to allow
	// resetting of the vector without deallocating the internal array.
	for (unsigned x = 0; x < current_texture_count % max_textures; ++x) {
		sprite_indicies.At(x) = Util::ArrayList<unsigned>();
		sprite_count.At(x) = 0;
	}

	writer->Reset();
	current_sprite_count = 0;
}


void SpriteBatcher::SetCameraPosition(const Vector3<float>& position) {
	batch_program.SetUniform("camera_position", position);
}

void SpriteBatcher::SetCameraDimensions(const Vector2<float>& position) {
	batch_program.SetUniform("camera_window", Vector3<float>(position.x / 2, position.y / 2, 1.f));
}

void SpriteBatcher::Flush() {
	Render();
	Reset();
}
