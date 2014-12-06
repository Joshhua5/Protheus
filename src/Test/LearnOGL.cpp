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
	Mesh* cube = MeshLoader::loadOBJ(&fs.getFile("obj/v.obj"));

	Transformation camera;
	Transformation model;


	camera.setPosition({0 , 0, -1 });

	GLenum err;
	// TEST 
	// cbuffer is deleted after being passed as rvalue
	auto tex = TextureLoader::loadTexture(fs.getFile("bananas.bmp"));

	GLuint vbo = 0;
	GLuint vao = 0;
	GLuint sampler = 0; 

	cube->bind();
	cube->unbind();

	Shader vert(fs.getFile("shader.vert"), GL_VERTEX_SHADER);
	Shader frag(fs.getFile("shader.frag"), GL_FRAGMENT_SHADER);
	if ((err = glGetError()) != GL_NO_ERROR)
		return err;
	Program program;
	program.attachShader(vert);
	program.attachShader(frag);
	program.link();
	program.setActive();
	program.setVertexAttribute("position", 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	program.setUniform("inColor", { 1, 0, 1 });
	/*program.setVertexAttribute("color", 0, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat) , 3 * sizeof(GLfloat));
	program.setVertexAttribute("texcoord", 0, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 6 * sizeof(GLfloat));
	*/ 

	glActiveTexture(GL_TEXTURE0);
	tex->bind();
	program.setUniform("tex", 0);

	glBindVertexArray(0);
	 
	while (true) {
		window.startFrame();
		glBindVertexArray(vao);
		cube->draw();
		//camera.rotate({ 0, 0, 0.01f });
		model.rotate(Vector3<float>(0, 0.01f, 0));
		program.setUniform("model", model.getViewMatrix());
		program.setUniform("view", camera.getViewMatrix());
		glBindVertexArray(0);
		window.endFrame();
	}
}