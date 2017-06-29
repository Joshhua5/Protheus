/*
 * Geneva Smith
 * Terrrain Generator
 * File: Height Mesh Class Header
 * --------------------------------------
 * This  program defines class functions that will later be implemented in the
 * HeightMapMeshimpl.cpp program.
 */
#pragma once 
#include <vector>
#include "Mesh.h"
#include "VertexArray.h"
class HeightMesh
{
private: 
	float **heights;
    float smoothing_, noise_, max_height_;

	int mapWidth;  //x plane
	int mapLength; //z plane
    unsigned seed_;

    Pro::Math::Vector3<float> **vertexNormals;
    Pro::Math::Vector3<float> **faceNormals;
	int faceIndex;

	GLuint vertex_buffer;
	GLuint vertex_buffer_count;
	Pro::Graphics::VertexArray vertex_array;
    
    void scalar_height(int, float&);
    void sse_height(int, float&);
    void avx_height(int, float&);
    void(HeightMesh::*generation_simd)(int, float&);
    
    void init(int x, int z);
    void destroy(); 
public:

	/*	Constructor and Destructor	*/
	HeightMesh();
	~HeightMesh();

	/*	Value Retrieval	*/
	int width() const;
	int length() const;
	float height() const;
    float noise() const;
    float smoothing() const;
    unsigned seed() const;
    void seed(unsigned);
    
    // Sets the maximum height of the height values
    void height(float);
    // Sets the noise of the generated map
    void noise(float);
    void smoothing(float);
 
    void Bind() const;
    
    // Generates the heightmap data
	void GenerateBuffer(bool shadeflat, const Pro::Graphics::Program& program, float width = 0.f, float depth = 0.f);
    // Overlaps the
    void OverlapMesh(HeightMesh&);
	void Render(bool wireframe = false) const; 

    
    void CalculateNormals();
	/*	Terrain Generation	*/
	void GenerateTerrain(int x, int z);
    
    void PrintGLBuffer() const;
}; 