#pragma once
#include <SDL_rect.h>
namespace Pro{
	typedef unsigned int ID;

	class Entity
	{
	protected:
		unsigned int ID;
		SDL_Rect position;
		bool active;
	public:
		Entity();
		virtual ~Entity();
		int getPosX();
		int getPosY();
		// returns an array with the current x an y values, must be deleted after
		int* getPos();

		void setPosX(int);
		void setPosY(int);
		void setPos(int X, int Y);
		SDL_Rect getPositionRect();
		SDL_Rect* getPositionRectP();

		void activate();
		void deactivate();
		bool isActive();

		unsigned int getID();
		void setID(unsigned int);
	};
}
