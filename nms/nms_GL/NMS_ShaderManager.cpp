#include "NMS_ShaderManager.h"
#include "NMS_StaticAllocator.h"

using namespace std;

NMS_ShaderManager * NMS_ShaderManager::singleton;

NMS_ShaderManager * NMS_ShaderManager::getInstance()
{
	if(!singleton) {
		singleton = new(STATIC_ALLOC, MEM_PERSISTENT) NMS_ShaderManager;
	}
	return singleton;
}

void NMS_ShaderManager::loadShaders(char * vertexShaderFilename, char * fragmentShaderFilename)
{
	const char *vs,*fs;
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);	
	
	vs = readFile(vertexShaderFilename);
	fs = readFile(fragmentShaderFilename);
	
	glShaderSource(vertexShader, 1, &vs, NULL);
	glShaderSource(fragmentShader, 1, &fs, NULL);
	
	compileShader(vertexShader);
	compileShader(fragmentShader);
	
	program = glCreateProgram();
		
	glAttachShader(program,vertexShader);
	glAttachShader(program,fragmentShader);
	
	glLinkProgram(program);
	glUseProgram(program);
}

void NMS_ShaderManager::compileShader(GLuint shader)
{
	int success;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(success != GL_TRUE)
		printShaderInfoLog(shader);
}

void NMS_ShaderManager::printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
	    infoLog = (char *)malloc(infologLength);
	    glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
	    free(infoLog);
	}
}

void NMS_ShaderManager::printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
	    infoLog = (char *)malloc(infologLength);
	    glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
	    free(infoLog);
	}
}

const char * NMS_ShaderManager::readFile(char * filename)
{
	string line;
	string contents = "";
	fstream file(filename);
	while(file.good()) {
		getline(file, line);
		contents += line;
	}
	if(!file.eof())
		throw "File read error ";

	return (contents.c_str());
}