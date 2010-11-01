#include "MD2Model.h"


//Initialize the vector of the 21 possible animations for an MD2 model
anim_t MD2Model::animlist[ 21 ] = 
{
    // first, last, fps

    {   0,  39,  9 },   // STAND
    {  40,  45, 10 },   // RUN
    {  46,  53, 10 },   // ATTACK
    {  54,  57,  7 },   // PAIN_A
    {  58,  61,  7 },   // PAIN_B
    {  62,  65,  7 },   // PAIN_C
    {  66,  71,  7 },   // JUMP
    {  72,  83,  7 },   // FLIP
    {  84,  94,  7 },   // SALUTE
    {  95, 111, 10 },   // FALLBACK
    { 112, 122,  7 },   // WAVE
    { 123, 134,  6 },   // POINT
    { 135, 153, 10 },   // CROUCH_STAND
    { 154, 159,  7 },   // CROUCH_WALK
    { 160, 168, 10 },   // CROUCH_ATTACK
    { 196, 172,  7 },   // CROUCH_PAIN
    { 173, 177,  5 },   // CROUCH_DEATH
    { 178, 183,  7 },   // DEATH_FALLBACK
    { 184, 189,  7 },   // DEATH_FALLFORWARD
    { 190, 197,  7 },   // DEATH_FALLBACKSLOW
    { 198, 198,  5 },   // BOOM
};

// Load the precalculated normal vectors
vec3_t  MD2Model::anorms[ 162 ] = {
#include    "NMS_anorms.h"
};



MD2Model::MD2Model()
{
	bModelLoadedCorrectly   =false;
	fileBuffer	            =NULL;
	p_frameData	            =NULL;
	p_modelVertices         =NULL;
	p_nextFrameVertices     =NULL;
	p_lightnormals          =NULL;
	p_nextLightNormals      =NULL;
	p_openGlCommands        =NULL;
}

MD2Model::~MD2Model()
{
	nmsFileManagement::Free((void**)&fileBuffer);
	nmsFileManagement::Free((void**)&p_modelVertices);
	nmsFileManagement::Free((void**)&p_nextFrameVertices);
	for (int index=0;index<md2Header.numFrames;index++)
		nmsFileManagement::Free((void**)&p_frameData[index].pvertices);
	nmsFileManagement::Free((void**)&p_frameData);
	nmsFileManagement::Free((void**)&p_lightnormals);
	nmsFileManagement::Free((void**)&p_nextLightNormals);
	nmsFileManagement::Free((void**)&p_openGlCommands);
}



//Load the model file
int MD2Model::LoadModel(const char* sModelName,const char* sTextName)
{
	int result=0;

	//Load the file into the buffer
	result=ReadFile(sModelName);
	if (result!=0)
	{
		LOG.write("MD2Model::LoadModel -> The model has not been loaded correctly!\n",LOG_ERROR);
		return result;
	}

	//Read the header of the file stored into the buffer
	result=ReadHeader(fileBuffer,&md2Header);
	if (result!=0)
		return result;
	

	InitData();
	LoadData();

	m_anim.startframe   = 0;
    m_anim.endframe     = 0;
    m_anim.next_frame   = 0;
    m_anim.fps          = 1;
    m_anim.type         = -1;
	
	sTextureName=sTextName;
	DrawFrame(0,0); // Initially set it to Point to First Frame
	bModelLoadedCorrectly=true;
	return 0;
}




//Read the model file
int MD2Model::ReadFile(const char* fileName)
{
	FILE	*fp;

	//Open the model file
	fopen_s(&fp, fileName,"rb");
	if (!fp)
		return 1;

	//Free the memory to store the file into the buffer
	nmsFileManagement::Free((void**)&fileBuffer);

	//Calculate the size of the file
	long fileSize= nmsFileManagement::FileSize(fp);
	if (fileSize<=0)
		return 1;

	//Allocate the space needed to keep the file into the buffer
	fileBuffer=(BYTE*)nmsFileManagement::Malloc(fileSize);
	if (!fileBuffer)
		return 1;

	//Load the file into the buffer
	if (fread(fileBuffer,1,fileSize,fp)!=(size_t)fileSize)
		return 1;

	//Close the file
	fclose(fp);
	return 0;
}


//Load the header of the model as stored into the buffer
int MD2Model::ReadHeader(BYTE *buffer,pHeader phead)
{
	//Copy into the header the header as read from the buffer
	memcpy(phead,buffer,sizeof(*phead));

	//The model is not an MD2 file, return error
	if ((phead->magicNumber != MAGIC_NO)||(phead->version != IDENT))
		return 1;
	numFrames=phead->numFrames;
	numVertices=phead->numVertices;
	numGlCommands=phead->numGlCommands;
	return 0;
}


void MD2Model::InitData()
{
	p_lightnormals=0;
	p_nextLightNormals=0;
	p_openGlCommands=0;
	scaleFactor=1.0f;
	textureID=0;
	sTextureName=NULL;

	//Free the memory we are using
	nmsFileManagement::Free((void**)&p_modelVertices);
	nmsFileManagement::Free((void**)&p_nextFrameVertices);
	nmsFileManagement::Free((void**)&p_lightnormals);
	nmsFileManagement::Free((void**)&p_nextLightNormals);
	nmsFileManagement::Free((void**)&p_frameData);
	if (p_frameData!=NULL)
	{
		for (int index=0;index<md2Header.numFrames;index++)
			nmsFileManagement::Free((void**)&p_frameData[index].pvertices);
		nmsFileManagement::Free((void**)&p_frameData);
	}

	//Allocate the space for the vertices and the vertices used in the next frame,
	//the ligthing normals and the normals used in the next frame
	//and the opengl command list to be able to draw our model
	p_modelVertices		= (vec3_t*) nmsFileManagement::Malloc(md2Header.numVertices*sizeof(vec3_t));
	p_nextFrameVertices = (vec3_t*) nmsFileManagement::Malloc(md2Header.numVertices*sizeof(vec3_t));
	p_lightnormals      = (vec3_t*) nmsFileManagement::Malloc(sizeof(vec3_t)*md2Header.numVertices);
	p_nextLightNormals  = (vec3_t*) nmsFileManagement::Malloc(sizeof(vec3_t)*md2Header.numVertices);
	p_openGlCommands    =  new(LEVEL_ALLOC, MEM_LEVEL) int[ numGlCommands ];
}


void MD2Model::LoadData()
{
	LoadFrames();
	LoadGLCommands();
}

void MD2Model::LoadFrames()
{
	int index=0;

	//Point to the frame section in the file we have read
	BYTE* buf_t		= fileBuffer+ md2Header.offsetFrames;

	//The total space to allocate for the frame data
	long	frameHeaderSize	= md2Header.numFrames * sizeof(Frame);
	//The total number of vertexes per frame, return the space for it
	long	frameVertSize	= md2Header.numVertices * sizeof(TriangleVertex); 

	p_frameData =(pFrame)nmsFileManagement::Malloc(frameHeaderSize);
	if (!p_frameData)
		return;

	//Create the space for each vertex, for each frame
	for (index=0;index< md2Header.numFrames;index++)
	{
		p_frameData[index].pvertices = (pTriangleVertex)nmsFileManagement::Malloc(frameVertSize);
		if (!p_frameData[index].pvertices)
			return;
	}

	//Fill the frame data
	for (index=0;index< md2Header.numFrames;index++)
	{
		memcpy(&p_frameData[index],buf_t, FRAME_HEADER_SIZE);
		buf_t+= FRAME_HEADER_SIZE;
		memcpy(p_frameData[index].pvertices, buf_t, frameVertSize);
		buf_t+=frameVertSize;
	}
}


void MD2Model::LoadGLCommands()
{
	int index=0;

	//Point to the glCommands section in the file we have read
	BYTE*   buf_t		= fileBuffer+ md2Header.offsetGlCommands;
	//Copy from memory the command from the buffer to our pointer
	memcpy((BYTE *)p_openGlCommands,buf_t, numGlCommands * sizeof( int ));
}


void MD2Model::DrawFrame(int frame,int nFrame)
{
	if (frame<0 || frame> md2Header.numFrames)
		return;

	int index=0;
	if (p_modelVertices!=NULL)
	{
		for (index=0;index<md2Header.numVertices;index++)
		{
				// Every vertex of frame is multiplied by it's respective scale and then the translation is added.
				p_modelVertices[index][0] = (p_frameData[frame].pvertices[index].vertex[0] * p_frameData[frame].scale[0])+p_frameData[frame].translate[0];
				p_modelVertices[index][1] = (p_frameData[frame].pvertices[index].vertex[1] * p_frameData[frame].scale[1])+p_frameData[frame].translate[1];
				p_modelVertices[index][2] = (p_frameData[frame].pvertices[index].vertex[2] * p_frameData[frame].scale[2])+p_frameData[frame].translate[2];
				// Take the normal vectors for the model
				p_lightnormals[index][0]     =      anorms[p_frameData[frame].pvertices[index].lightNormalIndex][0];
				p_lightnormals[index][1]     =      anorms[p_frameData[frame].pvertices[index].lightNormalIndex][1];
				p_lightnormals[index][2]     =      anorms[p_frameData[frame].pvertices[index].lightNormalIndex][2];
				// Take the normal vectors for the model for the next frame
				p_nextLightNormals[index][0] =      anorms[p_frameData[nFrame].pvertices[index].lightNormalIndex][0];
				p_nextLightNormals[index][1] =      anorms[p_frameData[nFrame].pvertices[index].lightNormalIndex][1];
				p_nextLightNormals[index][2] =      anorms[p_frameData[nFrame].pvertices[index].lightNormalIndex][2];
				// Take vertices for the next frame
				p_nextFrameVertices[index][0] = (p_frameData[nFrame].pvertices[index].vertex[0] * p_frameData[nFrame].scale[0])+p_frameData[nFrame].translate[0];
				p_nextFrameVertices[index][1] = (p_frameData[nFrame].pvertices[index].vertex[1] * p_frameData[nFrame].scale[1])+p_frameData[nFrame].translate[1];
				p_nextFrameVertices[index][2] = (p_frameData[nFrame].pvertices[index].vertex[2] * p_frameData[nFrame].scale[2])+p_frameData[nFrame].translate[2];
		}
	}
}



//Draw the model. Be careful that the model should be loaded starting from frame 0
void MD2Model::render(float time)
{
	if(bModelLoadedCorrectly)
	{
		if(textureID==0)
		{
			LoadSkin(sTextureName);
		}
		 // animate. calculate current frame and next frame
		if( time > 0.0 )
			Animate( time );
		glPushMatrix();
			// rotate the model
			glRotatef( -90.0, 1.0, 0.0, 0.0 );
			glRotatef( 180.0f, 0.0, 0.0, 1.0 );
			RenderFrame();
		glPopMatrix();
	}
	else
	{
		LOG.write("MD2Model::DrawModel -> The model has not been loaded correctly cannot draw it!\n",LOG_ERROR);
	}
}


void MD2Model::RenderFrame( void )
{
    int              *ptricmds = p_openGlCommands;       // pointer on gl commands


	//Reverse the orientation of the polygon facing because
	//gl command list's triangles are clockwise winded
    glPushAttrib( GL_POLYGON_BIT );
    glFrontFace( GL_CW );

    //This is to enable backface culling
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );

    //Interpolate the vertixes and the light normals for our animations
	Interpolate( p_modelVertices,p_lightnormals );
    // Bind the model texture to our model
	glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, textureID );
	

    //Find if we are using fans or strips
    while( int i = *(ptricmds++) )
    {
        if( i < 0 )
        {
            glBegin( GL_TRIANGLE_FAN );
            i = -i;
        }
        else
        {
            glBegin( GL_TRIANGLE_STRIP );
        }
	
		//Draw the triangles now. The number of drawings are determined by the number
		//of openglCommands
        for(; i > 0; i--, ptricmds += 3 )
        {
            // ptricmds[0] : texture coordinate s
            // ptricmds[1] : texture coordinate t
            // ptricmds[2] : vertex index to render

            //Parse and apply texture coordinates
			glTexCoord2f( ((float *)ptricmds)[0], ((float *)ptricmds)[1]);
			//Parse and apply the light normals
			glNormal3fv( p_lightnormals[ ptricmds[2]] );
            //Draw the vertices
            glVertex3fv( p_modelVertices[ ptricmds[2] ] );
        }

        glEnd();
    }
    glDisable( GL_CULL_FACE );
	glDisable(GL_TEXTURE_2D);
    glPopAttrib();
}


void MD2Model::Interpolate( vec3_t *vertlist,vec3_t* lightList)
{
	vec3_t  *curr_v;    // pointer to current frame vertices
    vec3_t  *next_v;    // pointer to next frame vertices
	vec3_t  *curr_l;    // pointer to current frame vertices
    vec3_t  *next_l;    // pointer to next frame vertices

    // create current frame and next frame's vertex list
    // from the whole vertex list
	curr_v = p_modelVertices;
    next_v = p_nextFrameVertices;
	curr_l = p_lightnormals;
	next_l = p_nextLightNormals;

    // interpolate and scale vertices to avoid ugly animation
    for( int i = 0; i < numVertices ; i++ )
    {
        vertlist[i][0]  = (curr_v[i][0] + m_anim.interpol * (next_v[i][0] - curr_v[i][0])) * scaleFactor;
        vertlist[i][1]  = (curr_v[i][1] + m_anim.interpol * (next_v[i][1] - curr_v[i][1])) * scaleFactor;
        vertlist[i][2]  = (curr_v[i][2] + m_anim.interpol * (next_v[i][2] - curr_v[i][2])) * scaleFactor;
		lightList[i][0] = (curr_l[i][0] + m_anim.interpol * (next_l[i][0] - curr_l[i][0]));
		lightList[i][1] = (curr_l[i][1] + m_anim.interpol * (next_l[i][1] - curr_l[i][1]));
		lightList[i][2] = (curr_l[i][2] + m_anim.interpol * (next_l[i][2] - curr_l[i][2]));
    }
}

int MD2Model::LoadSkin(const char *filename)
{
	textureID=NMS_ASSETMANAGER.LoadTexture((char*)filename,(char*)filename);
	return textureID;
}


void MD2Model::SetAnim( int type )
{
	if(bModelLoadedCorrectly)
	{
		if( (type < 0) || (type > MAX_ANIMATIONS) )
			type = 0;

		m_anim.startframe   = animlist[ type ].first_frame;
		m_anim.endframe     = animlist[ type ].last_frame;
		m_anim.next_frame   = animlist[ type ].first_frame + 1;
		m_anim.fps          = animlist[ type ].fps;
		m_anim.type         = type;
	}
	else
	{
		LOG.write("MD2Model::SetAnim ->The model has not been loaded correctly cannot draw it!\n",LOG_ERROR);
	}
}

void MD2Model::Animate( float time )
{
    m_anim.curr_time = time;

    // calculate current and next frames
    if( m_anim.curr_time - m_anim.old_time > (1.0 / m_anim.fps) )
    {
        m_anim.curr_frame = m_anim.next_frame;
        m_anim.next_frame++;

        if( m_anim.next_frame > m_anim.endframe )
            m_anim.next_frame = m_anim.startframe;

        m_anim.old_time = m_anim.curr_time;
    }

    //We don't want the current/next frame to be greater than the maximum number of frames...
    if( m_anim.curr_frame > (numFrames - 1) )
        m_anim.curr_frame = 0;

    if( m_anim.next_frame > (numFrames - 1) )
        m_anim.next_frame = 0;

    m_anim.interpol = m_anim.fps * (m_anim.curr_time - m_anim.old_time);
	DrawFrame(m_anim.curr_frame,m_anim.next_frame);
}