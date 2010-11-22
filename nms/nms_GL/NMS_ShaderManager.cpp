#include "NMS_ShaderManager.h"
#include "NMS_StaticAllocator.h"
#include "NMS_LogFunctions.h"

using namespace std;

NMS_ShaderManager * NMS_ShaderManager::singleton;

NMS_ShaderManager * NMS_ShaderManager::getInstance()
{
	if(!singleton) {
		singleton = new(STATIC_ALLOC, MEM_PERSISTENT) NMS_ShaderManager;
	}
	return singleton;
}

void NMS_ShaderManager::up()
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
		/*LOG.write("Error: \n", LOG_ERROR);
		LOG.write((char *) glewGetErrorString(err), LOG_ERROR);*/
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	}
}

void NMS_ShaderManager::loadShaders(char * vertexShaderFilename, char * fragmentShaderFilename)
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);	
	
	char vs[10000];
	char fs[10000];

	readFile(vertexShaderFilename, vs);
	readFile(fragmentShaderFilename, fs);

	const char * vsc = vs;
	const char * fsc = fs;

	glShaderSource(vertexShader, 1, &vsc, NULL);
	glShaderSource(fragmentShader, 1, &fsc, NULL);
	
	compileShader(vertexShader);
	compileShader(fragmentShader);
	
	program = glCreateProgram();
		
	glAttachShader(program,vertexShader);
	glAttachShader(program,fragmentShader);
	
	linkProgram(program);
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

void NMS_ShaderManager::linkProgram(GLuint program)
{
	int success;
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(success != GL_TRUE)
		printProgramInfoLog(program);
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

void NMS_ShaderManager::readFile(char * filename, char * dest)
{
	string line;
	string contents = "";
	fstream file(filename);
	if(file.bad()) {
			throw "Error opening file";
	}
	while(file.good()) {
		getline(file, line);
		contents += line;
	}
	const char * ct = contents.c_str();
	memcpy(dest, ct, contents.size());
	dest[contents.size()] = '\0';
	//contents.copy(dest, contents.size());
}