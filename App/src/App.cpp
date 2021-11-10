#include "FSY.h"
#include <iostream>
#include <string>

using namespace FSY;
#define DEBUG 1

bool rayTriangleIntersect(
	Vector3f orig, Vector3f dir,
	Vector3f v0, Vector3f v1, Vector3f v2,
	float& t)
{

	// compute plane's normal
	Vector3f v0v1 = v1 - v0;
	Vector3f v0v2 = v2 - v0;
	// no need to normalize
	Vector3f N = Vector3f::CrossProduct(v0v1, v0v2);

	// Step 1: finding P

	dir = Vector3f::Normalize(dir);
	// check if ray and plane are parallel ?
	float NdotRayDirection = Vector3f::DotProduct(N, dir);
	//std::cout << NdotRayDirection << std::endl;
	if (fabs(NdotRayDirection) < 0.0001f) // almost 0 
		return false; // they are parallel so they don't intersect ! 

	// compute d parameter using equation 2
	float d = Vector3f::DotProduct(N, v0);
	//std::cout << d << std::endl;

	// compute t (equation 3)
	t = -(Vector3f::DotProduct(N, orig) - d) / NdotRayDirection;
	// check if the triangle is in behind the ray
	if (t < 0) return false; // the triangle is behind 

	// compute the intersection point using equation 1
	Vector3f dirt = (dir * t);
	Vector3f P = orig + dirt;

	// Step 2: inside-outside test
	Vector3f C; // vector perpendicular to triangle's plane 

	// edge 0
	Vector3f edge0 = v1 - v0;
	Vector3f vp0 = P - v0;
	C = Vector3f::CrossProduct(edge0, vp0);
	if (Vector3f::DotProduct(N, C) < 0) return false; // P is on the right side 

	// edge 1
	Vector3f edge1 = v2 - v1;
	Vector3f vp1 = P - v1;
	C = Vector3f::CrossProduct(edge1, vp1);
	if (Vector3f::DotProduct(N, C) < 0)  return false; // P is on the right side 

	// edge 2
	Vector3f edge2 = v0 - v2;
	Vector3f vp2 = P - v2;
	C = Vector3f::CrossProduct(edge2, vp2);
	if (Vector3f::DotProduct(N, C) < 0) return false; // P is on the right side; 

	return true; // this ray hits the triangle 
}

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
	GameObject* objs;
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
		//Settings::s_fullscreen = true;
		Settings::s_editorFontPath = "./src/Data/Assets/Fonts/Poppins/Poppins-SemiBold.ttf";
	}

	~App() override{
		
	}

	void OnStart() override {
		//Settings
		inEditor = true;
		//Objects
		myObject = { Vector3f(0, 0, -5.0f), Vector3f(0, 0, 0), Vector3f(1, 1, 1), "Object" };
		g2 = { Vector3f(2, 0, -5.0f), Vector3f(0, 0, 0), Vector3f(1, 1, 1), "Object" };
		g = { Vector3f(5, 0, -5.0f), Vector3f(0, 0, 0), Vector3f(0.5f, 0.5f, 0.5f), "Object" };
		sound = scene.GetCamera()->AddComponent<Sound>();
		//Dirs
#if DEBUG == 1
		s = { "./src/Data/Shaders/textured.vert", "./src/Data/Shaders/textured.frag" };
		t = { "./src/Data/Textures/Dirt.png" };
		colored = { "./src/Data/Shaders/colored.vert", "./src/Data/Shaders/colored.frag" };
		sound->Path("./src/Data/Audio/breakout.mp3");
#else
		s = { "Data/Shaders/textured.vert", "Data/Shaders/textured.frag" };
		//t = { "Data/Textures/Dirt.png" };
		colored = { "Data/Shaders/colored.vert", "Data/Shaders/colored.frag" };
		sound->Path("Data/Audio/breakout.mp3");
#endif
		sound->play = true;
		//Meshes & Scene
		m1 = { Mesh::s_verticesForPlane, Mesh::s_planeMeshSize, &colored };
		m = { Mesh::s_verticesForCube, Mesh::s_cubeMeshSize, &s };
		m.SetTexture(&t);
		g2.AddComponent<Control>();
		m1.AddGameObject(&myObject);
		m1.AddGameObject(&g2);
		m.AddGameObject(&g);
		/*PerlinNoise noise(200);
		int iter = 0;
		objs = new GameObject[1000];
		for (int x = 0; x < 25; x++) {
			for (int y = 0; y < 40; y++) {
				double yVal = noise.noise((double)x, (double)y, 0.5);
				objs[iter] = GameObject(Vector3f((float)x, (float)yVal*2, (float)y), Vector3f(0,0,0), Vector3f(1,1,1), "Object");
				m1.AddGameObject(&objs[iter]);
				iter++;
			}
		}*/
		scene.AddInstanceMesh(&m);
		scene.AddInstanceMesh(&m1);
		ChangeScene(&scene);
	}

	void OnUpdate() override {
		colored.setColorValues4("Color", 0.9f, 0.2f, 0.2f, 1.0f);
		if (Input::GetKey(Keys::Key_ESCAPE)) {
			Close();
		}
	}

	void OnEditorUpdate() override{
		colored.setColorValues4("Color", 0.9f, 0.2f, 0.2f, 1.0f);
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
		float intersect = 0;
		glm::vec4 v0_ = { -0.5f, 0.0f, 0.5f, 1 };
		glm::vec4 v1_ = { 0.5f, 0.0f, 0.5f, 1 };
		glm::vec4 v2_ = { -0.5f, 0.0f, -0.5f, 1 };
		glm::vec4 v0_1 = { 0.5f, 0.0f, 0.5f, 1 };
		glm::vec4 v1_1 = { 0.5f, 0.0f, -0.5f, 1 };
		glm::vec4 v2_1 = { -0.5f, 0.0f, -0.5f, 1 };
		//Transform v0_ - v2_
		v0_ = myObject.GetTransformationMatrix() * v0_;
		v1_ = myObject.GetTransformationMatrix() * v1_;
		v2_ = myObject.GetTransformationMatrix() * v2_;
		v0_1 = myObject.GetTransformationMatrix() * v0_1;
		v1_1 = myObject.GetTransformationMatrix() * v1_1;
		v2_1 = myObject.GetTransformationMatrix() * v2_1;
		if (rayTriangleIntersect(Camera::GetMain()->position, Camera::GetMain()->rotation , Vector3f(v0_.x, v0_.y, v0_.z), Vector3f(v1_.x, v1_.y, v1_.z), Vector3f(v2_.x, v2_.y, v2_.z), intersect)) {
			colored.setColorValues4("Color", 0.2f, 0.9f, 0.2f, 1.0f);
			Vector3f v = Vector3f::Normalize(Camera::GetMain()->rotation);
			Vector3f pos = (v * intersect);
			g.position = (Camera::GetMain()->position + pos);
		}
		if (rayTriangleIntersect(Camera::GetMain()->position, Camera::GetMain()->rotation, Vector3f(v0_1.x, v0_1.y, v0_1.z), Vector3f(v1_1.x, v1_1.y, v1_1.z), Vector3f(v2_1.x, v2_1.y, v2_1.z), intersect)) {
			colored.setColorValues4("Color", 0.2f, 0.9f, 0.2f, 1.0f);
			Vector3f v = Vector3f::Normalize(Camera::GetMain()->rotation);
			Vector3f pos = (v * intersect);
			g.position = (Camera::GetMain()->position + pos);
		}
	}

	void OnClose() override {
		delete sound;
		delete[] objs;
	}

};

FSY::Application* FSY::CreateApplication() {
	return new App(800, 600, "My App");
}