#include "Framework\WinGameTimer.h"
#include "Common\MemoryManager.h"
#include "Common\Log.h"
#include "Rendering\RenderingInitData.h"
#include "GameApp.h"
#include <iostream>
#include <windows.h>

#if BEY_USE_OPENGL
#include <glew.h>
#endif

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
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // tell Windows to redraw the window if it is moved vertically or horizontally and also allocates unique device context for each window in the class (necessary for openGL)
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

	BEY_LOG("Finished Creating Main Window");	
}

void MainLoop(bey::GameApp* app) 
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
			app->CalculateFps(timer.DeltaTime());

			//simple game loop
			app->Update(timer.DeltaTime());
			app->Render();
		}		
	}	
}

void SetupOpenGl(HWND hWnd)
{
	BEY_LOG("Creating OpenGL context ...");
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
		32,                        //Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                        //Number of bits for the depthbuffer
		8,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	HDC ourWindowHandleToDeviceContext = GetDC(hWnd);

	int  letWindowsChooseThisPixelFormat;
	letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd);
	SetPixelFormat(ourWindowHandleToDeviceContext, letWindowsChooseThisPixelFormat, &pfd);

	HGLRC ourOpenGLRenderingContext = wglCreateContext(ourWindowHandleToDeviceContext);
	wglMakeCurrent(ourWindowHandleToDeviceContext, ourOpenGLRenderingContext);

	//MessageBoxA(0, (char*) glGetString(GL_VERSION), "OPENGL VERSION", 0);
	BEY_LOGF("Open GL context created. OPENGL_VERSION %s\n", glGetString(GL_VERSION));
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is created	
		case WM_CREATE:
		{
#if BEY_USE_OPENGL
			// create open gl context
			SetupOpenGl(hWnd);
#endif
		} break;

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

	bey::GameApp app;

	//initialize app
	app.Init(screenWidth, screenHeight, hWnd);

	// enter the main loop:
	MainLoop(&app);

	// clean app
	app.Clean();

	bey::MemoryManager::GetInstance()->Dump();
	printf("Press any key...\n");
	char c;
	scanf("%c", &c);

	return 0;
}
