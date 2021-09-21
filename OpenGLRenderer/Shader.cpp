#include "Shader.h"

// ---
// Function Definitions
// ---
Shader::Shader(const char* vertShaderPath, const char* fragShaderPath)
{
	// 1. Retrieve the source code from given file path
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// Poll ifstream to ensure they can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// a. Open files
		vShaderFile.open(vertShaderPath);
		fShaderFile.open(fragShaderPath);
		std::stringstream vShaderStream, fShaderStream;

		// b. Read file's buffer contents into streams.
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// c. Close file handlers
		vShaderFile.close();
		fShaderFile.close();

		// d. Convert stream into a string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. Compile the shaders
	linkShaderProgramID(vShaderCode, fShaderCode);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

// Compile vertex and fragment shaders and returned a linked program ID
void Shader::linkShaderProgramID(const char* vertSource, const char* fragSource) {
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Attach the shader source code to the object and compile the shader.
	//		The second argument is how many strings are being passed as source code, in this case 1.
	glShaderSource(vertexShader, 1, &vertSource, NULL);
	glCompileShader(vertexShader);

	// Since we're compiling at runtime, it's beneficial to double check that compilation was a success.
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // We can check the compile status to our shader object.

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return;
	}

	// Fragment shader compilation.
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return;
	}

	// Finally we create a shader object to link our shader objects together.
	//		We need to make sure the outputs of each shader is consistent
	ID = glCreateProgram(); // Creates an ID for an empty program object we can attach shaders to.

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog); // When checking success, we use GetProgramInfoLog
		std::cout << "ERROR::SHADER::OBJECT::LINKING::COMPILATION_FAILED\n" << infoLog << std::endl;
		return;
	}

	// Call the linked shader program after compilation.
	//		Each call of 'useProgram' will use the currently linked shaderProgram.
	//		Once compiled and linked, we don't have need of the shader objects anymore

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}