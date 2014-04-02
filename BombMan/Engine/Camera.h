#pragma once
#include <SDL.h>
namespace Pro{

	class Camera
	{
		// position and width
		SDL_Rect position;
	public:
		Camera();
		~Camera();
		SDL_Rect getPosition();
		void move(int x, int y);
		SDL_Rect* getDimensions();
		void setDimensions(SDL_Rect);
		void setDimensions(int x, int y, int w, int h);
	};
}
