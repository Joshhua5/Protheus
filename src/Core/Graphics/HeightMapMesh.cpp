/*
 *
 * Original Author: Geneva Smith
 *
 * Major performance and compliance edits have been made
 */

#include "HeightMapMesh.h"

 //Protheus libraries
#include "OpenGL.h"
#include <Buffer.h>
#include <BufferWriter.h>
#include <ProMath.h>
#include <SIMD.h>

//C++ libraries
#include <cmath>

using namespace std;
using namespace Pro;
using namespace Graphics;
using namespace Util;

//Constant values
const float DISPLACEMENT = 0.3f;
const int MAX_RADIUS = 20;

//Maximum height to calculate colours

/*	Class Constructor	*/
/*
 * The class constructor sets the private class variables for width and depth to the parameters given to it
 * by the user. It then dynamically creates three 2D arrays - one for each heights. face ormals and vertex
 * normals. It fills in the height cells with an intial value of zero in preparation for the summations
 * computed by the terrain algorithm.
 */

#if defined(__SSE__)
inline __m128 abs_ps(__m128 x) {
	static const __m128 sign_mask = _mm_set1_ps(-0.f); // -0.f = 1 << 31
	return _mm_andnot_ps(sign_mask, x);
}
#endif

#if defined(__AVX__)
inline __m256 avx_abs_ps(__m256 x) {
	static const __m256 sign_mask = _mm256_set1_ps(-0.f);
	return _mm256_andnot_ps(sign_mask, x);
}
#endif

inline void prefetch(void* a) {
#if defined(SSE) || defined(AVX)
	_mm_prefetch(a, _MM_HINT_T0);
#endif
}


void HeightMesh::scalar_height(int numCircles, float& maxHeight) {
	for (int a = 0; a < numCircles; a++) {
		//Get Circle Posititon and radius
		const int x = rand() % mapWidth;
		const int z = rand() % mapLength;
		const int radius = rand() % MAX_RADIUS + 1;
		const float displacement2 = DISPLACEMENT / 2.0f;
		// Calculate area of effect

		const int startx = (x - radius < 0) ? 0 : (x - radius);
		const int endx = (x + radius > mapWidth) ? mapWidth : (x + radius);
		const int startz = (z - radius < 0) ? 0 : (z - radius);
		const int endz = (z + radius > mapLength) ? mapLength : (z + radius);
		prefetch(&heights[startz][startx]);

		//Each terrain point inside of the boundary
		for (int i = startz; i < endz; i++)
		{
			const int idz = z - i;
			// Start the prefetch for the next iteration
			prefetch(&heights[i + 1][startx]);
			for (int j = startx; j < endx; ++j)
			{
				const int dx = x - j;

				//Calculate distance between point and circle centre
				const float distance = sqrtf(float((idz*idz) + (dx*dx)));
				const float test = distance / radius;

				if (fabs(test) <= 0.5f) {
					const float cosVal = cos(test * PI / 90);
					heights[i][j] += (displacement2 + (cosVal * displacement2)) * smoothing_;

					if (heights[i][j] > maxHeight)
						maxHeight = heights[i][j];
				}
			}
		}
	}
	// Scale the height to max_height
	const float scale_factor = (maxHeight != 0) ? max_height_ / maxHeight : 0;
	if (scale_factor != 1.f) {
		for (int i = 0; i < mapWidth; ++i)
			for (int j = 0; j < mapLength; ++j)
				heights[i][j] *= scale_factor;

	}
}

void HeightMesh::sse_height(int numCircles, float& maxHeight) {
#ifdef __SSE__
	// SSE constants
	const static float displacement2 = DISPLACEMENT / 2.f;
	const static float pi90 = PI / 90.f;
	alignas(16) const static __m128 sse_displacement2 = _mm_set1_ps(displacement2);
	alignas(16) const static __m128 sse_pi90 = _mm_set1_ps(pi90);
	alignas(16) const static __m128 sse_delta = { 0, 1, 2, 3 };

	//Adjust height values
	for (int a = 0; a < numCircles; a++)
	{
		//Get Circle Posititon and radius
		const int x = rand() % mapWidth;
		const int z = rand() % mapLength;
		const int radius = rand() % MAX_RADIUS + 1;
		const __m128 sse_radius = _mm_set1_ps((float)radius);
		// Calculate area of effect

		const int startx = (x - radius < 0) ? 0 : (x - radius);
		const int endx = (x + radius > mapWidth) ? mapWidth : (x + radius);
		const int startz = (z - radius < 0) ? 0 : (z - radius);
		const int endz = (z + radius > mapLength) ? mapLength : (z + radius);
		prefetch(&heights[startz][startx]);

		//Each terrain point inside of the boundary
		for (int i = startz; i < endz; i++)
		{
			const __m128 dz = _mm_set1_ps(static_cast<float>(z - i));
			const int idz = z - i;
			// Start the prefetch for the next iteration
			prefetch(&heights[i + 1][startx]);
			for (int j = startx; j < endx;)
			{
				if (mapWidth - endx < 4) {
					const int dx = x - j;

					//Calculate distance between point and circle centre
					const float distance = sqrtf(float((idz*idz) + (dx*dx)));
					const float test = distance / radius;

					if (fabs(test) <= 0.5f) {
						const float cosVal = cos(test * pi90);
						heights[i][j] += (displacement2 + (cosVal * displacement2)) * smoothing_;

						if (heights[i][j] > maxHeight)
							maxHeight = heights[i][j];
					}
					++j;
				}
				else {
					//Calculate y of points
					alignas(16) const __m128 dx = _mm_sub_ps(_mm_set1_ps((float)x - j), sse_delta);

					//Calculate distance between point and circle centre
					alignas(16) const __m128 distance = _mm_sqrt_ps(_mm_add_ps(_mm_mul_ps(dz, dz), _mm_mul_ps(dx, dx)));
					alignas(16) const __m128 test = _mm_div_ps(distance, sse_radius);
					// Determine which test values are above 1 and create a bitmask
					alignas(16) const __m128 comp = _mm_cmpgt_ps(_mm_set1_ps(0.5f), abs_ps(test));
					if (_mm_movemask_ps(comp) != 0) {
						const __m128 original_height = _mm_loadu_ps(&heights[i][j]);
						const __m128 cosVal = cos_ps(_mm_mul_ps(test, sse_pi90));

						const __m128 delta_height = _mm_add_ps(sse_displacement2, _mm_mul_ps(cosVal, sse_displacement2));
						const __m128 scaled_height = _mm_mul_ps(delta_height, _mm_set1_ps(smoothing_));
						const __m128 new_height = _mm_add_ps(_mm_and_ps(comp, scaled_height), original_height);
						_mm_storeu_ps(&heights[i][j], new_height);

						// Check if one of the new heights is larger that the maxHeight
						alignas(16) const __m128 max_comp = _mm_cmpgt_ps(new_height, _mm_set1_ps(maxHeight));
						if (_mm_movemask_ps(max_comp) != 0) {
							for (int x = 0; x < 4; ++x)
								if (heights[i][j + x] > maxHeight)
									maxHeight = heights[i][j + x];
						}
					}
					j += 4;
				}
			}
		}
	}
	// Scale the height to max_height
	const float scale_factor = (maxHeight != 0) ? max_height_ / maxHeight : 0;
	__m128 sse_scale_factor = _mm_set1_ps(scale_factor);
	if (scale_factor != 1.f) {
		for (int i = 0; i < mapWidth; ++i) {
			for (int j = 0; j < mapLength; j += 4) {
				if (mapLength - j > 4)
					_mm_storeu_ps(&heights[i][j], _mm_mul_ps(_mm_loadu_ps(&heights[i][j]), sse_scale_factor));
				else
					for (; j < mapLength; ++j)
						heights[i][j] *= scale_factor;
			}
		}
	}
#endif
}

void HeightMesh::avx_height(int numCircles, float& maxHeight) {
#ifdef __AVX__
	// SSE constants
	const static float displacement2 = DISPLACEMENT / 2.f;
	const static float pi90 = PI / 90.f;
	alignas(32) const static __m256 avx_displacement2 = _mm256_set1_ps(displacement2);
	alignas(32) const static __m256 avx_pi90 = _mm256_set1_ps(pi90);
	alignas(32) const static __m256 avx_delta = { 0, 1, 2, 3, 4, 5, 6, 7 };
	//Adjust height values
	for (int a = 0; a < numCircles; a++)
	{
		//Get Circle Posititon and radius
		x = rand() % mapWidth;
		z = rand() % mapLength;
		const float radius = 10 % MAX_RADIUS + 1;
		const __m256 avx_radius = _mm256_set1_ps(radius);
		// Calculate area of effect

		const int startx = (x - radius < 0) ? 0 : (x - radius);
		const int endx = (x + radius > mapWidth) ? mapWidth : (x + radius);
		const int startz = (z - radius < 0) ? 0 : (z - radius);
		const int endz = (z + radius > mapLength) ? mapLength : (z + radius);
		prefetch(&heights[startz][startx]);

		//Each terrain point inside of the boundary
		for (int i = startz; i < endz; i++)
		{
			const __m256 dz = _mm256_set1_ps(z - i);
			const int idz = z - i;
			// Start the prefetch for the next iteration
			prefetch(&heights[i + 1][startx]);
			for (int j = startx; j < endx;)
			{
				if (mapWidth - endx < 8) {
					const int dx = x - j;

					//Calculate distance between point and circle centre
					const float distance = sqrtf(float((idz*idz) + (dx*dx)));
					const float test = distance / radius;

					if (fabs(test) <= 0.5f) {
						const float cosVal = cos(test * pi90);
						heights[i][j] += (displacement2 + (cosVal * displacement2)) * smoothing_;

						if (heights[i][j] > maxHeight)
							maxHeight = heights[i][j];
					}
					++j;
				}
				else {
					alignas(32) const __m256 dx = _mm256_sub_ps(_mm256_set1_ps(x - i), avx_delta);

					alignas(32) const __m256 distance = _mm256_sqrt_ps(_mm256_add_ps(_mm256_mul_ps(dz, dz), _mm256_mul_ps(dx, dx)));
					alignas(32) const __m256 test = _mm256_div_ps(distance, avx_radius);
					alignas(32) const __m256 comp = _mm256_cmp_ps(avx_abs_ps(test), _mm256_set1_ps(0.5f), _CMP_GE_OS);
					if (_mm256_movemask_ps(comp) != 0) {
						const __m256 original_height = _mm256_loadu_ps(&heights[i][j]);
						const __m256 cosVal = cos256_ps(_mm256_mul_ps(test, avx_pi90));

						const __m256 delta_height = _mm256_add_ps(avx_displacement2, _mm256_mul_ps(cosVal, avx_displacement2));
						const __m256 scaled_height = _mm256_mul_ps(delta_height, _mm256_set1_ps(smoothing_));
						const __m256 new_height = _mm256_add_ps(_mm256_and_ps(comp, scaled_height), original_height);
						_mm256_storeu_ps(&heights[i][j], new_height);

						// Check if one of the new heights is larger that the maxHeight
						const __m256 max_comp = _mm256_cmp_ps(_mm256_set1_ps(maxHeight), new_height, _CMP_GE_OS);
						if (_mm256_movemask_ps(max_comp) != 0) {
							for (int x = 0; x < 8; ++x)
								if (heights[i][j + x] > maxHeight)
									maxHeight = heights[i][j + x];
						}
					}
					j += 8;
				}
			}
		}

	}// Scale the height to max_height
	const float scale_factor = (maxHeight != 0) ? max_height_ / maxHeight : 0;
	__m256 avx_scale_factor = _mm256_set1_ps(scale_factor);
	if (scale_factor != 1.f) {
		for (int i = 0; i < mapWidth; ++i) {
			for (int j = 0; j < mapLength; j += 8) {
				if (mapLength - j > 8) 
					_mm256_storeu_ps(&heights[i][j], _mm256_mul_ps(_mm256_loadu_ps(&heights[i][j]), avx_scale_factor));
				else 
					for (; j < mapLength; ++j)
						heights[i][j] *= scale_factor; 
			}
		}
	}
#endif
}

HeightMesh::HeightMesh()
{
	SIMDCall<void(HeightMesh::*)(int, float&)>(nullptr, &HeightMesh::avx_height, &HeightMesh::sse_height, &HeightMesh::scalar_height, generation_simd);

	mapWidth = 0;
	mapLength = 0;

	smoothing(0.1f);
	height(1.f);

	vertex_buffer = 0;
	vertex_buffer_count = 0;

	heights = nullptr;
	faceNormals = nullptr;
	vertexNormals = nullptr;
}

/*	Class Destructor	*/
HeightMesh::~HeightMesh()
{
	destroy();
	if (vertex_buffer != 0)
		glDeleteBuffers(1, &vertex_buffer);
}

void HeightMesh::destroy() {
	if (heights != nullptr) {
		for (int i = 0; i < mapWidth; i++)
			delete[] heights[i];
		delete[] heights;
		heights = nullptr;
	}
	if (vertexNormals != nullptr) {
		for (int i = 0; i < mapLength; ++i)
			delete[] vertexNormals[i];
		delete[] vertexNormals;
		vertexNormals = nullptr;
	}
	if (faceNormals != nullptr) {
		for (int i = 0; i < mapLength - 1; ++i)
			delete[] faceNormals[i];
		delete[] faceNormals;
		faceNormals = nullptr;
	}
}


void HeightMesh::init(int x, int z) {
	destroy();
	mapWidth = x;
	mapLength = z;
	faceIndex = (mapWidth - 1) * 2; 

	//Creating a 2D array dynamically to match map xz plane
	heights = new float *[mapLength];

	for (int i = 0; i < mapLength; i++)
		heights[i] = new float[faceIndex];

	//Initializing the height values to 0 for summations later
	for (int j = 0; j < mapLength; j++)
		for (int k = 0; k < mapWidth; k++)
			heights[j][k] = 0;

	//Dynamic array creation for Face normals

	//Dynamic array creation for normals
	faceNormals = new Vector3<float> *[mapLength - 1]; // Face Normals
	vertexNormals = new Vector3 <float> *[mapLength]; // Point Normals
	for (int i = 0; i < mapLength - 1; i++) {
		vertexNormals[i] = new Vector3<float>[mapWidth];
		faceNormals[i] = new Vector3<float>[faceIndex];
	}
	// vertexNormals has one more array on the end
	vertexNormals[mapLength - 1] = new Vector3<float>[mapWidth];

}

int HeightMesh::length() const
{
	return mapLength;
}

int HeightMesh::width() const
{
	return mapWidth;
}

float HeightMesh::height() const
{
	return max_height_;
}

void HeightMesh::seed(unsigned x) {
	seed_ = x;
}

unsigned HeightMesh::seed() const {
	return seed_;
}

float HeightMesh::smoothing() const {
	return smoothing_;
}

float HeightMesh::noise() const {
	return noise_;
}

void HeightMesh::height(float x) {
	max_height_ = x;
}

void HeightMesh::smoothing(float x) {
	smoothing_ = x;
}

void HeightMesh::Bind() const{
    vertex_array.Bind();
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
}

void HeightMesh::noise(float x) {
	noise_ = x;
}


/*	Drawing Functions	*/
/*
 * These functions are used to draw the actual height mesh. It allows for toggling between shading types
 * flat or Gouraud) and between wireframe modes (on or off).
 * The overview function is used to draw a 2D overhead view of the terrain. It is coloured in the same fashion
 * as the 3D map.
 */

void HeightMesh::GenerateBuffer(bool shadeflat, const Program& program, float width, float depth)
{
	Buffer data;
	data.Resize(mapLength * mapWidth * sizeof(Vector3<float>) * 6);
	BufferWriter writer(&data);

	const float resolutionX = (width == 0.f) ? 1.f : (width / mapLength);
	const float resolutionZ = (depth == 0.f) ? 1.f : (depth / mapWidth);

	 Vector3<float>** normal_source = faceNormals;

	for (int i = 0; i < mapLength - 2; i++)
	{
		//Triangle strips
		for (int j = 0; j < mapWidth; j++)
		{
			// Color
			writer.Write<Vector3<float>>({ 0, heights[i][j], 0 });

			//Normal
			writer.Write<Vector3<float>>({ normal_source[i][j].x, normal_source[i][j].y, normal_source[i][j].z });

			//Vertex
			writer.Write<Vector3<float>>({ i * resolutionX, heights[i][j], j * resolutionZ });

			// Color
			writer.Write<Vector3<float>>({ 0, heights[i + 1][j], 0 });

			//Normals
			writer.Write<Vector3<float>>({ normal_source[i + 1][j].x, normal_source[i + 1][j].y, normal_source[i + 1][j].z });

			//Vertex
			writer.Write<Vector3<float>>({ (i + 1) * resolutionX, heights[i + 1][j], j * resolutionZ });
		}
	}

	vertex_array.Bind();

	if (vertex_buffer != 0)
		glDeleteBuffers(1, &vertex_buffer);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, writer.head(), data.data<const void>(), GL_STATIC_DRAW);
	vertex_buffer_count = static_cast<GLuint>(writer.head() / (sizeof(Vector3<float>) * 3));

	vertex_array.setVertexAttribute(program, "in_color", 3, GL_FLOAT, GL_FALSE, sizeof(Vector3<float>) * 3, 0);
	vertex_array.setVertexAttribute(program, "in_normal", 3, GL_FLOAT, GL_FALSE, sizeof(Vector3<float>) * 3, sizeof(Vector3<float>));
	vertex_array.setVertexAttribute(program, "in_vertex", 3, GL_FLOAT, GL_FALSE, sizeof(Vector3<float>) * 3, sizeof(Vector3<float>) * 2);
}

void HeightMesh::Render(bool wireframe) const {
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Bind();
	for (int i = 0; i < mapLength - 1; i++)
		glDrawArrays(GL_TRIANGLE_STRIP, i * mapWidth * 2, mapWidth * 2);

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/*	Terrain Algorithm
 * These functions intialiaize the terrain. filling in height values and computing the face and vertex normals.
 * These should only be called once by the user, unless they want to compound the effects of the terrain
 * generator.
 */

inline Vector3<float> ComputeUnitNormal(const Vector3<float>& p1, const Vector3<float>& p2, const Vector3<float>& p3)
{
	Vector3<float> u, v;
	Vector3<float> normal;

	//u vector
	u = p1 - p2;

	//v vector
	v = p3 - p2;

	//Normal
	normal.x = u.y * v.z - u.z * v.y;
	normal.y = u.z * v.x - u.x * v.z;
	normal.z = u.x * v.y - u.y * v.x;

	return normal.NormalizeThis() * -1.f;
}

void HeightMesh::CalculateNormals() {
	Vector3<float> point;
	Vector3<float> p1, p2, p3;
	Vector3<float> face;

	for (int i = 0; i < mapLength - 1; i++)
	{
		for (int j = 0; j < mapWidth - 1; j++)
		{  
			p1 = { i + 1.f, heights[i + 1][j], (float)j };
			p2 = { (float)i, heights[i][j], (float)j };
			p3 = { (float)i, heights[i][j + 1], j + 1.f };
			faceNormals[i][j] = ComputeUnitNormal(p1, p2, p3);

			p1 = { (float)i, heights[i][j + 1], j + 1.f };
			p2 = { i + 1.f, heights[i + 1][j], (float)j };
			p3 = { i + 1.f, heights[i + 1][j + 1], j + 1.f };
			faceNormals[i][j + 1] = ComputeUnitNormal(p1, p2, p3);
		}
	}

	//First row
	vertexNormals[0][0] = faceNormals[0][0];

	//Middle points first row
	for (int i = 1; i < mapWidth - 1; i++)
	{
		const int index = i * 2;
		point.x = (faceNormals[0][index - 2].x + faceNormals[0][index - 1].x + faceNormals[0][index].x) / 3.f;
		point.y = (faceNormals[0][index - 2].y + faceNormals[0][index - 1].y + faceNormals[0][index].y) / 3.f;
		point.z = (faceNormals[0][index - 2].z + faceNormals[0][index - 1].z + faceNormals[0][index].z) / 3.f;

		vertexNormals[0][i] = point;
	}

	//Last point, first row
	point.x = (faceNormals[0][mapWidth - 4].x + faceNormals[0][mapWidth - 3].x) / 2.f;
	point.y = (faceNormals[0][mapWidth - 4].y + faceNormals[0][mapWidth - 3].y) / 2.f;
	point.z = (faceNormals[0][mapWidth - 4].z + faceNormals[0][mapWidth - 3].z) / 2.f;

	vertexNormals[0][mapWidth - 1] = point;

	//Middle rows
	for (int i = 1; i < mapLength - 1; i++)
	{
		//First point in the row
		point.x = (faceNormals[i - 1][0].x + faceNormals[i - 1][1].x + faceNormals[i][0].y) / 3.f;
		point.y = (faceNormals[i - 1][0].y + faceNormals[i - 1][1].y + faceNormals[i][0].y) / 3.f;
		point.z = (faceNormals[i - 1][0].z + faceNormals[i - 1][1].z + faceNormals[i][0].z) / 3.f;

		vertexNormals[i][0] = point;

		//Middle row points (columns)
		for (int j = 1; j < mapWidth - 1; j++)
		{
			const int index = j * 2;

			point.x = (faceNormals[i - 1][index - 1].x + faceNormals[i - 1][index].x + faceNormals[i - 1][index + 1].x + faceNormals[i][index - 2].x + faceNormals[i][index - 1].x + faceNormals[i][index].x) / 6.f;
			point.y = (faceNormals[i - 1][index - 1].y + faceNormals[i - 1][index].y + faceNormals[i - 1][index + 1].y + faceNormals[i][index - 2].y + faceNormals[i][index - 1].y + faceNormals[i][index].y) / 6.f;
			point.z = (faceNormals[i - 1][index - 1].z + faceNormals[i - 1][index].z + faceNormals[i - 1][index + 1].z + faceNormals[i][index - 2].z + faceNormals[i][index - 1].z + faceNormals[i][index].z) / 6.f;

			vertexNormals[i][j] = point;
		}

		//Last row point
		point.x = (faceNormals[i - 1][(mapWidth) - 3].x + faceNormals[i][(mapWidth) - 4].x + faceNormals[i][(mapWidth) - 3].x) / 3.f;
		point.y = (faceNormals[i - 1][(mapWidth) - 3].y + faceNormals[i][(mapWidth) - 4].y + faceNormals[i][(mapWidth) - 3].y) / 3.f;
		point.z = (faceNormals[i - 1][(mapWidth) - 3].z + faceNormals[i][(mapWidth) - 4].z + faceNormals[i][(mapWidth) - 3].z) / 3.f;

		vertexNormals[i][mapWidth - 1] = point;
	}

	//Last row, first point
	point.x = (faceNormals[mapLength - 2][0].x + faceNormals[mapLength - 2][1].x) / 2.f;
	point.y = (faceNormals[mapLength - 2][0].y + faceNormals[mapLength - 2][1].y) / 2.f;
	point.z = (faceNormals[mapLength - 2][0].z + faceNormals[mapLength - 2][1].z) / 2.f;

	vertexNormals[mapLength - 1][0] = point;

	//Last row, middle points
	for (int i = 1; i < mapWidth - 1; i++)
	{
		const int index = i * 2;

		point.x = (faceNormals[mapLength - 2][index - 1].x + faceNormals[mapLength - 2][index].x + faceNormals[mapLength - 2][index + 1].x) / 3.f;
		point.y = (faceNormals[mapLength - 2][index - 1].y + faceNormals[mapLength - 2][index].y + faceNormals[mapLength - 2][index + 1].y) / 3.f;
		point.z = (faceNormals[mapLength - 2][index - 1].z + faceNormals[mapLength - 2][index].z + faceNormals[mapLength - 2][index + 1].z) / 3.f;

		vertexNormals[mapLength - 1][i] = point;
	}

	//Last row, last point
	vertexNormals[mapLength - 1][mapWidth - 1] = faceNormals[mapLength - 2][(mapWidth) - 3];
}

void HeightMesh::GenerateTerrain(int x, int z)
{
	init(x, z);

	//Determine number of hills in terrain
	const int area = mapWidth * mapLength;
	float maxHeight = 0;

	srand(seed_);

	const int numCircles = rand() % area + (int)ceil(area / 1000.f) * mapWidth / 20;

	// calls the appropriate SIMD version of terrain generation
	// The function pointer is defined at the the construction of
	// HeightMesh
	(this->*generation_simd)(numCircles, maxHeight);
	CalculateNormals();

	return;
}