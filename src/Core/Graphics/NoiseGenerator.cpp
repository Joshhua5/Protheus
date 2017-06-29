//
//  NoiseGenerator.cpp
//  Protheus
//
//  Created by Joshua Waring on 14/05/2015.
//  Copyright (c) 2015 Joshua Waring. All rights reserved.
//

#include "NoiseGenerator.h"

using namespace Pro;
using namespace Util;
using namespace Math;

inline float RandomFloat() {
	return (float)rand() / (float)RAND_MAX;
}

Buffer Pro::Util::GenerateNoise1D(int width, int frequency, float amplitude) {
	int spacing = width / frequency;
	Buffer values(width);
	int index = 0;
	float last_spacing = RandomFloat() * amplitude;
	for (int x = 0; x < frequency; ++x) {
		float next_spacing = RandomFloat() * amplitude;
		for (int i = 0; i < spacing; ++i) {
			// Cosine Interpolation
			float ratio = ((float)i + 1.0f) / (float)spacing;
			*(values.data<float>() + index++) = last_spacing * (1.0f - ratio) + next_spacing * ratio;
		}
		last_spacing = next_spacing;
	}
	return values;
}


Buffer Pro::Util::GeneratePerlinNoise1D(float middle_value, int width, float persistence) {
	float amplitude = 1;
	Buffer graph_1 = GenerateNoise1D(width, 2, amplitude); amplitude *= persistence;
	Buffer graph_2 = GenerateNoise1D(width, 4, amplitude); amplitude *= persistence;
	Buffer graph_3 = GenerateNoise1D(width, 8, amplitude); amplitude *= persistence;
	Buffer graph_4 = GenerateNoise1D(width, 16, amplitude); amplitude *= persistence;
	Buffer graph_5 = GenerateNoise1D(width, 32, amplitude); amplitude *= persistence;
	Buffer graph_6 = GenerateNoise1D(width, 64, amplitude);

	// Merge graphs together

	for (int x = 0; x < width; ++x) {
		// Reuse Graph 1 for one less object creation
		*(graph_1.data<float>() + x) += (
			*(graph_2.data<float>() + x) + 
			*(graph_3.data<float>() + x) + 
			*(graph_4.data<float>() + x) + 
			*(graph_5.data<float>() + x) + 
			*(graph_6.data<float>() + x));
		// Compress the graph to fit between 0 and 1
		// graph_1[x] += middle_value;
	}  
	return graph_1;
}

Buffer Pro::Util::GeneratePerlinNoise2D(const Vector2<int>& dimensions, float persistence,const Vector3<float>& color_multiplier) {
	const int byte_per_pixel = 3; // R8G8B8
	// The texture take unsinged chars (4 per pixel) 
	Buffer pixels(dimensions.x * dimensions.y * byte_per_pixel);
    BufferWriter writer(&pixels);
	Buffer width_noise = GeneratePerlinNoise1D(0.5f, dimensions.x, persistence);
	Buffer height_noise = GeneratePerlinNoise1D(0.5f, dimensions.y, persistence);

	// Interpolate the noises
	for (int x = 0; x < dimensions.x; ++x) {
		for (int y = 0; y < dimensions.y; ++y) {
			writer.Write<char>((char)(((*(height_noise.data<float>() + x) + *(width_noise.data<float>() + y)) * 128.0f) * color_multiplier.x));
			writer.Write<char>((char)(((*(height_noise.data<float>() + x) + *(width_noise.data<float>() + y)) * 128.0f) * color_multiplier.y));
			writer.Write<char>((char)(((*(height_noise.data<float>() + x) + *(width_noise.data<float>() + y)) * 128.0f) * color_multiplier.z));
		}
	} 
	return pixels;
}

Buffer Pro::Util::GeneratePerlinNoise2D(const Vector2<int>& dimensions, float persistence,const Vector4<float>& color_multiplier) {
	const int byte_per_pixel = 4; // R8G8B8A8 
	Buffer pixels(dimensions.x * dimensions.y * byte_per_pixel);
    BufferWriter writer(&pixels);
	Buffer width_noise = GeneratePerlinNoise1D(0.5f, dimensions.x, persistence);
	Buffer height_noise = GeneratePerlinNoise1D(0.5f, dimensions.y, persistence);

	// Interpolate the noises
	for (int x = 0; x < dimensions.x; ++x) {
		for (int y = 0; y < dimensions.y; ++y) {
			writer.Write<char>((char)(((*(height_noise.data<float>() + x) + *(width_noise.data<float>() +y)) * 128.0f) * color_multiplier.x));
			writer.Write<char>((char)(((*(height_noise.data<float>() + x) + *(width_noise.data<float>() +y)) * 128.0f) * color_multiplier.y));
			writer.Write<char>((char)(((*(height_noise.data<float>() + x) + *(width_noise.data<float>() +y)) * 128.0f) * color_multiplier.z));
			writer.Write<char>((char)(((*(height_noise.data<float>() + x) + *(width_noise.data<float>() +y)) * 128.0f) * color_multiplier.w));
		}
	} 
	return pixels;
}