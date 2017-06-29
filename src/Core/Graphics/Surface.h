#pragma once
/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 22/02/2016 Waring J.

*************************************************************************/
#include <Vector2.h> 
#include "Color.h"

namespace Pro {
	namespace Graphics { 

		//! A Surface is a CPU side texture  
		class Surface {
			void* data_;
			Math::Vector2<int> dimensions_;

		public:
			Surface(void* data, const Math::Vector2<int>& dimensions) :
				data_(data), dimensions_(dimensions) {}

			//void DrawPoint(const Math::Vector2<int>& point, const Color& color);
			//void DrawLine(const Math::Vector2<int>& start, const Math::Vector2<int>& end, const Color& color, bool aliasing);
			//void DrawCircle(const Math::Vector2<int>& start, int radius, const Color& color);
			 

			// Definitions
			inline void DrawPoint(const Math::Vector2<int>& point, const Color& color) { 
				*(Color*)(static_cast<char*>(data_) + ((dimensions_.x * point.y) + point.x) * 4) = color;
			} 

			void DrawLine(const Math::Vector2<int>& start, const Math::Vector2<int>& end, const Math::Vector4<char>& color) {
				const int dx = end.x - start.x;
				const int dy = end.y - start.y;

				int D = 2 * (dy - dx); 
				auto point = start;
				DrawPoint(point, color);
				if (D > 0) {
					++point.y;
					D -= (2 * dy);
				}
				for (; point.x < end.x; ++point.x) {
					DrawPoint(point, color);
					D += (2 * dy);
					if (D > 0) {
						++point.y;
						D -= (2 * dy);
					}
				} 
			}

		};
	}
}