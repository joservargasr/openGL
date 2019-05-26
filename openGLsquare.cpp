#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct ShaderProgramSource 
{
	string vertexSource, fragmentSource;
};
static ShaderProgramSource ParseShader(const string& sFilePath)
{
	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderType stType = ShaderType::NONE;
	ifstream ifsStream(sFilePath);
	string sLine;
	stringstream ss[2];
	while (getline(ifsStream, sLine))
	{
		if (sLine.find("#shader") != string::npos)
		{
			if (sLine.find("vertex") != string::npos)
				stType = ShaderType::VERTEX;
			else if(sLine.find("fragment")!= string::npos)
				stType = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)stType] << sLine<<"\n";
		}
	}
	return { ss[(int)ShaderType::VERTEX].str(),ss[(int)ShaderType::FRAGMENT].str() };
}
static unsigned int CompileShader(const string& source,unsigned int type) 
{
	
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message=(char*)alloca(length*sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		cout << "Failed to compile: " <<
			(type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")<< " shader"<<endl;
		cout << message << endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}
static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(vertexShader,GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);
	glAttachShader(program,vs);
	glAttachShader(program,fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glewInit();
	cout << glGetString(GL_VERSION) << endl;

	float fPositions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
	};

	unsigned int nIndices[]{
		0,1,2,
		2,3,0
	};
	unsigned int nBuffer;
	glGenBuffers(1, &nBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, nBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fPositions),fPositions,GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT,GL_FALSE, sizeof(float) * 2, 0);

	unsigned int nBufferObject;
	glGenBuffers(1, &nBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(nIndices), nIndices, GL_STATIC_DRAW);
	
	ShaderProgramSource source= ParseShader("res/shaders/Basic.shader");
	unsigned int shader=CreateShader(source.vertexSource,source.fragmentSource);
	glUseProgram(shader);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT,nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	//glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}