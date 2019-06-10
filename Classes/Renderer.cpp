#include "Renderer.h"
#include<iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
bool GLLogCall(const char* function, const char* file, int line)
{
	if (GLenum error = glGetError())
	{
		std::cout << "GLError (" << error << ") in function: " << function << " at file: " << file << "at line: " << line << std::endl;
		return false;
	}
	return true;
}
