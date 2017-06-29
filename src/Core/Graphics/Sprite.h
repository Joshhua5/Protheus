/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include <Vector2.h>
#include <Vector3.h>
#include <Buffer.h>
#include <string>
#include "Texture.h"

namespace Pro{
	namespace Graphics{
		using namespace std;

		class Sprite
		{
		private:
			Vector2<float> center_;

			Vector2<float> dimensions_;
			Vector3<float> position_;

			Texture* texture_ = nullptr;
		public:
			Sprite(Texture*);
			~Sprite();

			Texture* texture();
			Vector2<float>& center();

			Vector3<float>& position();
			Vector2<float>& dimensions();

			void position(const Vector3<float>&);
			void dimensions(const Vector2<float>&);

		};
	}
}
