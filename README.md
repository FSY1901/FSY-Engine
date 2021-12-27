# FSY Engine
 A 3D Game Engine.

## Info

The Engine can be used and modified for free. <br />
Though libaries are used, that have a license that you must always follow. <br />
These libaries can be found under: FSY\src\vendor. Their respective license/README
can be found in the libaries folder(the one in the vendor folder).

## App & FSY

There are two main folders: The 'App'-folder & the 'FSY'-folder. <br />
In the App-folder you can just find an example on how to use the Engine in App.cpp
(Though you can't expect to find nice code all the time as it's just used for testing
purposes!). <br />
The FSY-folder includes the files for the Engine.

## Setup

To setup the Engine follow these steps: <br />
-Grab FSY.dll & FSY.lib from FSY/bin/Debug-x64/FSY or/and FSY/bin/Release-x64/FSY <br />
-Link the libary <br />
-In your project under Properties->C/C++->Preprocessor: <br />
	-under Preprocessor Definitions add: FSY_PLATFORM_WINDOWS; <br />
-Then under Properties->C/C++->General under Additional include directories
 add the src folder from the FSY folder. It should
 look something like this: (PathToYourProject)FSY\src; <br />
-In your Project create a src folder <br />
-Create an App.cpp <br />
-Add the following code:

	#include "FSY.h"
	#include <iostream>

	using namespace FSY;

	class App : public FSY::Application {

	public:

		Scene scene;

		App() {

		}

		App(int width, int height, const char* title) : FSY::Application(width, height, title) {
		
		}

		~App() override {

		}

		void OnStart() override {
			inEditor = true;
			ChangeScene(&scene);
		}

		void OnUpdate() override {
			
		}

		void OnEditorUpdate() override {
		
		}

		void OnClose() override {
		
		}
	
	};

	FSY::Application* FSY::CreateApplication() {
		return new App(800, 600, "My App");
	}

-Copy all the dll files you find under bin\Debug-x64\App
 into the output directory of your folder <br />
-Copy the Data folder you find under App\src into
 your src folder(or create a Data folder and just
 paste the Assets folder that you find in the Data
 folder as you don't need the textures because
 they have just been used for debug purposes in
 development) <br />
-Run the Application <br />

Everything should work now and you should see a grey
background and the UI.