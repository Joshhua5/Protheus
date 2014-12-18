#include <Graphics\Window.h>
#include <Graphics\MeshLoader.h>
#include <Graphics\TextureLoader.h>
#include <Graphics\Lighting.h>
#include <FileSystem\FileSystem.h>

using namespace Pro;

int main() {
	Graphics::Window window("scene", { 800, 600 });
	
	Util::FileSystem fs; 
	fs.setRootDir("");

	auto scene = MeshLoader::loadOBJ(&fs.getFile("scene/scene.obj"));
	auto tex = TextureLoader::loadTexture(&fs.getFile("scene/Wall.bmp"));

	Shader vert(fs.getFile("shaders/shader.vert"), GL_VERTEX_SHADER);
	Shader frag(fs.getFile("shaders/shader.frag"), GL_FRAGMENT_SHADER);

	GLuint err = 0;
	if ((err = glGetError()) != GL_NO_ERROR)
		return err;
	Program program;

	program.attachShader(vert);
	program.attachShader(frag);
	program.link();
	program.use();

	// Get a good error if this hasn't been bound.
	scene->bind();

	VertexArray vao;
	vao.bind();

	vao.setVertexAttribute(program, "in_normal", scene->normalSize(), GL_FLOAT, GL_FALSE, scene->stride(), scene->normalOffset());
	vao.setVertexAttribute(program, "position", scene->vertexSize(), GL_FLOAT, GL_FALSE, scene->stride(), scene->vertexOffset());
	vao.setVertexAttribute(program, "in_tex", scene->texCoordSize(), GL_FLOAT, GL_FALSE, scene->stride(), scene->texCoordOffset());

	vao.unbind();

	program.setUniform("has_normal", scene->hasNormals());
	program.setUniform("has_tex_coord", scene->hasTexCoord());
	program.setUniform("world_pos", Vector3<float>(.5, 0, 0));

	
	glActiveTexture(GL_TEXTURE0);
	tex->bind();

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