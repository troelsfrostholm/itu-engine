#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include "SDL.h"
#include "SDL_opengl.h" 

#define NMS_SHADER_MANAGER NMS_ShaderManager::getInstance()
//#define MAX_SHADER_FILE_LENGTH 1000;

class NMS_ShaderManager
{
public:
	void loadShaders(char * vertexShader, char * fragmentShader);
	static NMS_ShaderManager * getInstance();
	void up();

private:
	static NMS_ShaderManager * singleton;
	GLuint vertexShader, fragmentShader, program;

	void compileShader(GLuint shader);
	void linkProgram(GLuint program);
	void printShaderInfoLog(GLuint obj);
	void printProgramInfoLog(GLuint obj);
	static void readFile(char * filename, char * dest);
};