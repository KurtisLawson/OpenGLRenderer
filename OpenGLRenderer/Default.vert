// This link is a handy plugin for syntax highlighting for GLSL code in Visual Studio.
// https://marketplace.visualstudio.com/items?itemName=DanielScherzer.GLSL
// ----------------------------------------------------------------------------------

#version 330 core
layout (location = 0) in vec3 aPos;			// The "position" attribute has a position index 0
layout (location = 1) in vec3 aColor;		// The "color" attribute has a position index of 1
layout (location = 2) in vec2 aTexCoord;	// The "texture" attribute has a position index of 2

out vec3 vertexColor; // specify a color output to the fragment shader
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	vertexColor = aColor;
	TexCoord = aTexCoord;
}