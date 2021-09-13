#include "RenderableObject.h"

// Member functions definitions including constructor
RenderableObject::RenderableObject(float verts[], unsigned int indices[], unsigned int indexCount, const char* vertShaderSource, const char* fragShaderSource) {
	cout << "RenderableObject is being created" << endl;
	float squareVerts[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

	unsigned int squareIndices[] = { // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// ..:: Initialization code (done once (unless your object frequently changes)) ::..
	unsigned int VBO, EBO, VAO;
	glGenVertexArrays(1, &VAO);

	// 1. Create an ID for a new VBO & EBO to send to the Vertex Shader for rendering, stored in the GPU.
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// VBO's must be bound to a unique buffer object type, in this case GL_ARRAY_BUFFER.
	//		Any buffer calls made on GL_ARRAY_BUFFER will refer to and configure our VBO until it is re-bound.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy the previously defined vertex data into the bound buffer's memory.
	//		The graphics card will manage the data as follows:
	//			GL_STREAM_DRAW: the data is set only once, and used by the GPU at most a few times.
	//			GL_STATIC_DRAW: the data is set only once, and used many times.
	//			GL_DYNAMIC_DRAW : the data is changed a lot, and used many times.
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVerts), squareVerts, GL_STATIC_DRAW);

	// Next, we bind our index array in the same way as our VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

	// 2. OpenGL does not yet know how it should interpret the vertex data in memory.
	//		Now we define how it should connect the vertex data to the vertex shader's attributes
	// ---
	//		The first parameter specifies which vertex attribute we want to configure. Remember that we specified the location of the position vertex attribute in the vertex shader with layout (location = 0). This sets the location of the vertex attribute to 0 and since we want to pass data to this vertex attribute, we pass in 0.
	//		The next argument specifies the size of the vertex attribute.The vertex attribute is a vec3 so it is composed of 3 values.
	//		The third argument specifies the type of the data which is GL_FLOAT(a vec * in GLSL consists of floating point values).
	//		The next argument specifies if we want the data to be normalized.If we're inputting integer data types (int, byte) and we've set this to GL_TRUE, the integer data is normalized to 0 (or -1 for signed data) and 1 when converted to float.This is not relevant for us so we'll leave this at GL_FALSE.
	//		The fifth argument is known as the stride and tells us the space between consecutive vertex attributes.Since the next set of position data is located exactly 3 times the size of a float away we specify that value as the stride.Note that since we know that the array is tightly packed(there is no space between the next vertex attribute value) we could've also specified the stride as 0 to let OpenGL determine the stride (this only works when values are tightly packed). Whenever we have more vertex attributes we have to carefully define the spacing between each vertex attribute.
	//		The last parameter is of type void*and thus requires that weird cast.This is the offset of where the position data begins in the buffer.Since the position data is at the start of the data array this value is just 0. We will explore this parameter in more detail later on
	// ---
	int positionAttributeID = 0;
	int vec3Size = 3;
	glVertexAttribPointer(positionAttributeID, vec3Size, GL_FLOAT, GL_FALSE, vec3Size * sizeof(float), (void*)positionAttributeID);
	glEnableVertexAttribArray(positionAttributeID);

	vao = VAO;
	vbo = VBO;
	ebo = EBO;
	shader_program = linkShaderProgram(vertShaderSource, fragShaderSource);
	numIndices = indexCount;
}

// Compile vertex and fragment shaders and returned a linked program ID
unsigned int RenderableObject::linkShaderProgram(const char* vertSource, const char* fragSource) {
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
		return -1;
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
		return -1;
	}

	// Finally we create a shader object to link our shader objects together.
	//		We need to make sure the outputs of each shader is consistent
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); // Creates an ID for an empty program object we can attach shaders to.

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog); // When checking success, we use GetProgramInfoLog
		std::cout << "ERROR::SHADER::OBJECT::LINKING::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}

	// Call the linked shader program after compilation.
	//		Each call of 'useProgram' will use the currently linked shaderProgram.
	//		Once compiled and linked, we don't have need of the shader objects anymore

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void RenderableObject::Draw() {
	// ..:: Drawing code (in render loop) :: ..
	//		This is done FOR EACH object we want to draw this frame.
	// 1. Choose the shader to use
	glUseProgram(shader_program);
	// 2. Bind the VAO of the object we want to draw.
	glBindVertexArray(vao);
	// 3. Draw the object.
	//		Use DrawArrays for ordered, and DrawElements for indexed.
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	// 4. Unbind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}