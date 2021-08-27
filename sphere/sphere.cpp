#include "sphere.h"

void Sphere::Init()
{
	const glm::vec3 l_vertices[] = {
			glm::vec3(0, 1, 0),
			glm::vec3(0, 0.4472, 0.8944),
			glm::vec3(0.8506, 0.4472, 0.2763),
			glm::vec3(0.5257, 0.4472, -0.7235),
			glm::vec3(-0.5257, 0.4472, -0.7235),
			glm::vec3(-0.8506, 0.4472, 0.2763),
			glm::vec3(0.5257, -0.4472, 0.7235),
			glm::vec3(0.8506, -0.4472, -0.2763),
			glm::vec3(0, -0.4472, -0.8944),
			glm::vec3(-0.8506, -0.4472, -0.2763),
			glm::vec3(-0.5257, -0.4472, 0.7235),
			glm::vec3(0, -1, 0),
	};
	vertices.assign(l_vertices, l_vertices + sizeof(l_vertices) / sizeof(glm::vec3));

	const glm::uvec3 l_faces[] = {
			glm::uvec3(0, 1, 2),
			glm::uvec3(0, 2, 3), 
			glm::uvec3(0, 3, 4), 
			glm::uvec3(0, 4, 5), 
			glm::uvec3(0, 5, 1), 
			glm::uvec3(1, 10, 6), 
			glm::uvec3(2, 6, 7), 
			glm::uvec3(3, 7, 8), 
			glm::uvec3(4, 8, 9), 
			glm::uvec3(5, 9, 10), 
			glm::uvec3(1, 6, 2), 
			glm::uvec3(2, 7, 3), 
			glm::uvec3(3, 8, 4), 
			glm::uvec3(4, 9, 5), 
			glm::uvec3(5, 10, 1), 
			glm::uvec3(11, 6, 10), 
			glm::uvec3(11, 7, 6), 
			glm::uvec3(11, 8, 7), 
			glm::uvec3(11, 9, 8), 
			glm::uvec3(11, 10, 9), 
	};
	faces.assign(l_faces, l_faces + sizeof(l_faces) / sizeof(glm::uvec3));

	for (auto iter = faces.begin(); iter != faces.end(); iter++) {
		unsigned long long first = (*iter)[0];
		unsigned long long second = (*iter)[1];
		unsigned long long third = (*iter)[2];
		unsigned long long edge1 = first < second ? (first << 32) + second : (second << 32) + first;
		unsigned long long edge2 = second < third ? (second << 32) + third : (third << 32) + second;
		unsigned long long edge3 = third < first ? (third << 32) + first : (first << 32) + third;
		edges[edge1] = 0;
		edges[edge2] = 0;
		edges[edge3] = 0;
	}
}

Sphere Sphere::Quadruple()
{
	Sphere newSphere;

	// Start the new sphere with the current vertices
	newSphere.vertices = vertices;
	
	// Give the new sphere new edges based on current edges
	// Only splits edges. En
	for (auto iter = edges.begin(); iter != edges.end(); iter++) {
		// Get indices of points on this edge
		unsigned long long index1 = iter->first >> 32;
		unsigned long long index2 = iter->first & ((1LL << 32) - 1);
		unsigned int index3 = newSphere.vertices.size();
		
		// Add new vertex to new sphere
		glm::vec3 newVertex = glm::normalize((vertices[index1] + vertices[index2]) / 2.0f);
		newSphere.vertices.push_back(newVertex);
		
		// Get new edges
		unsigned long long newEdge1 = (index1 << 32) + index3;
		unsigned long long newEdge2 = (index2 << 32) + index3;
		
		// Add new edges to new sphere
		newSphere.edges[newEdge1] = 0;
		newSphere.edges[newEdge2] = 0;
		
		// Set value of edge from this sphere to index of new point in new sphere
		iter->second = index3;
	}
	
	for (auto iter = faces.begin(); iter != faces.end(); iter++) {
		// Get old vertices
		unsigned long long first = (*iter)[0];
		unsigned long long second = (*iter)[1];
		unsigned long long third = (*iter)[2];

		// Get old edges
		unsigned long long edge1 = first < second ? (first << 32) + second : (second << 32) + first;
		unsigned long long edge2 = second < third ? (second << 32) + third : (third << 32) + second;
		unsigned long long edge3 = third < first ? (third << 32) + first : (first << 32) + third;
		
		// Get new vertices
		unsigned long long fourth = edges[edge1];
		unsigned long long fifth = edges[edge2];
		unsigned long long sixth = edges[edge3];
		
		// Assign four new faces
		newSphere.faces.push_back(glm::uvec3(first, fourth, sixth));
		newSphere.faces.push_back(glm::uvec3(fourth, second, fifth));
		newSphere.faces.push_back(glm::uvec3(sixth, fifth, third));
		newSphere.faces.push_back(glm::uvec3(fourth, fifth, sixth));
		
		// Assign new edges
		unsigned long long edge4 = fourth < fifth ? (fourth << 32) + fifth : (fifth << 32) + fourth;
		unsigned long long edge5 = fifth < sixth ? (fifth << 32) + sixth : (sixth << 32) + fifth;
		unsigned long long edge6 = sixth < fourth ? (sixth << 32) + fourth : (fourth << 32) + sixth;
		newSphere.edges[edge4] = 0;
		newSphere.edges[edge5] = 0;
		newSphere.edges[edge6] = 0;
	}

	return newSphere;
}
