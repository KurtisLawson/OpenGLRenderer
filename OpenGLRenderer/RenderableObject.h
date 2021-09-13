#pragma once

// OpenGL Includes
#include <glad/glad.h> // Always include glad first to get the OpenGL headers required by glfw
#include <GLFW/glfw3.h>

// Standard Library Includes
#include <iostream>

using namespace std;

class RenderableObject {

	private:
		unsigned int vao, vbo, ebo;
		unsigned int shader_program;
		
		unsigned int numIndices;

	public:
		// Constructor
		RenderableObject(float verts[], unsigned int indices[], unsigned int indexCount, const char* vertShaderSource, const char* fragShaderSource);

		// Functions
		void Draw();
		unsigned int linkShaderProgram(const char* vertSource, const char* fragSource);

};