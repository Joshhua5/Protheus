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
#include <Buffer.h>
#include <BufferWriter.h>
#include <Vector2.h>
#include <ArrayList.h>

using namespace Pro;
using namespace Util;
using namespace Input;
using namespace Graphics; 

int main() {
	Graphics::Window window("title", Vector2<int>(1800, 960));

	FileSystem fs;
   
	this_thread::sleep_for(std::chrono::seconds(1));
 	auto cube = MeshLoader::loadOBJ(&fs.getFile("scene/monkey.obj"));
  
	Transformation camera;
	Projection projection(0.01f, 1000.0f, 45, window.getAspect());
	Transformation model;
	Transformation light_t; 
	   
	camera.setPosition({0 , 0 , .7f});

	GLenum err;
	// TEST 
	// cbuffer is deleted after being passed as rvalue
	auto tex = TextureLoader::loadTexture(fs.getFile("textures/box.bmp"));

	if (tex == nullptr || cube == nullptr) {
		return 1;
	}

	GLuint dwvbo = 0; 
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
	 
	vao.setVertexAttribute(program, "in_normal", cube->getObjects().at(0).normalSize(), GL_FLOAT, GL_FALSE, cube->getObjects().at(0).stride(), cube->getObjects().at(0).normalOffset());
	vao.setVertexAttribute(program, "position", cube->getObjects().at(0).vertexSize(), GL_FLOAT, GL_FALSE, cube->getObjects().at(0).stride(), cube->getObjects().at(0).vertexOffset());
	vao.setVertexAttribute(program, "in_tex", cube->getObjects().at(0).texCoordSize(), GL_FLOAT, GL_FALSE, cube->getObjects().at(0).stride(), cube->getObjects().at(0).texCoordOffset());
 
	vao.unbind(); 

	program.setUniform("world_pos", Vector3<float>(0, 0, 0));
	program.setUniform("camera_pos", camera.getPosition());
 
	TextureUnit::bind(0, tex);
	  
	LightPoint point;
	point.position = { 2, 0, 0 };
	point.color = { 1, 1, 1 };
	point.intensity = 0.4f;
	point.attenuation = 10;

	light_t.setPosition(Vector3<float>(2, 1, 0));

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
		model.setScale({ 0.1f, 0.1f, 0.1f });
		model.setPosition({ 0, -0.5f, 0 });
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		program.setUniform("model", model.getMatrix());
		program.setUniform("view", camera.getMatrix()); 
		program.setUniform("projection", projection.getPerspective());
		lights.bindLights(program);
		program.setUniform("normal_matrix", model.getNormalMatrix());
		cube->bind();
		 

		for (const auto& obj : cube->getObjects()){
			program.setUniform("has_normal", obj.hasNormals());
			program.setUniform("has_tex_coord", obj.hasTexCoord());
			glDrawElements(obj.getMode(), obj.size, GL_UNSIGNED_INT, obj.p_start); 
		}

		vao.unbind();

		if (window.getKeyboard().isKeyDown(KEY::KEY_W) != KEY_PRESSED::RELEASED)
			rotation.x += 0.1f;
		if (window.getKeyboard().isKeyDown(KEY::KEY_D) != KEY_PRESSED::RELEASED)
			rotation.y += 0.1f;
		  
		model.setRotation(rotation);
		 
		window.endFrame();
	}
}