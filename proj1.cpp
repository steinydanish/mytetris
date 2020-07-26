#include "proj1.h"
#include "Application.h"

#include <iostream>

int main()
{
    Application mainApp;
	mainApp.showWindow();
	mainApp.mainLoop();
	mainApp.cleanup();

	return 0;
}