#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// global declarations
IDXGISwapChain *g_SwapChain;             // the pointer to the swap chain interface
ID3D11Device *g_Device;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *g_DeviceContext;           // the pointer to our Direct3D device context
ID3D11RenderTargetView *g_BackBuffer;    // global declaration

// function prototypes
void InitD3D(HWND hWnd);     // sets up and initializes Direct3D
void CleanD3D(void);         // closes Direct3D and releases memory

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd) 
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	// clear out the struct for use
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	swapChainDesc.BufferCount = 1;                                    // one back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used (draw to back buffer)
	swapChainDesc.OutputWindow = hWnd;                                // the window to be used
	swapChainDesc.SampleDesc.Count = 4;                               // how many multisamples (for anti-aliasing, guaranteed support up to 4, minimum 1)
	swapChainDesc.Windowed = TRUE;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the swapChainDesc struct
	D3D11CreateDeviceAndSwapChain(NULL, // use default adapter (there might be more than one graphics adapter)
		D3D_DRIVER_TYPE_HARDWARE, // use GPU hardware for rendering
		NULL,
		NULL, // flags 
		NULL, // feature level list
		NULL, // number of elements in feature level list
		D3D11_SDK_VERSION, // sdk version
		&swapChainDesc, // pointer to pointer to swap chain description struct
		&g_SwapChain, // pointer to pointer to swap chain object
		&g_Device, // pointer to pointer to device object
		NULL, // pointer to feature level variable 
		&g_DeviceContext); // pointer to pointer device context object

	// get the address of the back buffer (number 0)
	ID3D11Texture2D *pBackBuffer;
	g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	g_Device->CreateRenderTargetView(pBackBuffer, NULL, &g_BackBuffer);
	pBackBuffer->Release(); // destroy the com object used to access the back buffer (the back buffer itself is not destroyed)

	// set the render target as the back buffer
	g_DeviceContext->OMSetRenderTargets(1, &g_BackBuffer, NULL);

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	g_DeviceContext->RSSetViewports(1, &viewport);
}

// cleans up Direct3D and COM
void CleanD3D()
{
	// close and release all existing COM objects
	g_SwapChain->Release();
	g_BackBuffer->Release();
	g_Device->Release();
	g_DeviceContext->Release();
}

// this is the function used to render a single frame
void RenderFrame(void)
{
	// clear the back buffer to a deep blue
	g_DeviceContext->ClearRenderTargetView(g_BackBuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	// do 3D rendering on the back buffer here

	// switch the back buffer and the front buffer
	g_SwapChain->Present(0, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, // an integer which identifies this application
	HINSTANCE hPrevInstance, // obsolete
	LPSTR lpCmdLine, // long pointer to the command line string
	int nCmdShow) // how window appear when created
{
	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX); //  size of this structure
	wc.style = CS_HREDRAW | CS_VREDRAW; // tell Windows to redraw the window if it is moved vertically or horizontally
	wc.lpfnWndProc = WindowProc; //  what function to use when it gets a message from Windows
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // stores the default mouse image for the window class
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // 
	wc.lpszClassName = L"WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	RECT wr = { 0, 0, 500, 400 };    // set the client size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		L"WindowClass1",    // name of the window class
		L"Our First Windowed Program",   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		300,    // x-position of the window
		300,    // y-position of the window
		500,    // width of the window
		400,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL

	// display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	// initialize direct3D
	InitD3D(hWnd);

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
		RenderFrame();
	}

	// release direct 3d resources
	CleanD3D();

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
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