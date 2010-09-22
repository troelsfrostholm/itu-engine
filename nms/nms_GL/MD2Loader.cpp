#include "MD2Loader.h"
#include "NMSFramework.h" 

MD2Loader::MD2Loader()
{
	m_buffer	=NULL;
	m_vertData	=NULL;
	m_frameData	=NULL;
	m_vertices  =NULL;
	m_glIndicesData=NULL;
}

MD2Loader::~MD2Loader()
{
	md2Free((void**)&m_buffer);
	md2Free((void**)&m_vertData);
	md2Free((void**)&m_vertices);
	md2Free((void**)&m_glIndicesData);
	for (int index=0;index<m_header.numFrames;index++)
		md2Free((void**)&m_frameData[index].pvertices);
	md2Free((void**)&m_frameData);
	md2Free((void**)&m_lightnormals);
	md2Free((void**)&m_glcmds);
}

//Load the model file
int MD2Loader::LoadModel(const char* fileName)
{
	int result=0;

	//Load the file into the buffer
	result=md2ReadFile(fileName);
	if (result!=0)
		return result;

	//Read the header of the file stored into the buffer
	result=md2ReadHeader(m_buffer,&m_header);
	if (result!=0)
		return result;
	

	md2InitData();
	md2LoadData();
	md2ProcessData();
	DrawFrame(0); // Initially set it to Point to First Frame

	return 0;
}


//Allocate the memory space with regard to the type to be used. Return a pointer to the memory allocation
void* MD2Loader::md2Malloc(size_t size)
{
	void* p=malloc(size);
	if (p==NULL)
		return p;
	memset(p,0,size);

	return p;
}

//Free the memory space pointed by the given pointer. Set to void** to receive every kind of pointers
void MD2Loader::md2Free(void** p)
{
	if (*p!=NULL)
	{
		free(*p);
		*p=NULL;
	}
}

//Read the model file
int MD2Loader::md2ReadFile(const char* fileName)
{
	FILE	*fp;

	//Open the model file
	fp=fopen(fileName,"rb");
	if (!fp)
		return 1;

	//Free the memory to store the file into the buffer
	md2Free((void**)&m_buffer);

	//Calculate the size of the file
	long fileSize= md2FileSize(fp);
	if (fileSize<=0)
		return 1;

	//Allocate the space needed to keep the file into the buffer
	m_buffer=(byte*)md2Malloc(fileSize);
	if (!m_buffer)
		return 1;

	//Load the file into the buffer
	if (fread(m_buffer,1,fileSize,fp)!=(size_t)fileSize)
		return 1;

	//Close the file
	fclose(fp);
	return 0;
}

long MD2Loader::md2FileSize(FILE *fp)
{
	long oldpos=ftell(fp);
	fseek(fp,0,SEEK_END);
	long curpos=ftell(fp);
	fseek(fp,oldpos,SEEK_SET);
	return curpos;
}



//Load the header of the model as stored into the buffer
int MD2Loader::md2ReadHeader(byte *buffer,pmd2Header phead)
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

/*
	Name : md2InitData
*/
void MD2Loader::md2InitData()
{
	m_lightnormals=0;
	m_glcmds=0;
	m_scale=1.0f;
	m_texid=0;
	md2Free((void**)&m_vertices);
	md2Free((void**)&m_vertData);
	md2Free((void**)&m_frameData);
	if (m_frameData!=NULL)
	{
		for (int index=0;index<m_header.numFrames;index++)
			md2Free((void**)&m_frameData[index].pvertices);
		md2Free((void**)&m_frameData);
	}
}











/*
	Name : md2LoadData
*/
void MD2Loader::md2LoadData()
{
	LoadSkin("hobgoblin.tga");
	md2LoadFrames();
	md2LoadGLCommands();
}

/*
	Name : md2LoadFrames
*/
void MD2Loader::md2LoadFrames()
{
	int index=0;

	//Point to the frame section in the file we have read
	byte	*buf_t		= m_buffer+ m_header.offsetFrames;

	//The total space to allocate for the frame data
	long	frameHeaderSize	= m_header.numFrames * sizeof(md2Frame);
	//The total number of vertexes per frame, return the space for it
	long	frameVertSize	= m_header.numVertices * sizeof(md2TriangleVertex); 

	m_frameData =(pmd2Frame)md2Malloc(frameHeaderSize);
	if (!m_frameData)
		return;

	//Create the space for each vertex, for each frame
	for (index=0;index< m_header.numFrames;index++)
	{
		m_frameData[index].pvertices = (pmd2TriangleVertex)md2Malloc(frameVertSize);
		if (!m_frameData[index].pvertices)
			return;
	}

	//Fill the frame data
	for (index=0;index< m_header.numFrames;index++)
	{
		memcpy(&m_frameData[index],buf_t, FRAME_HEADER_SIZE);
		buf_t+= FRAME_HEADER_SIZE;
		memcpy(m_frameData[index].pvertices, buf_t, frameVertSize);
		buf_t+=frameVertSize;
	}
}

/*
	Name : md2LoadGLCommands
*/
void MD2Loader::md2LoadGLCommands()
{
	int index=0;

	//Point to the glCommands section in the file we have read
	byte	*buf_t		= m_buffer+ m_header.offsetGlCommands;
	m_glcmds        = new int[ numGlCommands ];
	m_lightnormals = new int[numVertices*numFrames];
	memcpy((char *)m_glcmds,buf_t, numGlCommands * sizeof( int ));
}

/*
	Name : md2ProcessData
*/
void MD2Loader::md2ProcessData()
{
	md2Free((void**)&m_vertices);
	m_vertices = (vec3_t*) md2Malloc(m_header.numVertices*sizeof(vec3_t));
	if (!m_vertices)
	{
		printf("Error at md2ProcessData\n");
		return;
	}
}

void MD2Loader::DrawFrame(int frame)
{
	if (frame<0 || frame>= m_header.numFrames)
		return;

	int index=0;
	if (m_vertices!=NULL)
	{
		memset(m_vertices,0,sizeof(vec3_t)*m_header.numVertices);

		for (index=0;index<m_header.numVertices;index++)
		{
			// Every vertex of frame is multiplied by it's respective scale and then the translation is added.
			m_vertices[index][0] = (m_frameData[frame].pvertices[index].vertex[0] * m_frameData[frame].scale[0])+m_frameData[frame].translate[0];
			m_vertices[index][1] = (m_frameData[frame].pvertices[index].vertex[1] * m_frameData[frame].scale[1])+m_frameData[frame].translate[1];
			m_vertices[index][2] = (m_frameData[frame].pvertices[index].vertex[2] * m_frameData[frame].scale[2])+m_frameData[frame].translate[2];
			m_lightnormals[index]=m_frameData[frame].pvertices[index].lightNormalIndex;
		}
	}
}

/*
	Name : GetNumFrames
*/
int	MD2Loader::GetNumFrames() 
{ 
	return this->m_header.numFrames; 
}

/*
	Name : Draw()
	// frame : is base zero index
*/
void MD2Loader::DrawModel()
{
	//glPushMatrix();
        // rotate the model
        glRotatef( -90.0, 1.0, 0.0, 0.0 );
        glRotatef( 180.0f, 0.0, 0.0, 1.0 );
		RenderFrame();
	//glPopMatrix();
}


void MD2Loader::RenderFrame( void )
{
	static vec3_t    vertlist[ MAX_VERTICES ];  // interpolated vertices

    int              *ptricmds = m_glcmds;       // pointer on gl commands


    // reverse the orientation of front-facing
    // polygons because gl command list's triangles
    // have clockwise winding
    glPushAttrib( GL_POLYGON_BIT );
    glFrontFace( GL_CW );

    // enable backface culling
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );

    // interpolate
    //Interpolate( vertlist );

    // bind model's texture
    glBindTexture( GL_TEXTURE_2D, m_texid );


    // draw each triangle!
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


        for( /* nothing */; i > 0; i--, ptricmds += 3 )
        {
            // ptricmds[0] : texture coordinate s
            // ptricmds[1] : texture coordinate t
            // ptricmds[2] : vertex index to render

            //// parse texture coordinates
            glTexCoord2f( ((float *)ptricmds)[0], ((float *)ptricmds)[1] );

            // draw the vertex
            glVertex3fv( m_vertices[ ptricmds[2] ] );
        }

        glEnd();
    }

    glDisable( GL_CULL_FACE );
    glPopAttrib();
}


void MD2Loader::Interpolate( vec3_t *vertlist )
{
    for( int i = 0; i < numVertices ; i++ )
    {
		vertlist[i][0] = m_vertices[ i + (numVertices * m_anim.curr_frame) ][0] * m_scale;
        vertlist[i][1] = m_vertices[ i + (numVertices * m_anim.curr_frame) ][1] * m_scale;
        vertlist[i][2] = m_vertices[ i + (numVertices * m_anim.curr_frame) ][2] * m_scale;
    }
}

int MD2Loader::LoadSkin( const char *filename )
{
	return m_texid=TEXMANAGER.LoadTexture("hobgoblin.tga",5333);
   /* return (m_texid != TEXMANAGER.LoadTexture( "Texture.tga" ));
	return 0;*/
}