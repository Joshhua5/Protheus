#include <Graphics\Window.h>
#include <Graphics\Shader.h>
#include <FileSystem\FileSystem.h>
#include <Graphics\Program.h>
#include <Graphics\Camera.h>
#include <Graphics\MeshLoader.h>
#include <thread>
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
	Mesh* cube = MeshLoader::loadOBJ(&fs.getFile("obj/vn.obj"));

	Transformation camera;
	Transformation model;
	   
	camera.setPosition({0 , 0 , .7f});

	GLenum err;
	// TEST 
	// cbuffer is deleted after being passed as rvalue
	auto tex = TextureLoader::loadTexture(fs.getFile("bananas.bmp"));

	GLuint vbo = 0;
	GLuint vao = 0;
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

	tex->bind();
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(0);
	 
	while (true) {
		window.startFrame();
		glBindVertexArray(vao);
		//camera.rotate({ 0, 0, 0.01f });
		model.rotate(Vector3<float>(0, 0.1f , 0.01f));
		model.setPosition({ 5.0f, 0, 0 });
		model.setScale({ 0.1f, 0.1f, 0.1f });
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		program.setUniform("model", model.getViewMatrix());
		program.setUniform("view", camera.getViewMatrix()); 
		cube->bind();
		for (const auto& obj : cube->getObjects()) {
			glDrawElements(cube->getMode(), obj.size, GL_UNSIGNED_INT, obj.p_start);
		}
		glBindVertexArray(0);
		window.endFrame();
	}
}