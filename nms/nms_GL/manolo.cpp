#include "SDL.h" 
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library 
#include <IL/il.h>
#include <GL/gl.h>
 
int main(int argc, char *argv[])
{
  SDL_Event event;
  ILuint texid; /* ILuint is a 32bit unsigned integer.
    Variable texid will be used to store image name. */
  ILboolean success; /* ILboolean is type similar to GLboolean and can equal GL_FALSE (0) or GL_TRUE (1)
    it can have different value (because it's just typedef of unsigned char), but this sould be
    avoided.
    Variable success will be used to determine if some function returned success or failure. */
  GLuint image;
  int finished;
 
  SDL_Init(SDL_INIT_VIDEO);
  SDL_SetVideoMode(640, 480, 32, SDL_OPENGL | SDL_HWSURFACE);
 
  glViewport(0, 0, 640, 480);
  glEnable(GL_TEXTURE_2D);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 640.0, 480.0, 0.0, 0.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
/* Before calling ilInit() version should be checked. */
  if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
  {
    /* wrong DevIL version */
    SDL_Quit();
    return -1;
  }
 
  ilInit(); /* Initialization of DevIL */
  ilGenImages(1, &texid); /* Generation of one image name */
  ilBindImage(texid); /* Binding of image name */
  success = ilLoadImage("hobgoblin.png"); /* Loading of image "image.jpg" */
  if (success) /* If no error occured: */
  {
    success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); /* Convert every colour component into
      unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
    if (!success)
    {
      /* Error occured */
      SDL_Quit();
      return -1;
    }
    glGenTextures(1, &image); /* Texture name generation */
    glBindTexture(GL_TEXTURE_2D, image); /* Binding of texture name */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
      interpolation for magnification filter */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear
      interpolation for minifying filter */
    glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
      ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
      ilGetData()); /* Texture specification */
  }
  else
  {
    /* Error occured */
    SDL_Quit();
    return -1;
  }
  ilDeleteImages(1, &texid); /* Because we have already copied image data into texture data
    we can release memory used by image. */
 
  /* Main loop */
  finished = 0;
  while (!finished)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE)
            finished = 1;
          break;
        case SDL_QUIT:
          finished = 1;
          break;
      }
    }
    /* Draw a quad */
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex2i(0,   0);
    glTexCoord2i(0, 1); glVertex2i(0,   480);
    glTexCoord2i(1, 1); glVertex2i(640, 480);
    glTexCoord2i(1, 0); glVertex2i(640, 0);
    glEnd();
    SDL_GL_SwapBuffers();
  }
 
  glDeleteTextures(1, &image);
  SDL_Quit();
 
  return 0;
}