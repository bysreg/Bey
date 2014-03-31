#include "Framework\WinGameTimer.h"
#include "Common\MemoryManager.h"
#include "Common\Log.h"
#include "Rendering\D3DRendering.h"
#include "Rendering\RenderingInitData.h"
#include "GameApp.h"
#include <iostream>
#include <windows.h>

static bey::GameApp s_App;

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

void CreateMainWindow(HWND* hWnd, int screenWidth, int screenHeight) {
	BEY_LOG("Creating Main Window..");		
	
	HINSTANCE hInstance =  GetModuleHandle(NULL);
	WNDCLASSEX wc; // this struct holds information for the window class (wc)

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(wc); //  size of this structure
	wc.style = CS_HREDRAW | CS_VREDRAW; // tell Windows to redraw the window if it is moved vertically or horizontally
	wc.lpfnWndProc = WindowProc; //  what function to use when it gets a message from Windows
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // stores the default mouse image for the window class	 
	wc.lpszClassName = L"WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	DWORD windowStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

	RECT wr = { 0, 0, screenWidth, screenHeight };    // set the client size, but not the position
	AdjustWindowRect(&wr, windowStyle, FALSE);    // adjust the size (so that the border is also counted for the size)

	// create the window and use the result as the handle
	*hWnd = CreateWindowEx(NULL,
		L"WindowClass1",    // name of the window class
		L"Game Title",   // title of the window
		windowStyle,    // window style
		300,    // x-position of the window
		100,    // y-position of the window
		wr.right - wr.left,    // width of the window (including window border)
		wr.bottom - wr.top,    // height of the window (including window border)
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL	

	ShowWindow(*hWnd, TRUE);

	BEY_LOG("Finish Creating Main Window");	
}

void MainLoop() 
{
	// this struct holds Windows event messages
	MSG msg;

	bey::WinGameTimer timer;
	timer.Reset();

	// wait for the next message in the queue, store the result in 'msg'
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);

			// check to see if it's time to quit
			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			timer.Tick();
			s_App.CalculateFps(timer.DeltaTime());

			//simple game loop
			s_App.Update(timer.DeltaTime());
			s_App.Render();
		}		
	}	
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
		case WM_DESTROY:
		{
			// close the application entirely
			PostQuitMessage(0);
			return 0;
		} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int main() {
	int screenWidth = 800, screenHeight = 600;
	HWND hWnd;	

	CreateMainWindow(&hWnd, screenWidth, screenHeight);

	//initialize app
	s_App.Init(screenWidth, screenHeight, hWnd);

	// enter the main loop:
	MainLoop();

	// clean app
	s_App.Clean();

	bey::MemoryManager::GetInstance()->Dump();
	printf("Press any key...\n");
	char c;
	scanf("%c", &c);

	return 0;
}
