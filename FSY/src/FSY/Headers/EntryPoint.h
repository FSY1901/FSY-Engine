#pragma once

#ifdef FSY_PLATFORM_WINDOWS

extern FSY::Application* FSY::CreateApplication();

int main(int argc, char** argv) {
	auto app = FSY::CreateApplication();
	app->Run();
	delete app;
}

#endif // FSY_PLATFORM_WINDOWS