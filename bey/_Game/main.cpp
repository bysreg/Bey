#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, // obsolete
	LPSTR lpCmdLine, // long pointer to the command line string
	int nCmdShow) // how window appear when created
{
	// create a "Hello World" message box using MessageBox()
	MessageBox(NULL,
		"Hello World!",
		"Just another Hello World program!",
		MB_ICONEXCLAMATION | MB_OK);

	return 0;
}