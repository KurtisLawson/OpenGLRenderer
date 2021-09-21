// This pre-processor prevents multiple shader definitions from being included if called from multiple files.
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // Include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// Shader program ID
	unsigned int ID;

	// Constructor: must read and build shaders.
	Shader() = default;
	Shader(const char* vertShaderPath, const char* fragShaderPath);

	// Use and activate the Shader Program
	void use();

	// Utility functions for setting uniforms externally
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
private:
	void linkShaderProgramID(const char* vertSource, const char* fragSource);

};
#endif



