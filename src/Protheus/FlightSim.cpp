#include <ProMath.h>
#include <Graphics/Window.h>
#include "Plane.h"
#include "Hoop.h"
#include "Skybox.h"
#include "ShaderCode.h"
#include <Graphics/HeightMapMesh.h>
#include <Graphics/MeshLoader.h>
#include <Graphics/TextureLoader.h>
#include <Graphics/Projection.h>
#include <FileSystem/FileSystem.h>
#include <stack>

using namespace std;
using namespace chrono;
using namespace Pro;
using namespace Math;
using namespace Graphics;
using namespace Input;
  
class FlightSim {
	Window window;

	Projection camera;
	Vector3<float> camera_position;
	Vector3<float> camera_rotation;

	HeightMesh height;
	Plane main_plane;

	std::stack<Hoop> hoops;
	Timer<std::chrono::milliseconds> frame_timer;

	Shader vertex;
	Shader fragment;
	Program program;
	 
	VertexArray terrain_vao, hoop_vao;
	 
	FileSystem fileSys;

	shared_ptr<Texture> skybox_texture;
	shared_ptr<Mesh> hoop_model;
	shared_ptr<Mesh> skybox_model;

	Skybox skybox;

    
	bool wireframe;
	unsigned score;
    float time_avaliable;
public:
	FlightSim() : 
		// Initialize Window
		window("Joshua Waring", { 1320, 720 }) ,
		camera(0.1f, 1000, 45, window.Aspect()),
		
		// Load Shaders
		vertex(Buffer((void*)vertex_shader.data(), vertex_shader.size(), false), GL_VERTEX_SHADER),
		fragment(Buffer((void*)fragment_shader.data(), fragment_shader.size(), false), GL_FRAGMENT_SHADER),
		fileSys(""), 
		
		// Load Models
		hoop_model(MeshLoader::LoadModel(fileSys.GetFile("hoop.obj"))), 
		skybox_model(MeshLoader::LoadModel(fileSys.GetFile("skybox.obj"))),
		skybox_texture(TextureLoader::LoadTexture(fileSys.GetFile("skybox.png"))),
		
		// Initialize Skybox
		skybox(skybox_model, skybox_texture)
	{
		//
		//// Attach Shaders
		//
		   
		program.AttachShader(vertex);
		program.AttachShader(fragment);
		program.Link();
		program.Use(); 

		//
		//// Prepare Scene 
		//
		 
		hoop_vao.Bind();
		
		if (hoop_model != nullptr && hoop_model->GetObjects().size() != 0) {
			hoop_model->Bind();
			auto& object = hoop_model->GetObjects()[0];
			hoop_vao.setVertexAttribute(program, "in_vertex", 3, GL_FLOAT, GL_FALSE, object.Stride(), object.VertexOffset());
			hoop_vao.setVertexAttribute(program, "in_normal", 3, GL_FLOAT, GL_FALSE, object.Stride(), object.NormalOffset());
		}
		else
			global_log.Report<LogCode::FAULT>("Unable to create hoops, model failed to load", __FUNCTION__, __LINE__);

		terrain_vao.Bind();

		height.smoothing(0.1f);
		height.height(2.f); 
		height.GenerateTerrain(1024, 1024);
		height.GenerateBuffer(false, program, 5000, 5000);
		program.SetUniform("sun_vector", Vector3<float>({ -.5, -.5, 0 }).NormalizeThis());

		srand((unsigned)GetGlobalTimer().GetTime<nanoseconds>());
		for (int x = 0; x < 100; ++x)
			hoops.push(Hoop({ float(rand() % 1000), float(rand() % 40 + 20), float(rand() % 1000) }, 10));
		 
		wireframe = false; 
		score = 0;
        time_avaliable = 120;

		window.Focus(); 
	}

	~FlightSim() {
		window.Close();
	}

	void Run() {
		auto start_time = GetGlobalTimer().GetTime<milliseconds>();
		while(window.IsExitRequested() == false) { 
			frame_timer.Tick();
			std::this_thread::sleep_for(milliseconds(16 - frame_timer.GetTickDelta()));

			Update();
			Render();
			if (hoops.empty() ||
				time_avaliable <= duration_cast<seconds>(milliseconds(GetGlobalTimer().GetTime<milliseconds>()) - milliseconds(start_time)).count())
				break;
		}
		auto end_time = GetGlobalTimer().GetTime<std::chrono::milliseconds>();
		unsigned result = score;
		std::cout << "Game Over - Score: " << result << std::endl;
	}
    
	void Render() {
        glEnable(GL_DEPTH_TEST);
		window.StartFrame();
		 
		auto main_plan_camera = main_plane.GetCamera();
        
        //
        //// Render Hoops
        //
        
        program.Use(); 
		program.SetUniform("diffuse_color", Vector3<float>({ 255, 0, 0 }).NormalizeThis());
        
        if (hoop_model != nullptr && hoops.empty() == false) {
            Matrix44 hoop_position;
            hoop_position.Translate(hoops.top().position());
            program.SetUniform("world", hoop_position);
            hoop_model->Bind();
            hoop_vao.Bind();
            for (auto& x : hoop_model->GetObjects())
                glDrawArrays(x.GetMode(), x.start, x.size); 
        }
        
		//
		//// Render Terrain
		//
          
		program.SetUniform("diffuse_color", Vector3<float>({ 241, 253, 255 }).NormalizeThis());

        height.Bind();
		program.SetUniform("projection", camera.GetPerspective()); 
		program.SetUniform("camera", main_plan_camera);
		program.SetUniform("model", IDENTITY_MATRIX);
		Matrix44 terrain_position;
        terrain_position.Translate({-2500, 0, -2500});
		program.SetUniform("world", terrain_position); 
		height.Render(wireframe);
        
        //
        //// Render Skybox
        //
        
        skybox.Render(camera.GetPerspective(), main_plane.rotation());
        
		 
		window.EndFrame();
	}

	void Update() {
		window.Update();
		KeyboardUpdate();
		main_plane.Update();
		if (hoops.empty() == false) {
			if (hoops.top().HasCollided(main_plane.position())) {
				++score;
				hoops.pop();
			}
		}
        static bool start_count = false;
        if(main_plane.position().y >= 0.5)
            start_count = true;
        if(start_count && main_plane.position().y <= 0.001f)
            time_avaliable -= 0.5f;
	}

	void KeyboardUpdate() {
		auto key_event = window.keyboard().PollKey();

		Keyboard& keyboard = window.keyboard();
		switch (key_event.second) {
		case KEY::KEY_8:
			if (key_event.first == KEY_PRESSED::PRESSED)
				wireframe = !wireframe;
			break;
		case KEY::KEY_7:
			if (key_event.first == KEY_PRESSED::PRESSED)
				camera_position = camera_rotation = { 0, 0, 0 };
			break; 
		default:
			break;
		} 

		// Pitch
		if (keyboard.IsKeyDown(KEY::KEY_S))
			main_plane.raisePitch();
		if (keyboard.IsKeyDown(KEY::KEY_W))
			main_plane.lowerPitch();
 
		// Yaw
		if (keyboard.IsKeyDown(KEY::KEY_A))
			main_plane.leftYaw();
		if (keyboard.IsKeyDown(KEY::KEY_D))
			main_plane.rightYaw();

		if (keyboard.IsKeyDown(KEY::KEY_UP))
			main_plane.raiseThrust();
		if (keyboard.IsKeyDown(KEY::KEY_DOWN))
			main_plane.lowerThrust(); 
	} 
};


struct Velocity : Math::Vector4<float> {
};

#include <QuadTree.h>
#include <Timer.h>
#include "../Engine/Entity/Entity.h"
#include "../Engine/Entity/System.h" 
#include "../Engine/Entity/Components/Position.h"
using namespace Pro::ECS;
int main() {

	Entity entity("balls");
	entity.AddComponent<Position>([](Position* component) {
		component->Set(0, 0, 0, 0);
		});
	unsigned increment = 0;
	entity.AddComponent<Velocity>([&](Velocity* component) {
		component->Set(0, increment++ * 0.01, 0, 0);
		});

	for (int i = 0; i < 1024 * 1024 * 10; ++i)
		entity.NewInstance();


	System<Position, Velocity> ProcessPhysics([](System<Position, Velocity>& system) {
		Position* position;
		while ((position = system.Next<Position>()) != nullptr) {
			auto velocity = system.Next<Velocity>();

			velocity->y -= 9.8f / 1000.f;
			*position += *velocity;

			if (position->y <= 0) {
				velocity->y *= -0.8f;
				position->y = 0.01f;
			}
		}
		});

	for (int i = 0; i < 100; ++i) {
		ProcessPhysics.Execute(entity);
	}
	//srand(Pro::GetGlobalTimer().getTime());
	//QuadTree<int, 4> tree({ 50, 50}, { 100, 100 });
	//for (int x = 0; x < 1000; ++x){ 	
	//	Vector2<float> point = { Rand<float>(0, 100), Rand<float>(0, 100) };
	//	tree.Insert(x, point);
	//	tree.GetQuadrantData(point);
	//}
	
	//global_log.EchoOnConsole(true);
	//{
	//	FlightSim game;
	//	game.Run();
	//}
	//global_log.Close();
	//sleep_for(10000);
} 