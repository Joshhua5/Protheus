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

#include <Vector2.h> 
#include <Vector3.h>
#include <CBuffer.h>
#include <string>
#include "Texture.h"

namespace Pro{
	namespace Graphics{
		using namespace std;
		using namespace Math; 

		class Sprite 
		{
		private:  
			Vector2<float> center;

			Vector2<float> dimensions;
			Vector3<float> position;

			Texture* m_texture = nullptr;
		public: 
			Sprite(Texture*); 
			~Sprite();

			Texture* getTexture();
			Vector2<float>& getCenter();

			Vector3<float>& getPosition();
			Vector2<float>& getDimensions();

			void setPosition(const Vector3<float>&);
			void setDimensions(const Vector2<float>&);

		};
	}
}