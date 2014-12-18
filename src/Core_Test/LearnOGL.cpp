#include <Graphics\Window.h>
#include <Graphics\Shader.h>
#include <FileSystem\FileSystem.h>
#include <Graphics\Program.h>
#include <Graphics\Camera.h>
#include <Graphics\MeshLoader.h>
#include <Graphics\Projection.h>
#include <thread>
#include <Graphics\Lighting.h>
#include <Graphics\TextureLoader.h>
#include <smart_pointer.h>
#include <CBuffer.h>
#include <BufferWriter.h>
#include <Vector2.h>

using namespace Pro;
using namespace Util;
using namespace Graphics; 

int main() {
	Graphics::Window window("title", Vector2<int>(800, 800));

	FileSystem fs;
   
	this_thread::sleep_for(std::chrono::seconds(1));
	auto cube = MeshLoader::loadOBJ(&fs.getFile("scene/scene.obj"));

	Transformation camera;
	Projection projection(0.01f, 1000.0f, 45, 1);
	Transformation model;
	Transformation light_t;
	   
	camera.setPosition({0 , 0 , .7f});

	GLenum err;
	// TEST 
	// cbuffer is deleted after being passed as rvalue
	auto tex = TextureLoader::loadTexture(fs.getFile("scene/Wall.bmp"));

	GLuint vbo = 0; 
	GLuint sampler = 0; 

	cube->bind(); 

	Shader vert(fs.getFile("shaders/shader.vert"), GL_VERTEX_SHADER);
	Shader frag(fs.getFile("shaders/shader.frag"), GL_FRAGMENT_SHADER);
	if ((err = glGetError()) != GL_NO_ERROR)
		return err;
	Program program;
	program.attachShader(vert);
	program.attachShader(frag);
	program.link();
	program.use();

	VertexArray vao;
	vao.bind();
	 

	vao.setVertexAttribute(program, "in_normal", cube->normalSize(), GL_FLOAT, GL_FALSE, cube->stride(), cube->normalOffset());
	vao.setVertexAttribute(program, "position", cube->vertexSize(), GL_FLOAT, GL_FALSE, cube->stride(), cube->vertexOffset());
	vao.setVertexAttribute(program, "in_tex", cube->texCoordSize(), GL_FLOAT, GL_FALSE, cube->stride(), cube->texCoordOffset());

	vao.unbind();

	program.setUniform("has_normal", cube->hasNormals());
	program.setUniform("has_tex_coord", cube->hasTexCoord());
	program.setUniform("world_pos", Vector3<float>(.5, 0, 0));

	glActiveTexture(GL_TEXTURE0);
	tex->bind();
	  
	LightPoint point;
	point.position = { 2, 0, 0 };
	point.color = { 1, 1, 1 };
	point.intensity = 10;
	point.attenuation = 10;

	light_t.setPosition(Vector3<float>(2, 0, 0));

	Lighting lights;

	LightPoint& light = lights.attachLight(point);

	lights.setAmbient(Vector3<float>(1.0f, 1.0f, 1.0f));
	 
	float pos = 0;
	Vector3<float> rotation(0);

	while (true) {
		window.startFrame(); 
		vao.bind();
		//camera.rotate({ 0, 0, 0.01f }); 

		//window.getMouse().getMousePosition<float>(&light.position.x, &light.position.y); 

		light_t.setPosition(light.position);  
		model.setScale({ 0.01f, 0.01f, 0.10f });
		model.setPosition({ 0, -0.1f, 0 });
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		program.setUniform("model", model.getMatrix());
		program.setUniform("view", camera.getMatrix()); 
		program.setUniform("projection", projection.getPerspective());
		lights.bindLights(program);
		program.setUniform("normal_matrix", model.getNormalMatrix());
		cube->bind();

		for (const auto& obj : cube->getObjects()) {
			glDrawElements(cube->getMode(), obj.size, GL_UNSIGNED_INT, obj.p_start);
		} 
  
		vao.unbind();

		if (window.getKeyboard().isKeyDown(KEY::KEY_W) != KEY_PRESSED::RELEASED)
			rotation.x += 0.1f;
		if (window.getKeyboard().isKeyDown(KEY::KEY_D) != KEY_PRESSED::RELEASED)
			rotation.y += 0.1f;
		 
		camera.setRotation(rotation);


		window.endFrame();
	}
}