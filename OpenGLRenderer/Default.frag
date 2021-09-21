#version 330 core
out vec4 FragColor;

in vec3 vertexColor; // the input variable from the vertex shader (same name and type)
in vec2 TexCoord;

uniform vec3 positionOffset; // The global "uniform" variable, possibly updated per frame through an external loop?

uniform sampler2D texture1; // This uniform allows us to assign our texture as output for our Fragment shader
uniform sampler2D texture2;

void main()
{
	FragColor = texture(texture1, TexCoord) * vec4(vertexColor, 1.0f);
}