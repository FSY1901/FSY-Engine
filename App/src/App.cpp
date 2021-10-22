#include "FSY.h"
#include <iostream>
#include <string>

using namespace FSY;
#define DEBUG 1

class Control : public FSY::Component {
public:

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

	Sound* _s;
	Sound* s1;

	App() {
		
	}

	App(int width, int height, const char* title) : FSY::Application(width, height, title) {
		//Settings::fullscreen = true;
		Settings::editorFontPath = "./src/Data/Assets/Fonts/Poppins/Poppins-SemiBold.ttf";
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
		//Dirs
#if DEBUG == 1
		s = { "./src/Data/Shaders/textured.vert", "./src/Data/Shaders/textured.frag" };
		t = { "./src/Data/Textures/Red.png" };
		colored = { "./src/Data/Shaders/colored.vert", "./src/Data/Shaders/colored.frag" };
		_s = new Sound("./src/Data/Audio/song.mp3");
		s1 = new Sound("./src/Data/Audio/breakout.mp3");
#else
		s = { "Data/Shaders/textured.vert", "Data/Shaders/textured.frag" };
		t = { "Data/Textures/Dirt.png" };
		colored = { "Data/Shaders/colored.vert", "Data/Shaders/colored.frag" };
		_s = new Sound("Data/Audio/song.mp3");
		s1 = new Sound("Data/Audio/breakout.mp3");
#endif
		//Meshes & Scene
		m = {Mesh::s_verticesForCube, Mesh::s_cubeMeshSize, &s};
		m.isTransparent = true;
		m.renderMode = RenderModes::RENDER_FRONT;
		m.SetTexture(&t);
		m1 = { Mesh::s_verticesForCube, Mesh::s_cubeMeshSize, &colored };
		m1.isTransparent = true;
		m1.SetTexture(&t);
		//g2.AddChild(&myObject);
		myObject.AddChild(&g);
		g2.AddComponent<Control>();
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
		//_s->Play(Vector3f(0,0,0),5, 20);
		s1->Play(Vector3f(Camera::GetMain()->position.x, Camera::GetMain()->position.y, Camera::GetMain()->position.z),5, 20);
		if (Input::GetKey(Keys::Key_ESCAPE)) {
			Close();
		}
	}

	void OnClose() override {
		//delete[] objs;
		delete _s;
		delete s1;
	}

};

FSY::Application* FSY::CreateApplication() {
	return new App(800, 600, "My App");
}