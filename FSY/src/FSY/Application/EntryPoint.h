#pragma once

#ifdef FSY_PLATFORM_WINDOWS

extern FSY::Application* FSY::CreateApplication();

int main(int argc, char** argv) {
#ifdef CREATE_APP
	auto app = FSY::CreateApplication();
	app->Run();
	delete app;
#endif
}

#endif // FSY_PLATFORM_WINDOWS