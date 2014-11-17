#include "Graphics.h" 
#include "glcorearb.h"
 
using namespace Pro;
using namespace Math;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); 
  
Window Pro::Graphics::createWindow(Vector2& position, Vector2& dimensions){

	// Make sure the window is valid
	if (dimensions.x == 0 || dimensions.y == 0){
		error.reportErrorNR("Invalid dimensions for window");
		dimensions.x = 800;
		dimensions.y = 600;
	}

	Window window;

	window.wc.hInstance = window.hInstance = GetModuleHandle(NULL);
	ZeroMemory(&window, sizeof(Window)); 
	 
	window.wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window.wc.lpfnWndProc = (WNDPROC) WndProc;
	window.wc.cbClsExtra = 0;
	window.wc.cbWndExtra = 0; 
	window.wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	window.wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	window.wc.hbrBackground = NULL;
	window.wc.lpszMenuName = NULL;
	window.wc.lpszClassName = "";
	 
	RegisterClassEx(&window.wc); 

	window.hWnd = CreateWindowEx(NULL,
		"",
		"",
		WS_OVERLAPPEDWINDOW,
		position.x, position.y,
		dimensions.x, dimensions.y,
		NULL,
		NULL, 
		window.hInstance, 
		NULL); 

#if PRO_OPENGL
	  
		PIXELFORMATDESCRIPTOR pfd; // Create a new PIXELFORMATDESCRIPTOR (PFD)
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // Clear our  PFD
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Set the size of the PFD to the size of the class
		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // Enable double buffering, opengl support and drawing to a window
		pfd.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels
		pfd.cColorBits = 32; // Give us 32 bits of color information (the higher, the more colors)
		pfd.cDepthBits = 32; // Give us 32 bits of depth information (the higher, the more depth levels)
		pfd.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD

		int nPixelFormat = ChoosePixelFormat(window.hDC, &pfd); // Check if our PFD is valid and get a pixel format back

		SetPixelFormat(window.hDC, nPixelFormat, &pfd); // Try and set the pixel format based on our PFD 
	 
		wglMakeCurrent(NULL, NULL); // Remove the temporary context from being active   
		 
#endif 
}
 