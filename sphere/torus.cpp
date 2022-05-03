#include <cstdio>
#include <map>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
// #include <glm/gtx/rotate_vector.hpp>

#include "torus.h"

const float PI = 3.14159f;

void Torus::InitUV(float radius1, float radius2, int detailU, int detailV)
{
	unsigned int i, j;

	if (detailV < 2 || detailU < 2)
		return;

	for (j = 0; j < detailV; j++) {
		float fV = (float) j / detailV;
		float height = glm::sin(2 * PI * fV) * radius2;
		float factor2 = glm::cos(2 * PI * fV);
		for (i = 0; i < detailU; i++) {
			float x, y, z, fU;
			float xfactor1, xfactor2, zfactor1, zfactor2;
			glm::vec3 pos;
			fU = (float) i / detailU;

			xfactor1 = glm::cos(2 * PI * fU);
			x = xfactor1 * radius1 + xfactor1 * factor2 * radius2;
			
			zfactor1 = -glm::sin(2 * PI * fU);
			z = zfactor1 * radius1 + zfactor1 * factor2 * radius2;
			
			pos = glm::vec3(x, height, z);
			vertices.push_back(pos);
		}
	}
	
	for (j = 0; j < detailV; j++) {
		for (i = 0; i < detailU; i++) {
			unsigned int index1, index2, index3, index4;
			index1 = (j + 0) * detailV + i + 0;
			index2 = (j + 0) * detailV + ((i + 1) % detailU);
			index3 = ((j + 1) % detailV) * detailV + ((i + 1) % detailU);
			index4 = ((j + 1) % detailV) * detailV + i + 0;
			quads.push_back(glm::uvec4(index1, index2, index3, index4));
		}
	}
	
	radii = glm::vec2(radius1, radius2);
}

void Torus::Triangulate()
{
	for (auto iter = quads.begin(); iter != quads.end(); iter++) {
		int index1 = (*iter)[0];
		int index2 = (*iter)[1];
		int index3 = (*iter)[2];
		int index4 = (*iter)[3];
		
		faces.push_back(glm::uvec3(index1, index2, index3));
		faces.push_back(glm::uvec3(index1, index3, index4));
	}
}

void Torus::InitSA()
{


}

Torus Torus::Double()
{
	float totalSA = 4 * PI * PI * radii[0] * radii[1];
	float averageSA = totalSA / quads.size();
	
	Torus newTorus;
	newTorus.vertices = vertices;
	newTorus.radii = radii;
	
	std::map<unsigned long long, unsigned int> edges;
	
	for (auto iter = quads.begin(); iter != quads.end(); iter++) {
		// Get index of corners
		unsigned long long index1 = (*iter)[0];
		unsigned long long index2 = (*iter)[1];
		unsigned long long index3 = (*iter)[2];
		unsigned long long index4 = (*iter)[3];

		// Calculate surface area
		float hl = glm::length(vertices[index2] - vertices[index1]);
		float vl = glm::length(vertices[index4] - vertices[index1]);
		float sa = hl * vl;
//		printf("sa: %f     averageSA: %f\n", sa, averageSA);
		if (sa > averageSA) {
			// Add two vertices and split quad into two.
			int index5, index6;
			glm::vec3 newVertex;
			if (hl > vl) {
				// Horizontal split
				// Calculate edges
				unsigned long long edge1 = index1 < index2 ? (index1 << 32) + index2 : (index2 << 32) + index1;
				unsigned long long edge2 = index3 < index4 ? (index3 << 32) + index4 : (index4 << 32) + index3;
				
				// Add new vertices
				std::map<unsigned long long, unsigned int>::iterator edgeIter;
				if ((edgeIter = edges.find(edge1)) == edges.end()) {
					index5 = newTorus.vertices.size();
					newVertex = (vertices[index1] + vertices[index2]) / 2.0f;
					newTorus.vertices.push_back(newVertex);
					edges[edge1] = index5;
				}
				else
					index5 = edgeIter->second;
				
				if ((edgeIter = edges.find(edge2)) == edges.end()) {
					index6 = newTorus.vertices.size();
					newVertex = (vertices[index3] + vertices[index4]) / 2.0f;
					newTorus.vertices.push_back(newVertex);
					edges[edge2] = index6;
				}
				else
					index6 = edgeIter->second;
				
				// Add new quads
				newTorus.quads.push_back(glm::uvec4(index1, index5, index6, index4));
				newTorus.quads.push_back(glm::uvec4(index5, index2, index3, index6));
			}
			else {
				// Vertical split
				// Calculate edges
				int edge1 = index1 < index4 ? (index1 << 32) + index4 : (index4 << 32) + index1;
				int edge2 = index2 < index3 ? (index2 << 32) + index3 : (index3 << 32) + index2;
				
				// Add new vertices
				std::map<unsigned long long, unsigned int>::iterator edgeIter;
				if ((edgeIter = edges.find(edge1)) == edges.end()) {
					index5 = newTorus.vertices.size();
					newVertex = (vertices[index1] + vertices[index4]) / 2.0f;
					newTorus.vertices.push_back(newVertex);
					edges[edge1] = index5;
				}
				else
					index5 = edgeIter->second;
				
				if ((edgeIter = edges.find(edge2)) == edges.end()) {
					index6 = newTorus.vertices.size();
					newVertex = (vertices[index2] + vertices[index3]) / 2.0f;
					newTorus.vertices.push_back(newVertex);
					edges[edge2] = index6;
				}
				else
					index6 = edgeIter->second;
				
				// Add new quads
				newTorus.quads.push_back(glm::uvec4(index1, index2, index6, index5));
				newTorus.quads.push_back(glm::uvec4(index5, index6, index3, index4));
			}
		}
		else {
			// Transfer small quad to new torus
			newTorus.quads.push_back(*iter);
		}
	
	}

	return newTorus;
}
