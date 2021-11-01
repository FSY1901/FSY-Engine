#include "FSY.h"
#include <iostream>
#include <string>

using namespace FSY;
#define DEBUG 1

class Control : public FSY::Component {
public:

	Control() {
		
	}

	float speed = 10.0f;

	void Start() override {
		
	}

	void Update() override {
		if (Input::GetKey(Keys::Key_W)) {
			gameObject->position.z -= speed * Time::deltaTime();
		}
		if (Input::GetKey(Keys::Key_S)) {
			gameObject->position.z += speed * Time::deltaTime();
		}
		if (Input::GetKey(Keys::Key_D)) {
			gameObject->position.x += speed * Time::deltaTime();
		}
		if (Input::GetKey(Keys::Key_A)) {
			gameObject->position.x -= speed * Time::deltaTime();
		}
	}

	const char* getName() {
		return "Control";
	}

	void DrawUI() {
		EditorUI::Text("Controller for the Object");
		EditorUI::SliderFloat("Speed", &speed, 1, 15);
	}

};

class App : public FSY::Application {

public:
	GameObject myObject;
	GameObject g2;
	GameObject g;
	Scene scene;
	Shader s;
	Shader colored;
	Texture t;
	Mesh m;
	Mesh m1;
	Sound* sound;
	Control* control;

	App() {
		
	}

	App(int width, int height, const char* title) : FSY::Application(width, height, title) {
		//Settings::fullscreen = true;
		Settings::s_editorFontPath = "./src/Data/Assets/Fonts/Poppins/Poppins-SemiBold.ttf";
	}

	~App() override{
		
	}

	void OnStart() override {
		//Settings
		inEditor = true;
		//Objects
		myObject = { Vector3f(0, 0, -5.0f), Vector3f(0, 0, 0), Vector3f(1, 1, 1), "Object" };
		g2 = { Vector3f(2, 0, -5.0f), Vector3f(30, 40, 0), Vector3f(1, 1, 1), "Object" };
		g = { Vector3f(5, 0, -5.0f), Vector3f(30, 40, 0), Vector3f(1, 1, 1), "Object" };
		sound = scene.GetCamera()->AddComponent<Sound>();
		//Dirs
#if DEBUG == 1
		s = { "./src/Data/Shaders/textured.vert", "./src/Data/Shaders/textured.frag" };
		t = { "./src/Data/Textures/Red.png" };
		colored = { "./src/Data/Shaders/colored.vert", "./src/Data/Shaders/colored.frag" };
		sound->Path("./src/Data/Audio/breakout.mp3");
#else
		s = { "Data/Shaders/textured.vert", "Data/Shaders/textured.frag" };
		t = { "Data/Textures/Dirt.png" };
		colored = { "Data/Shaders/colored.vert", "Data/Shaders/colored.frag" };
		sound->Path("Data/Audio/breakout.mp3");
#endif
		sound->play = true;
		//Meshes & Scene
		m = {Mesh::s_verticesForCube, Mesh::s_cubeMeshSize, &s};
		m.isTransparent = true;
		m.renderMode = RenderModes::RENDER_FRONT;
		m.SetTexture(&t);
		m1 = { Mesh::s_verticesForCube, Mesh::s_cubeMeshSize, &colored };
		m1.isTransparent = true;
		m1.SetTexture(&t);
		g2.AddChild(&myObject);
		g2.AddComponent<Control>();
		g2.AddComponent<Control>();
		myObject.AddChild(&g);
		m1.AddGameObject(&g2);
		m1.AddGameObject(&myObject);
		m1.AddGameObject(&g);
		scene.AddInstanceMesh(&m1);
		/*objs = new GameObject[100];
		int iter = 0;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				objs[iter] = GameObject(Vector3f(i*2, 0, -5.0f + j*2), Vector3f(0, 0, 0), Vector3f(1, 1, 1), std::to_string(iter).c_str());
				m.AddGameObject(&objs[iter]);
				iter++;
			}
		}
		scene.AddInstanceMesh(&m);*/
		ChangeScene(&scene);
	}

	void OnUpdate() override {
		colored.setColorValues4("Color", 0.2f, 0.9f, 0.2f, 0.2f);
		if (Input::GetKey(Keys::Key_ESCAPE)) {
			Close();
		}
	}

	void OnEditorUpdate() override{
		colored.setColorValues4("Color", 0.2f, 0.9f, 0.2f, 0.2f);
		sound->Update();//Testing purposes :)
		if (Input::GetKey(Keys::Key_ESCAPE)) {
			Close();
		}
		if (Input::GetKey(Keys::Key_SPACE)) {
			sound->Stop();
		}
		if (Input::GetKey(Keys::Key_Q)) {
			sound->play = true;
		}
	}

	void OnClose() override {
		delete sound;
	}

};

FSY::Application* FSY::CreateApplication() {
	return new App(800, 600, "My App");
}