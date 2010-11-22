#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>

#define NMS_SHADER_MANAGER NMS_ShaderManager::getInstance()

class NMS_ShaderManager
{
public:
	void loadShaders(char * vertexShader, char * fragmentShader);
	static NMS_ShaderManager * getInstance();

private:
	static NMS_ShaderManager * singleton;
	GLuint vertexShader, fragmentShader, program;

	void compileShader(GLuint shader);
	void printShaderInfoLog(GLuint obj);
	void printProgramInfoLog(GLuint obj);
	static const char * readFile(char * filename);
};