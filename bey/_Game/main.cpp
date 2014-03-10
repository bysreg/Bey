#include "Rendering\IRendering.h"
#include "Rendering\Rendering.h"

#include <windows.h>

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

void CreateMainWindow(const HINSTANCE& hInstance, HWND& hWnd, int screenWidth, int screenHeight) {
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
	hWnd = CreateWindowEx(NULL,
		L"WindowClass1",    // name of the window class
		L"Game Title",   // title of the window
		windowStyle,    // window style
		200,    // x-position of the window
		200,    // y-position of the window
		wr.right - wr.left,    // width of the window (including window border)
		wr.bottom - wr.top,    // height of the window (including window border)
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL	

	ShowWindow(hWnd, TRUE);
}

/*
int WINAPI WinMain(HINSTANCE hInstance, // an integer which identifies this application
	HINSTANCE hPrevInstance, // obsolete
	LPSTR lpCmdLine, // long pointer to the command line string
	int nCmdShow) // how window appear when created
{
	HWND hWnd;
	int screenWidth = 800, screenHeight = 600;

	CreateMainWindow(hInstance, hWnd, screenWidth, screenHeight);	
	ShowWindow(hWnd, nCmdShow);

	// initialize direct3D
	bey::RenderingInitData data;
	data.screenHeight = screenHeight;
	data.screenWidth = screenWidth;
	data.handleWindow = hWnd;
	bey::Rendering::GetInstance().Init(data);

	// enter the main loop:

	// this struct holds Windows event messages
	MSG msg;

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
		bey::Rendering::GetInstance().Render();
	}
	
	bey::Rendering::GetInstance().Clean();

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}
*/

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
	HWND hWnd;
	HINSTANCE hInstance = GetModuleHandle(NULL);;
	int screenWidth = 800, screenHeight = 600;

	CreateMainWindow(hInstance, hWnd, screenWidth, screenHeight);	

	// initialize direct3D
	bey::RenderingInitData data;
	data.screenHeight = screenHeight;
	data.screenWidth = screenWidth;
	data.handleWindow = hWnd;
	bey::Rendering::GetInstance().Init(data);	

	// enter the main loop:

	// this struct holds Windows event messages
	MSG msg;

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
		bey::Rendering::GetInstance().Render();
	}

	bey::Rendering::GetInstance().Clean();

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;

	return 0;
}
