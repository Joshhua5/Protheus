#include <Graphics\Window.h>
#include <Graphics\MeshLoader.h>
#include <Graphics\TextureLoader.h>
#include <Graphics\Lighting.h>
#include <FileSystem\FileSystem.h>

using namespace Pro;
using namespace Graphics;
using namespace Util;

int main() {
	Graphics::Window window("scene", { 800, 600 });
	
	Util::FileSystem fs; 
	fs.rootDir("");

	auto scene = MeshLoader::LoadOBJ(&fs.GetFile("scene/scene.obj"));
	auto tex = TextureLoader::LoadTexture(&fs.GetFile("scene/Wall.bmp"));

	Shader vert(fs.GetFile("shaders/shader.vert"), GL_VERTEX_SHADER);
	Shader frag(fs.GetFile("shaders/shader.frag"), GL_FRAGMENT_SHADER);

	GLuint err = 0;
	if ((err = glGetError()) != GL_NO_ERROR)
		return err;
	Program program;

	program.AttachShader(vert);
	program.AttachShader(frag);
	program.Link();
	program.Use();

	// Get a good error if this hasn't been bound.
	scene->Bind();

	VertexArray vao;
	vao.Bind();

	vao.setVertexAttribute(program, "in_normal", scene->NormalSize(), GL_FLOAT, GL_FALSE, scene->Stride(), scene->NormalOffset());
	vao.setVertexAttribute(program, "position", scene->VertexSize(), GL_FLOAT, GL_FALSE, scene->Stride(), scene->VertexOffset());
	vao.setVertexAttribute(program, "in_tex", scene->TexCoordSize(), GL_FLOAT, GL_FALSE, scene->Stride(), scene->TexCoordOffset());

	vao.Unbind();

	program.SetUniform("has_normal", scene->HasNormals());
	program.SetUniform("has_tex_coord", scene->HasTexCoord());
	program.SetUniform("world_pos", Vector3<float>(.5, 0, 0));

	
	glActiveTexture(GL_TEXTURE0);
	tex->Bind();

	LightPoint point;
	point.position = { 2, 0, 0 };
	point.color = { 1, 1, 1 };
	point.intensity = 10;
	point.attenuation = 10;
	  
	Lighting lights;

	LightPoint& light = lights.attachLight(point); 
	lights.setAmbient(Vector3<float>(1.0f, 1.0f, 1.0f));


	while (true /*window.isExitRequested()*/) {
		window.startFrame();

		for (const auto& obj : scene->getObjects()) {
			glDrawElements(scene->getMode(), obj.size, GL_UNSIGNED_INT, obj.p_start);
		}

		window.endFrame();
	}
	 
}