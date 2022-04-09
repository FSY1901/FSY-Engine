# FSY Engine
 A 3D Game Engine.

## Info

The FSY Engine is a 3D Engine that can be modified and used for free. <br />
The main purpose of the engine is to be embedded into your Application and to give you an Editor <br />
with which you can edit scenes and other things.

## App & FSY

There are two main folders: The 'App'-folder & the 'FSY'-folder. <br />
"App" only contains App.cpp, whcih is used for testing new features. <br />
"App" also contains the folder "Data", whcih just contains textures or sound files <br />
and the important "Assets"-folder which is needed to display the Editor correctly <br />
The "FSY"-folder includes the files for the Engine.

## Setup

To setup the Engine follow these steps: <br />
-Grab FSY.dll & FSY.lib from FSY/bin/Debug-x64/FSY and FSY/bin/Release-x64/FSY <br />
-Link the libary <br />
-In your project under Properties->C/C++->Preprocessor: <br />
	-under Preprocessor Definitions add: FSY_PLATFORM_WINDOWS; (Yes, the Engine only works for windows(for now)) <br />
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
			//empty
		}

		App(int width, int height, const char* title) : FSY::Application(width, height, title) {
			//empty
		}

		~App() override {

		}

		void OnStart() override {
			//called once at the start of the game
			ChangeScene(&scene);
		}

		void OnUpdate() override {
			//called every frame
		}

		void OnEditorUpdate() override {
			//called every frame (Only in the Editor)
		}

		void OnClose() override {
			//called when the Application is closed
		}
	
	};

	FSY::Application* FSY::CreateApplication() {
		return new App(800, 600, "My App"); // width, height, title
	}

-Copy all the dll files you find under bin\Debug-x64\App
 into the output directory of your folder <br />
-Do the same for bin\Release-x64\App and paste them into your release mode output directory
-Copy the Data folder you find under App\src into
 your src folder(or create a Data folder and just
 paste the Assets folder that you find in the Data
 folder as you don't need the textures because
 they have just been used for debug purposes in
 development) <br />
-Run the Application (in Debug mode) <br />
-If you plan to ship your Application: <br />
	-In your release output directory create a folder named src <br />
	and paste the "Data" folder in there

Everything should work now and you should see a grey
background and the UI.

##Functionality

If you compile in debug mode and run you should get the UI and your scene displayed. <br />
If you compile in release mode and run you should not see the UI and only see the game. <br />