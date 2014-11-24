/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include <SDL.h> 
#include <Vector2.h>
#include <Vector3.h>
#include <CBuffer.h>
#include <string>
#include "Texture.h"

namespace Pro{
	namespace Asset{
		using namespace std;
		using namespace Math; 

		class Sprite 
		{
		private:  
			Vector2 center;

			Vector2 dimensions;
			Vector3 position;

			Texture* m_texture; 
		public: 
			Sprite(Texture*); 
			~Sprite();

			Texture* getTexture();
			Vector2& getCenter();

			Vector3& getPosition();
			Vector2& getDimensions();

			void setPosition(const Vector3&);
			void setDimensions(const Vector2&);

		};
	}
}