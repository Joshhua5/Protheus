#pragma once
#include <string>
#include "Entity.h"
namespace Pro{

	class SpriteEntity :
		public Entity
	{
	protected:
		std::string sprite_name;
	public:
		SpriteEntity();
		virtual ~SpriteEntity();

		std::string getSpriteName();

		void setSprite(const std::string&);
	};

}