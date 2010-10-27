#ifdef __EXP_NMS_GL
#    define MD2LOADER_D __declspec(dllexport)
#else
#    define MD2LOADER_D __declspec(dllimport)
#endif

#ifndef __MD2LOADER_H__
#define __MD2LOADER_H__

#include <windows.h>
#include <iostream>
#include "SDL_opengl.h"
#include <stdio.h>
#include "NMS_AssetManager.h"
#include "NMS_FileManagement.h"
#include "NMS_LogFunctions.h"
#include "NMS_Mesh.h"

#define MAGIC_NO	844121161  //It means IDP2

#define MAX_VERTICES 2048

#define IDENT 8

#define FRAME_HEADER_SIZE	(sizeof(float)*6+16)

typedef float vec3_t[3];

typedef enum {
    STAND,
    RUN,
    ATTACK,
    PAIN_A,
    PAIN_B,
    PAIN_C,
    JUMP,
    FLIP,
    SALUTE,
    FALLBACK,
    WAVE,
    POINTAT,
    CROUCH_STAND,
    CROUCH_WALK,
    CROUCH_ATTACK,
    CROUCH_PAIN,
    CROUCH_DEATH, 
    DEATH_FALLBACK,
    DEATH_FALLFORWARD,
    DEATH_FALLBACKSLOW,
    BOOM,

    MAX_ANIMATIONS

} animType_t;


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
}Header,*pHeader;



//STRUCTURE OF A SINGLE VERTEX  vertex_t in the guide
typedef struct
{
	char	vertex[3];
	char	lightNormalIndex;
}TriangleVertex,*pTriangleVertex;

//FRAME DEFINITION  frame_t in the guide
typedef struct
{
	float				scale[3];       //Scale factor to uncompress vertexes
	float				translate[3];   //Translation factor to uncompress vertexes
	char				name[16];       //Name of the frame
	pTriangleVertex	pvertices;      //Pointer to the list of vertexes
}Frame,*pFrame;  

//TEXTURE COORDINATES FOR THE VERTEX   texcoord_t in the guide
typedef struct
{
	short	s,t;
}TextureCoord,*pTextureCoord;


typedef struct
{
	float	s,t;
	int		vertexIndex;
}GLCommand,*pGLCommand;

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

class MD2LOADER_D MD2Model : public NMS_Mesh
{
	public:
		static anim_t   animlist[21];       // animation list
		MD2Model();
		~MD2Model();

		int		LoadModel(const char* sFileName,const char* sTextName);
		int     LoadSkin(const char* fileName);
		void    SetAnim( int type );
		void    render(float time);
		void	DrawFrame(int frame,int nFrame); // base zero

		
		

	private:
		//Initialize the variables
		void	InitData();
		void	LoadData();
		void	LoadFrames();
		void	LoadGLCommands();


		void    Animate( float time );
		void    Interpolate( vec3_t* vertlist,vec3_t* lightList);
		void    RenderFrame();

		//MISC MEMORY AND FILE ROUTINES
		int		ReadFile(const char* fileName);						    //Read the model file
		int		ReadHeader(char *buffer,pHeader phead);			        //Load the header of the model as stored into the buffer
		
	private:
		//Check to avoid crashes: the model has loaded in the right way
		bool                bModelLoadedCorrectly;

		char*				fileBuffer;	 //The buffer containing the whole file that has been read
		Header				md2Header;	 //Our MD2 header, useful to get the offset of the components


		int					numFrames;
		int					numVertices;
		int					numGlCommands;

		vec3_t*     		p_modelVertices;			//Vertices of the current frame
		vec3_t*     		p_nextFrameVertices;		//Vertices of the next frame in the animation
		vec3_t*             p_lightnormals;				//Normals of the model in the current frame
		vec3_t*             p_nextLightNormals;			//Normals of the model in the next frame
		int*				p_openGlCommands;			//List of the openGL commands

		int					textureID;					// TextureID for the model
		const char*			sTextureName;
		animState_t         m_anim;						// Animation state
		float               scaleFactor;				// Scale value for the model
		pFrame				p_frameData;				//Pointer to the frames array

		static vec3_t       anorms[ 162 ];					//Vectors of precalculated normals for the model

};

#endif 