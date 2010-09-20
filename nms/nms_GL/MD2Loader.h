#ifndef __MD2LOADER_H__
#define __MD2LOADER_H__

#include <iostream>
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library 
#include <stdio.h>
#include "TextureManager.h"

#define MAGIC_NO	844121161  //It means IDP2

#define MAX_VERTICES 2048

#define IDENT 8

#define MAX_NO_SKIN		5
#define MAX_SKIN_NAME	64

#define FRAME_HEADER_SIZE	(sizeof(float)*6+16)

typedef unsigned char	byte;
typedef short*	pshort;


//HEADER STRUCTURE
typedef struct
{
	int		magicNumber;      //The magic number. It should be equal to IDP2 to be a MD2 file
	int		version;	//Version of the header. It should be equal to 8
	int		skinWidth;  //Width of the texture. The textures are in another file, we will use the texture size instead
	int		skinHeight; //Height of the texture
	int		frameSize;  //The size in byte of each frame, it's the model and vertex of the model in a particular position

	int		numSkins;	//The number of textures that are available for this model, for reskinning for example red team or blue team
	int		numVertices;  //The sum of the number of vertexes in each frame or the sum of the vertexes in the model
	int		numTexCoords; //Number of texture coordinates that are stored with respect to ofs_st
	int		numTriangles;  //Total amount of triangles for this model
	int		numGlCommands;  //Number of openGL commands, an array of integer that allow us to render with just using triangles fans and strips
	int		numFrames;	   //Number of keyframes for animations, the other frames will be calculated with interpolation

	int		offsetSkins;	//Offset of the skins data in the header
	int		offsetTexCoord;
	int		offsetTriangles;
	int		offsetFrames;
	int		offsetGlCommands;
	int		offsetEnd;
}md2Header,*pmd2Header;


typedef float vec3_t[3];

//STRUCTURE OF A SINGLE VERTEX  vertex_t in the guide
typedef struct
{
	byte	vertex[3];
	byte	lightNormalIndex;
}md2TriangleVertex,*pmd2TriangleVertex;

//FRAME DEFINITION  frame_t in the guide
typedef struct
{
	float				scale[3];       //Scale factor to uncompress vertexes
	float				translate[3];   //Translation factor to uncompress vertexes
	char				name[16];       //Name of the frame
	pmd2TriangleVertex	pvertices;      //Pointer to the list of vertexes
}md2Frame,*pmd2Frame;  

//TEXTURE COORDINATES FOR THE VERTEX   texcoord_t in the guide
typedef struct
{
	short	s,t;
}md2TextureCoord,*pmd2TextureCoord;


typedef struct
{
	float	s,t;
	int		vertexIndex;
}md2GLCommand,*pmd2GLCommand;

// animation
typedef struct
{
    int     first_frame;            // first frame of the animation
    int     last_frame;             // number of frames
    int     fps;                    // number of frames per second

} anim_t;

// animation state
typedef struct
{
    int     startframe;              // first frame
    int     endframe;                // last frame
    int     fps;                     // frame per second for this animation

    float   curr_time;               // current time
    float   old_time;                // old time
    float   interpol;                // percent of interpolation

    int     type;                    // animation type

    int     curr_frame;              // current frame
    int     next_frame;              // next frame

} animState_t;

class MD2Loader
{
	public:
		MD2Loader();
		~MD2Loader();

		int		LoadModel(const char* fileName);
		int     LoadSkin(const char* fileName);
		int		GetNumFrames();

		void	DrawModel();
		void	DrawFrame(int frame); // base zero
		

	private:
		void*	md2Malloc(size_t size);                                    //Allocate the memory space with regard to the type to be used. Return a pointer to the memory allocation
		void	md2Free(void** p);                                         //Free the memory space pointed by the given pointer
		int		md2ReadFile(const char* fileName);						   //Read the model file
		int		md2ReadHeader(byte *buffer,pmd2Header phead);			   //Load the header of the model as stored into the buffer
		long	md2FileSize(FILE *fp);									   //Return the size of the file to read the entire file and put it into the buffer
		void	md2DumpHeader(const pmd2Header phead);

		void	md2InitData();
		void	md2LoadData();
		void	md2LoadFrames();
		void	md2LoadGLCommands();
		void	md2LoadTextures(const char* md2FileName);
		void	md2ProcessData();

		void    Animate( float time );
		void    Interpolate( vec3_t* vertlist );
		void    RenderFrame( void );

		
	private:
		byte*				m_buffer;	 //The buffer containing the whole file that has been read
		md2Header			m_header;	 //Our MD2 header, useful to get the offset of the components
		
		int					numFrames;
		int					numVertices;
		int					numGlCommands;

		vec3_t*     		m_vertices;
		int*                m_lightnormals;    // normal index array
		int*				m_glcmds;

		unsigned int        m_texid;            // texture id
		animState_t         m_anim;             // animation
		float               m_scale;            // scale value




		//pmd2TextureCoord	m_texData;	 //Pointer to the textures coordinate array
		pmd2TriangleVertex	m_vertData;  //Vertex array
		pmd2Frame			m_frameData; //Pointer to the frames array

		
		pshort				m_glIndicesData;

		
};

#endif 