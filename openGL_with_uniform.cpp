#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x,__FILE__,__LINE__))

using namespace std;

struct ShaderProgramSource 
{
	string vertexSource, fragmentSource;
};

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* function,const char* file,int line)
{
	if (GLenum error = glGetError())
	{
		cout << "GLError (" << error << ") in function: "<<function<<" at file: "<<file<<"at line: "<<line << endl;
		return false;
	}
	return true;
}
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
	
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message=(char*)alloca(length*sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		cout << "Failed to compile: " <<
			(type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")<< " shader"<<endl;
		cout << message << endl;
		GLCall(glDeleteShader(id));
		return 0;
	}
	return id;
}
static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	GLCall(unsigned int vs = CompileShader(vertexShader,GL_VERTEX_SHADER));
	GLCall(unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER));
	GLCall(glAttachShader(program,vs));
	GLCall(glAttachShader(program,fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

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

	//Change the frame rate.
	glfwSwapInterval(1);

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
	GLCall(glGenBuffers(1, &nBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, nBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(fPositions),fPositions,GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT,GL_FALSE, sizeof(float) * 2, 0));

	unsigned int nBufferObject;
	GLCall(glGenBuffers(1, &nBufferObject));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nBufferObject));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(nIndices), nIndices, GL_STATIC_DRAW));
	
	ShaderProgramSource source= ParseShader("res/shaders/Basic.shader");
	unsigned int shader=CreateShader(source.vertexSource,source.fragmentSource);
	GLCall(glUseProgram(shader));

	int iUniformLocation = glGetUniformLocation(shader,"uColor");
	ASSERT(iUniformLocation != -1);
	
	float g = 0.0f;
	float increment = 0.05;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall((GL_COLOR_BUFFER_BIT));

		glUniform4f(iUniformLocation, 0.2f, g, 0.4f, 1.0f);
		GLCall(glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT,nullptr));

		increment= (g > 1.0f) ? -0.05f : (g < 0.0f) ? 0.05f: increment;
		g += increment;

		/* Swap front and back buffers */
		GLCall(glfwSwapBuffers(window));

		/* Poll for and process events */
		GLCall(glfwPollEvents());
	}
	//glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}