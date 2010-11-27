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

	const char * vsc = readFile(vertexShaderFilename);
	const char * fsc = readFile(fragmentShaderFilename);

	glShaderSource(vertexShader, 1, &vsc, NULL);
	glShaderSource(fragmentShader, 1, &fsc, NULL);
	
	compileShader(vertexShader, vertexShaderFilename);
	compileShader(fragmentShader, fragmentShaderFilename);
	
	program = glCreateProgram();
		
	glAttachShader(program,vertexShader);
	glAttachShader(program,fragmentShader);
	
	linkProgram(program);
	glUseProgram(program);
}

void NMS_ShaderManager::enableTextures()
{
	NMS_SHADER_MANAGER->setShaderAttribute("textureEnabled", 1);
}

void NMS_ShaderManager::disableTextures()
{
	NMS_SHADER_MANAGER->setShaderAttribute("textureEnabled", 0);
}

void NMS_ShaderManager::setShaderAttribute(char * attribute, int value)
{
	GLint loc;
	loc = glGetUniformLocation(program,attribute);
	glUniform1i(loc,value);
}

void NMS_ShaderManager::compileShader(GLuint shader, char * file)
{
	int success;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(success != GL_TRUE)
		cerr << "Error compiling shader: " << file << endl;
	printShaderInfoLog(shader);
}

void NMS_ShaderManager::linkProgram(GLuint program)
{
	int success;
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(success != GL_TRUE) {
		cerr << "Error linking program" << endl;
		printProgramInfoLog(program);
	}
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
		cerr << infoLog << endl;
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
		cerr << infoLog << endl;
	    free(infoLog);
	}
}

char * NMS_ShaderManager::readFile(char * filename)
{
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (filename != NULL) {
		fp = fopen(filename,"rt");

		if (fp != NULL) {
      
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}