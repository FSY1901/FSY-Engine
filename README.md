# FSY Engine
 A 3D Game Engine.

The Engine can be used and modified for free. \n
Though libaries are used, that have a license that you must always follow. \n
These libaries can be found under: FSY\src\vendor. Their respective license/README
can be found in the libaries folder(the one in the vendor folder).

There are two main folders: The 'App'-folder & the 'FSY'-folder.\n
In the App-folder you can just find an example on how to use the Engine in App.cpp
(Though you can't expect to find nice code all the time as it's just used for testing
purposes!). \n
The FSY-folder includes the files for the Engine.

To use the Engine do the following steps: \n
-Grab FSY.dll & FSY.lib from FSY/bin/Debug-x64/FSY \n
-Link the libary \n
-In your project under Properties->C/C++->Preprocessor: \n
	-under Preprocessor Definitions add: FSY_PLATFORM_WINDOWS; \n
-Then under roperties->C/C++->General under Addition include directories
 add the src folder from the FSY folder. It should
 look something like this: (PathToYourProject)FSY\src;
-In your Project create a src folder \n
-Create an App.cpp \n
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
 into the output directory of your folder \n
-Copy the Data folder you find under App\src into
 your src folder(or create a Data folder and just
 paste the Assets folder that you find in the Data
 folder as you don't need the textures because
 they have just been used for debug purposes in
 development) \n
-Run the Application \n

Everything should work now and you should see a grey
background and the UI.