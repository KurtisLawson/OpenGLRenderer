#pragma once

// OpenGL Includes
#include <glad/glad.h> // Always include glad first to get the OpenGL headers required by glfw
#include <GLFW/glfw3.h>

// GL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Local Library Includes
#include "Shader.h"
#include "stb_image.h"

// Standard Library Includes
#include <iostream>
#include <vector>

using namespace std;

class RenderableObject {

	private:
		unsigned int vao, vbo, ebo;
		unsigned int texture;
		Shader shader_program;
		glm::vec4 transformation_vector;

		vector<float>* vertices;
		vector<int>* indices;
		
		unsigned int numIndices;

	public:
		// Constructor
		RenderableObject(vector<float>& verts, vector<unsigned int>& inds, unsigned int indexCount, const char* vertPath, const char* fragPath, const char* texPath);

		// Functions
		void translate(glm::vec3 translation);
		void rotate(glm::vec3 rotation);
		void scale(glm::vec3 scale);
		void Draw();

};