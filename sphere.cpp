#include <map>

#include "sphere.h"

void Sphere::InitIcosahedron()
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
}

void Sphere::InitOctahedron()
{
	const glm::vec3 l_vertices[] = {
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1),
		glm::vec3(-1, 0, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, 0, -1)
	};
	vertices.assign(l_vertices, l_vertices + sizeof(l_vertices) / sizeof(glm::vec3));

	const glm::uvec3 l_faces[] = {
		glm::uvec3(0, 1, 2),
		glm::uvec3(5, 1, 0),
		glm::uvec3(3, 1, 5),
		glm::uvec3(2, 1, 3),
		glm::uvec3(0, 2, 4),
		glm::uvec3(5, 0, 4),
		glm::uvec3(3, 5, 4),
		glm::uvec3(2, 3, 4),
	};
	faces.assign(l_faces, l_faces + sizeof(l_faces) / sizeof(glm::uvec3));
}

Sphere Sphere::Quadruple()
{
	Sphere newSphere;
	newSphere.vertices = vertices;
	
	std::map<unsigned long long, unsigned int> edges;
	
	for (auto iter = faces.begin(); iter != faces.end(); iter++) {
		// Get vertices from face
		unsigned long long first = (*iter)[0];
		unsigned long long second = (*iter)[1];
		unsigned long long third = (*iter)[2];
		
		// Get edges from vertices
		unsigned long long edge1 = first < second ? (first << 32) + second : (second << 32) + first;
		unsigned long long edge2 = second < third ? (second << 32) + third : (third << 32) + second;
		unsigned long long edge3 = third < first ? (third << 32) + first : (first << 32) + third;

		// Add new vertices to new sphere
		unsigned long long fourth, fifth, sixth;
		glm::vec3 newVertex;
		std::map<unsigned long long, unsigned int>::iterator edge_iter;
		if ((edge_iter = edges.find(edge1)) == edges.end()) {
			fourth = newSphere.vertices.size();
			newVertex = glm::normalize((vertices[first] + vertices[second]) / 2.0f);
			newSphere.vertices.push_back(newVertex);
			edges[edge1] = fourth;
		}
		else
			fourth = edge_iter->second;
		
		if ((edge_iter = edges.find(edge2)) == edges.end()) {
			fifth = newSphere.vertices.size();
			newVertex = glm::normalize((vertices[second] + vertices[third]) / 2.0f);
			newSphere.vertices.push_back(newVertex);
			edges[edge2] = fifth;
		}
		else
			fifth = edge_iter->second;
		
		if ((edge_iter = edges.find(edge3)) == edges.end()) {
			sixth = newSphere.vertices.size();
			newVertex = glm::normalize((vertices[third] + vertices[first]) / 2.0f);
			newSphere.vertices.push_back(newVertex);
			edges[edge3] = sixth;
		}
		else
			sixth = edge_iter->second;
		
		// Assign four new faces
		newSphere.faces.push_back(glm::uvec3(first, fourth, sixth));
		newSphere.faces.push_back(glm::uvec3(fourth, second, fifth));
		newSphere.faces.push_back(glm::uvec3(sixth, fifth, third));
		newSphere.faces.push_back(glm::uvec3(fourth, fifth, sixth));
	}
	
	return newSphere;
}
