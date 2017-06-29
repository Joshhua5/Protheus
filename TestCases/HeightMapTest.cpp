//
//  HeightMapTest.cpp
//  Protheus
//
//  Created by Joshua Waring on 6/09/2015.
//  Copyright (c) 2015 Joshua Waring. All rights reserved.
//

#include <string>

std::string vertex_shader = ""
"#version 410												 \n"
"															 \n"
"in vec3 in_color;											 \n"
"in vec3 in_normal;											 \n"
"in vec3 in_vertex;											 \n"
"															 \n"
"uniform mat4 model;										 \n"
"uniform mat4 projection;									 \n"
"uniform vec3 sun_vector;									 \n"
"															 \n"
"out vec3 color;											 \n"
"out vec3 normal;											 \n"
"flat out vec3 sun;											 \n"
"															 \n"
"void main() {												 \n"
"	gl_Position = projection * model * vec4(in_vertex, 1);	 \n"
"	color = in_color;										 \n"
"	sun = sun_vector;										 \n"
"	normal = in_normal;										 \n"
"}															 \n"
;
std::string fragment_shader = ""
"#version 410																 \n"
"																			 \n"
"in vec3 color;																 \n"
"in vec3 normal;															 \n"
"flat in vec3 sun;															 \n"
"																			 \n"
"out vec4 out_color;														 \n"
"																			 \n"
"void main() {																 \n"  
"	float diffuse_light = max(0.0, cos(dot(normal, sun)));					 \n"
"	out_color = vec4(diffuse_light, diffuse_light, diffuse_light, 1.f);		 \n" 
"	//out_color = vec4(normal, 1.f);		 \n" 
"}";


#include <Graphics/Window.h>  
#include <Graphics/Projection.h>
#include <Graphics/HeightMapMesh.h> 

using namespace Pro;
using namespace Graphics; 
using namespace Util;

int main(){
    global_log.EchoOnConsole(true);
    Window window("HeightMap", { 1320,720  });

    if(window.IsOpen() == false){
        global_log.Flush();
        return 1;
    } 
    
    VertexArray vao;
    vao.Bind();

	Shader vertex(Buffer((void*)vertex_shader.data(), vertex_shader.size(), false), GL_VERTEX_SHADER);
	Shader fragment(Buffer((void*)fragment_shader.data(), fragment_shader.size(), false), GL_FRAGMENT_SHADER);
	Program program;
	program.AttachShader(vertex);
	program.AttachShader(fragment);
	program.Link();
	program.Use(); 

	if (!program.Validate()) {
		global_log.Close();
		return 2;
	} 
	Projection camera(0.1f, 1000, 45, window.Aspect()); 
	Matrix44<float> model_matrix;

	HeightMesh height;
    height.smoothing(0.1f);
    height.height(2.f);
    
    auto timer = GetGlobalTimer();
    timer.Tick();
    height.GenerateTerrain(1024, 1024);
    height.GenerateBuffer(false, program);
    timer.Tick();
    global_log.Report<LogCode::MESSAGE>("dimensons:" + std::to_string(1024) + "x" + std::to_string(1024) +
                                        " duration: " + std::to_string(timer.GetTickDelta()), __FUNCTION__, __LINE__);  
	program.Use();
    
	height.GenerateBuffer(false, program); 
	program.SetUniform("projection", camera.GetPerspective());
	program.SetUniform("model", model_matrix);
    program.SetUniform("sun_vector", Vector3<float>({ -.5, -.5, 0 }).NormalizeThis());
  
	window.StartFrame();
	height.Render(); 
	window.EndFrame();
	window.Close();
	global_log.Close();
    return 0;
}