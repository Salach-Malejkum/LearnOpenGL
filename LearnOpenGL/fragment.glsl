#version 330 core
out vec4 FragColor;

in  vec3 ourColor;

uniform vec4 uColor;
uniform bool uUseColor;




void main()
{
	if (uUseColor)
		FragColor = uColor;
	else
		FragColor = vec4(ourColor, 1.0);
}