#include <Graphics\Window.h>
#include <Graphics\Shader.h>
#include <FileSystem\FileSystem.h>
#include <Graphics\Program.h>
#include <Graphics\Camera.h>
#include <thread>
#include <Graphics\TextureLoader.h>
#include <CBuffer.h>
#include <BufferWriter.h>
#include <Vector2.h>

using namespace Pro;
using namespace Util;
using namespace Graphics;
using namespace Math;

int main() {
	Graphics::Window window("title", Vector2<int>(800, 800));

	FileSystem fs;
	//CBuffer verticies(sizeof(float) * 16 + 12);
	//BufferWriter writer(&verticies);
	//writer.write<float>(.5);
	//writer.write<float>(.5);
	//writer.write<char>(1);
	//writer.write<char>(0);
	//writer.write<char>(0);
	//writer.write<float>(0);
	//writer.write<float>(0);

	//writer.write<float>(.5);
	//writer.write<float>(-.5);
	//writer.write<char>(0);
	//writer.write<char>(1);
	//writer.write<char>(0);
	//writer.write<float>(1);
	//writer.write<float>(0);

	//writer.write<float>(-.5);
	//writer.write<float>(-.5);
	//writer.write<char>(0);
	//writer.write<char>(0);
	//writer.write<char>(1);
	//writer.write<float>(1);
	//writer.write<float>(1);

	//writer.write<float>(-.5);
	//writer.write<float>(.5);
	//writer.write<char>(1);
	//writer.write<char>(1);
	//writer.write<char>(0);
	//writer.write<float>(0);
	//writer.write<float>(1);

	float f_cube[] = {
		// X      Y     Z     R     G     B     U     V
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};

	CBuffer verticies(f_cube, 36 * 8, false);

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
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(GLfloat), verticies.data(), GL_STATIC_DRAW);
	if ((err = glGetError()) != GL_NO_ERROR)
		return err;
	Shader vert(fs.getFile("shader.vert"), GL_VERTEX_SHADER);
	Shader frag(fs.getFile("shader.frag"), GL_FRAGMENT_SHADER);
	if ((err = glGetError()) != GL_NO_ERROR)
		return err;
	Program program;
	program.attachShader(vert);
	program.attachShader(frag);
	program.link();
	program.setActive();
	program.setVertexAttribute("position", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	program.setVertexAttribute("color", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat) , 3 * sizeof(GLfloat));
	program.setVertexAttribute("texcoord", 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 6 * sizeof(GLfloat));


	glActiveTexture(GL_TEXTURE0);
	tex->bind();
	program.setUniform("tex", 0);

	glBindVertexArray(0);
	 
	while (true) {
		window.startFrame();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//camera.rotate({ 0, 0, 0.01f });
		model.rotate(Vector3<float>(0, 0.01f, 0));
		program.setUniform("model", model.getViewMatrix());
		program.setUniform("view", camera.getViewMatrix());
		glBindVertexArray(0);
		window.endFrame();
	}
}