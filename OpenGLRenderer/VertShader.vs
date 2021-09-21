#version 330 core
layout (location = 0) in vec3 aPos;		// The "position" attribute has a position 0
layout (location = 1) in vec3 aColor;	// The "color" attribute has a position 1

out vec3 vertexColor; // specify a color output to the fragment shader

void main() {
	gl_Position = vec4(aPos, 1.0);
	vertexColor = aColor;
}