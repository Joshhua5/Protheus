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
#include <smart_ptr.h>
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
 	auto cube = MeshLoader::LoadOBJ(&fs.GetFile("scene/monkey.obj"));
  
	Transformation camera;
	Projection projection(0.01f, 1000.0f, 45, window.aspect());
	Transformation model;
	Transformation light_t; 
	   
	camera.position({0 , 0 , .7f});

	GLenum err;
	// TEST 
	// cbuffer is deleted after being passed as rvalue
	auto tex = TextureLoader::LoadTexture(fs.GetFile("textures/box.bmp"));

	if (tex == nullptr || cube == nullptr) {
		return 1;
	}

	GLuint dwvbo = 0; 
	GLuint sampler = 0; 

	cube->Bind(); 

	Shader vert(fs.GetFile("shaders/shader.vert"), GL_VERTEX_SHADER);
	Shader frag(fs.GetFile("shaders/shader.frag"), GL_FRAGMENT_SHADER);
	if ((err = glGetError()) != GL_NO_ERROR)
		return err;
	Program program;
	program.AttachShader(vert);
	program.AttachShader(frag);
	program.Link();
	program.Use();

	VertexArray vao;
	vao.Bind(); 
	
	// Set the attirbutes of the model
	vao.setVertexAttribute(program, "in_normal", cube->GetObjects().at(0).NormalSize(), GL_FLOAT, GL_FALSE, cube->GetObjects().at(0).Stride(), cube->GetObjects().at(0).NormalOffset());
	vao.setVertexAttribute(program, "position", cube->GetObjects().at(0).VertexSize(), GL_FLOAT, GL_FALSE, cube->GetObjects().at(0).Stride(), cube->GetObjects().at(0).VertexOffset());
	vao.setVertexAttribute(program, "in_tex", cube->GetObjects().at(0).TexCoordSize(), GL_FLOAT, GL_FALSE, cube->GetObjects().at(0).Stride(), cube->GetObjects().at(0).TexCoordOffset());
 
	vao.Unbind(); 

	// Set positions for lighting
	program.SetUniform("world_pos", Vector3<float>(0, 0, 0));
	program.SetUniform("camera_pos", camera.position());
 
	TextureUnit::Bind(0, tex);
	  
	LightPoint point;
	point.position = { 2, 0, 0 };
	point.color = { 1, 1, 1 };
	point.intensity = 0.4f;
	point.attenuation = 10;

	light_t.position(Vector3<float>(2, 1, 0));

	Lighting lights;

	LightPoint& light = lights.AttachLight(point);

	lights.SetAmbient(Vector3<float>(1.0f, 1.0f, 1.0f));

	float pos = 0;
	Vector3<float> rotation(0);

	while (true) {
		// Start the frame
		window.StartFrame(); 
		vao.Bind(); 

		// Update light position with the mouse
		window.mouse().position<float>(&light.position.x, &light.position.y); 
		light.position.x /= window.width();
		light.position.y /= window.height();
		light.position /= ToVector3<float>(window.dimensions().Cast<float>(), 1);
		light_t.position(light.position);  

		model.scale({ 0.1f, 0.1f, 0.1f });
		model.position({ 0, -0.5f, 0 });

		// Sleep for roughly 60hz
		std::this_thread::sleep_for(std::chrono::milliseconds(16));

		// Set the projection
		program.SetUniform("model", model.getMatrix());
		program.SetUniform("view", camera.getMatrix()); 
		program.SetUniform("projection", projection.GetPerspective());
		// Rebind the lights as they move to the mouse
		lights.BindLights(program);
		program.SetUniform("normal_matrix", model.getNormalMatrix());
		cube->Bind();
		  
		// Draw the object
		for (const auto& obj : cube->GetObjects()){
			program.SetUniform("has_normal", obj.HasNormals());
			program.SetUniform("has_tex_coord", obj.HasTexCoord());
			glDrawElements(obj.GetMode(), obj.size, GL_UNSIGNED_INT, obj.p_start); 
		}

		// Unbind the VAO
		vao.Unbind();

		// Poll for input to rotate the model
		if (window.keyboard().IsKeyDown(KEY::KEY_W) != KEY_PRESSED::RELEASED)
			rotation.x += 0.1f;
		if (window.keyboard().IsKeyDown(KEY::KEY_D) != KEY_PRESSED::RELEASED)
			rotation.y += 0.1f;
		if (window.keyboard().IsKeyDown(KEY::KEY_S) != KEY_PRESSED::RELEASED)
			rotation.x -= 0.1f;
		if (window.keyboard().IsKeyDown(KEY::KEY_A) != KEY_PRESSED::RELEASED)
			rotation.y -= 0.1f;
		if (window.keyboard().IsKeyDown(KEY::KEY_Q) != KEY_PRESSED::RELEASED)
			rotation.z += 0.1f;
		if (window.keyboard().IsKeyDown(KEY::KEY_E) != KEY_PRESSED::RELEASED)
			rotation.z -= 0.1f;
		
		// Apply rotation to the model
		model.rotation(rotation);
		 
		// End frame
		window.EndFrame();
	}
}