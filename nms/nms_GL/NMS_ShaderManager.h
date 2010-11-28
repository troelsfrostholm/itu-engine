#ifndef __NMS_SHADER_MANAGER_H
#define __NMS_SHADER_MANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#if (!defined(__gl_h_)) && (!defined(__GL_H__)) && (!defined(__X_GL_H))
#include <GL/glew.h>
#endif
#include "SDL.h"
#include "SDL_opengl.h"

using namespace std;

#define NMS_SHADER_MANAGER NMS_ShaderManager::getInstance()

class NMS_ShaderManager
{
public:
	static NMS_ShaderManager * getInstance();
	void up();
	void loadShaders(string vertexShader, string fragmentShader);
	void setShaderAttribute(char * attribute, int value);
	void enableTextures();
	void disableTextures();

private:
	static NMS_ShaderManager * singleton;
	GLuint vertexShader, fragmentShader, program;
	std::map<string, GLint> programs;

	void compileShader(GLuint shader, string file);
	void linkProgram(GLuint program);
	void printShaderInfoLog(GLuint obj);
	void printProgramInfoLog(GLuint obj);
	static char * readFile(string filename);
};

#endif