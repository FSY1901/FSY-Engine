#include "FSY.h"
#include <iostream>
#include <string>

using namespace FSY;
#define DEBUG 1

bool rayTriangleIntersect(Vector3f orig, Vector3f dir,
	Vector3f v0, Vector3f v1, Vector3f v2,
	float& t);

bool UnprojectRayCast(glm::vec3 pos, float radius, Vector3f v1, Vector3f v2, Vector3f v3) {
	float mouse_x = (float)Input::MouseX();
	float mouse_y = Input::WinHeight() - (float)Input::MouseY();
	float width = (float)Input::WinWidth();
	float height = (float)Input::WinHeight();
	glm::vec4 viewport = glm::vec4(0.0f, 0.0f, width, height);
	glm::vec3 rayStart = glm::unProject(glm::vec3(mouse_x, mouse_y, 0.0f), Camera::GetMain()->_GetViewMatrix(), Camera::GetMain()->_GetProjectionMatrix(), viewport);
	glm::vec3 rayEnd = glm::unProject(glm::vec3(mouse_x, mouse_y, 1.0f), Camera::GetMain()->_GetViewMatrix(), Camera::GetMain()->_GetProjectionMatrix(), viewport);

	glm::vec3 dirToSphere = pos - rayStart;
	//glm::vec3 lineDir = glm::normalize(rayEnd - rayStart);
	glm::vec3 lineDir = rayEnd - rayStart;
	float intersect;
	return rayTriangleIntersect(Camera::GetMain()->position, Vector3f(lineDir.x, lineDir.y, lineDir.z), v1, v2, v3, intersect);
	//return rayTriangleIntersect(Camera::GetMain()->position, Camera::GetMain()->rotation, v1, v2, v3, intersect);

	/*float len = glm::distance(rayStart, rayEnd);
	float t = glm::dot(dirToSphere, lineDir);
	glm::vec3 closestpoint;
	if (t <= 0)
		closestpoint = rayStart;
	else if (t >= len)
		closestpoint = rayEnd;
	else
		closestpoint = rayStart + lineDir * t;

	float dis = glm::distance(pos, closestpoint);
	return dis <= radius;*/
}

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

Vector3f Unproject() {
	float mouse_x = (float)Input::MouseX();
	float mouse_y = Input::WinHeight() - (float)Input::MouseY();
	float width = (float)Input::WinWidth();
	float height = (float)Input::WinHeight();
	glm::vec4 viewport = glm::vec4(0.0f, 0.0f, width, height);
	glm::vec3 rayStart = glm::unProject(glm::vec3(mouse_x, mouse_y, 0.0f), Camera::GetMain()->_GetViewMatrix(), Camera::GetMain()->_GetProjectionMatrix(), viewport);
	glm::vec3 rayEnd = glm::unProject(glm::vec3(mouse_x, mouse_y, 1.0f), Camera::GetMain()->_GetViewMatrix(), Camera::GetMain()->_GetProjectionMatrix(), viewport);
	//glm::vec3 lineDir = glm::normalize(rayEnd - rayStart);
	glm::vec3 lineDir = rayEnd - rayStart;

	return Vector3f(lineDir.x, lineDir.y, lineDir.z);
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
	Texture t1;
	Mesh m;
	Mesh m1;
	Control* control;
	Sound snd;

	App() {

	}

	App(int width, int height, const char* title) : FSY::Application(width, height, title) {
		//Settings::s_fullscreen = true;
		Settings::s_editorFontPath = "./src/Data/Assets/Fonts/Poppins/Poppins-SemiBold.ttf";
	}

	~App() override {

	}

	void OnStart() override {
		Camera::SetAsMain(scene.GetCamera());
		//Settings
		inEditor = true;
		//Objects
		myObject = { Vector3f(0, 0, -5.0f), Vector3f(0, 0, 0), Vector3f(1, 1, 1), "Object" };
		g2 = { Vector3f(2, 0, -5.0f), Vector3f(0, 0, 0), Vector3f(1, 1, 1), "Object" };
		g = { Vector3f(5, 0, -5.0f), Vector3f(0, 0, 0), Vector3f(0.5f, 0.5f, 0.5f), "Object" };
		//Dirs
#if DEBUG == 1
		s = { "./src/Data/Shaders/textured.vert", "./src/Data/Shaders/textured.frag" };
		s.diffuse = { 1,1,1 };
		s.specular = { 0.2f, 0.2f, 0.2f };
		t = { "./src/Data/Textures/Dirt.png" };
		t1 = { "./src/Data/Textures/awesomeface.png" };
		colored = { "./src/Data/Shaders/colored.vert", "./src/Data/Shaders/colored.frag" };
		colored.diffuse = { 0.9f, 0.2f, 0.2f };
		colored.specular = { .5f, .5f, .5f };
#else
		s = { "Data/Shaders/textured.vert", "Data/Shaders/textured.frag" };
		//t = { "Data/Textures/Dirt.png" };
		colored = { "Data/Shaders/colored.vert", "Data/Shaders/colored.frag" };
		sound->Path("Data/Audio/breakout.mp3");
#endif
		snd.LoadSource("./src/Data/Audio/breakout.mp3");
		//snd.Play();
		//Meshes & Scene
		m = { Mesh::s_verticesForCube, Mesh::s_cubeMeshSize, &s };
		m1 = { Mesh::s_verticesForCube, Mesh::s_cubeMeshSize, &colored };
		m.SetTexture(&t);
		//m1.SetTexture(&t1);
		g2.AddComponent<Control>();
		g2.AddChild(&myObject);
		g2.AddChild(&g);
		m.AddGameObject(&g);
		m1.AddGameObject(&myObject);
		m1.AddGameObject(&g2);
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
		if (Input::GetKey(Keys::Key_ESCAPE)) {
			Close();
		}
	}

	void OnEditorUpdate() override {
		if (Input::GetKey(Keys::Key_ESCAPE)) {
			Close();
		}
		/*if (Input::GetMouse(0)) {
			for (auto m : scene._GetMeshes()) {
				float* vertices = m->GetVertices();
				for (auto g : m->_GetGameObjects()) {
					for (int i = 0; i < m->GetVertexSize() / 24; i++) {
						glm::vec4 v1 = { vertices[i * 24], vertices[i * 24 + 1], vertices[i * 24 + 2], 1 };
						glm::vec4 v2 = { vertices[i * 24 + 8], vertices[i * 24 + 9], vertices[i * 24 + 10],1 };
						glm::vec4 v3 = { vertices[i * 24 + 16], vertices[i * 24 + 17], vertices[i * 24 + 18],1 };
						glm::vec4 v0_ = g->GetTransformationMatrix() * v1;
						glm::vec4 v1_ = g->GetTransformationMatrix() * v2;
						glm::vec4 v2_ = g->GetTransformationMatrix() * v3;
						if (UnprojectRayCast(glm::vec3(g->position.x, g->position.y, g->position.z), 0.65f, Vector3f(v0_.x, v0_.y, v0_.z), Vector3f(v1_.x, v1_.y, v1_.z), Vector3f(v2_.x, v2_.y, v2_.z))) {
							selectedObject = g;
							break;
						}
					}
				}
			}
		}*/

	}

	void OnClose() override {
		delete[] objs;
	}

};

FSY::Application* FSY::CreateApplication() {
	return new App(1000, 800, "My App");
}