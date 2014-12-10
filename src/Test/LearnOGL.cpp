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
	Mesh* cube = MeshLoader::loadOBJ(&fs.getFile("obj/monkey.obj"));

	Transformation camera;
	Projection projection(0.01f, 1000.0f, 45, 1);
	Transformation model;
	Transformation light_t;
	   
	camera.setPosition({0 , 0 , .7f});

	GLenum err;
	// TEST 
	// cbuffer is deleted after being passed as rvalue
	auto tex = TextureLoader::loadTexture(fs.getFile("bananas.bmp"));

	GLuint vbo = 0; 
	GLuint sampler = 0; 

	cube->bind(); 

	Shader vert(fs.getFile("shader.vert"), GL_VERTEX_SHADER);
	Shader frag(fs.getFile("shader.frag"), GL_FRAGMENT_SHADER);
	if ((err = glGetError()) != GL_NO_ERROR)
		return err;
	Program program;
	program.attachShader(vert);
	program.attachShader(frag);
	program.link();
	program.setActive();
	 
	program.setVertexAttribute("in_normal", cube->normalSize(), GL_FLOAT, GL_FALSE, cube->stride(), cube->normalOffset());
	program.setVertexAttribute("position", cube->vertexSize(), GL_FLOAT, GL_FALSE, cube->stride(), cube->vertexOffset());
	program.setVertexAttribute("in_tex", cube->texCoordSize(), GL_FLOAT, GL_FALSE, cube->stride(), cube->texCoordOffset());

	program.setUniform("has_normal", cube->hasNormals());
	program.setUniform("has_tex_coord", cube->hasTexCoord());
	program.setUniform("world_pos", Vector3<float>(.5, 0, 0));

	tex->bind();
	glActiveTexture(GL_TEXTURE0);
	 
	 
	LightPoint point;
	point.position = { 2, 0, 0 };
	point.color = { 0, 1, 0 };
	point.intensity = 10;
	point.attenuation = 10;

	light_t.setPosition(Vector3<float>(2, 0, 0));

	Lighting lights;

	LightPoint& light = lights.attachLight(point); 
	lights.setAmbient(Vector3<float>(0.1f, 0.1f, 0.1f));

	
	float pos = 0;
	while (true) {
		window.startFrame(); 
		//camera.rotate({ 0, 0, 0.01f });
		if (pos >= 1)
			pos = -1;
		else
			pos += 0.1f;
		light.position.y = pos;
		light_t.setPosition(light.position);
		// 
		model.rotate(Vector3<float>(0, 0.1f , 0));
		model.setScale({ 0.1f, 0.1f, 0.1f });
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		program.setUniform("model", model.getMatrix());
		program.setUniform("view", camera.getMatrix()); 
		program.setUniform("projection", projection.getPerspective());
		lights.bindLights(program.getID());
		program.setUniform("normal_matrix", model.getNormalMatrix());
		cube->bind();
		for (const auto& obj : cube->getObjects()) {
			glDrawElements(cube->getMode(), obj.size, GL_UNSIGNED_INT, obj.p_start);
		} 
		light_t.setScale({ 0.1f, 0.1f, 0.1f }); 
		light_t.move({ 0, 0, 10 });
		program.setUniform("model", light_t.getMatrix()); 
		program.setUniform("normal_matrix", light_t.getNormalMatrix());

		light_t.move({ 0, 0, -10 });
		glDrawElements(cube->getMode(), cube->getObjects()[0].size, GL_UNSIGNED_INT, cube->getObjects()[0].p_start);
		
		if (window.getKeyboard().isKeyDown(KEY::KEY_W))
			light_t.move({ -1, 0, 0 });

		window.endFrame();
	}
}