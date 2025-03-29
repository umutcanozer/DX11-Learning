#include "ApplicationClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	ApplicationClass* app;
	bool result;


	// Create the system object.
	app = new ApplicationClass();

	if (app->Initialize(800, 600)) {
		app->Go();
	}
	
	// Shutdown and release the system object.
	app->Shutdown();
	delete app;
	app = nullptr;

	return 0;
}