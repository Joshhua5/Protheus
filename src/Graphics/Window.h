#pragma once
#include "glcorearb.h"
#include <Vector2.h>

struct Window
{
#if _WIN32
	HGLRC hRC;
	HDC hDC;
	HWND hWnd;
	HINSTANCE hInstance;
	HGLRC context;
	WNDCLASSEX wc;
#endif 

	bool active;
	bool fullscreen;
	 
public:
	Window();
	~Window();

	void makeCurrent(); 
	void setViewport(Vector2& pos, Vector2& dim); 
	void renderFrame();
};