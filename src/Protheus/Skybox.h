#pragma once
#include <Graphics/Texture.h>
#include <Graphics/Mesh.h>
#include "ShaderCode.h"

using namespace Pro;
using namespace Graphics;

class Skybox {
	shared_ptr<Mesh> model_;
	shared_ptr<Texture> texture_;

	Shader skybox_frag;
	Shader skybox_vert;
	Program program;

	VertexArray vao;
	 
	bool has_error;
public:
	Skybox(shared_ptr<Mesh> model, shared_ptr<Texture> texture) : 
		skybox_vert(Buffer((void*)skybox_vert_shader.data(), skybox_vert_shader.size(), false), GL_VERTEX_SHADER),
		skybox_frag(Buffer((void*)skybox_frag_shader.data(), skybox_frag_shader.size(), false), GL_FRAGMENT_SHADER)
	{
		model_ = model;
		texture_ = texture;
		has_error = false;

		//
		//// Initialise Shaders
		//
		vao.Bind();

		program.AttachShader(skybox_frag);
		program.AttachShader(skybox_vert);
		program.Link();
		program.Use();

		// Check for an error 
		if (program.Validate() == false || model_ == nullptr || texture_ == nullptr) 
			has_error = true; 

		if (model_ != nullptr) {
			model_->Bind();
			auto& obj = model_->GetObjects()[0];
			vao.setVertexAttribute(program, "in_vertex", 3, GL_FLOAT, GL_FALSE, obj.Stride(), obj.VertexOffset());
			vao.setVertexAttribute(program, "in_texcoord", 2, GL_FLOAT, GL_FALSE, obj.Stride(), obj.TexCoordOffset());
		}
        
        // Bind that sampler to texture unit 0
        auto loc = glGetUniformLocation(program.id(), "tex");
        glUniform1i(loc, 0);
        
	}
	void Render(const Matrix44<float>& projection, const Vector3<float>& camera_rotation) {
		if (has_error)
			return;
		vao.Bind();
		program.Use();

		static Matrix44<float> scale;
		static bool first_run = true;
		if (first_run) {
			scale.Scale(2000);
			scale.Rotate({ 0, 0, Math::DegToRad(-90) });
			first_run = false;
		}

		Matrix44<float> rotation;
		rotation.Rotate(camera_rotation);
		 
		program.SetUniform("projection", projection);
		program.SetUniform("camera", rotation);
		program.SetUniform("model", scale);
		  
		model_->Bind();
        // Bind our texture to texture unit 0
		TextureUnit::Bind(0, texture_);
		for (auto& obj : model_->GetObjects())	
			glDrawArrays(obj.GetMode(), obj.start, obj.size);
	} 
};