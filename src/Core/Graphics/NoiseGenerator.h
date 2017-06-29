//
//  NoiseGenerator.h
//  Protheus
//
//  Created by Joshua Waring on 14/05/2015.
//  Copyright (c) 2015 Joshua Waring. All rights reserved.
//
 
#include <Buffer.h>
#include <BufferWriter.h>  
#include <Vector2.h>
#include <Vector3.h>
#include <Vector4.h>

namespace Pro{
    namespace Util{ 
        //! http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
        //! Arguments (Smoothness, 
        extern Buffer GenerateNoise1D(int width, int frequency, float amplitude);
        extern Buffer GeneratePerlinNoise1D(float middle_value, int width, float persistence);
		extern Buffer GeneratePerlinNoise2D(const Math::Vector2<int>& dimensions, float persistence, const Math::Vector4<float>& color_multiplier);
		extern Buffer GeneratePerlinNoise2D(const Math::Vector2<int>& dimensions, float persistence, const Math::Vector3<float>& color_multiplier);
    }
}