#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
//#include <gl\glaux.h>		// Header File For The Glaux Library //deprecated
#include <list>


/* 

FWIL: Copied from tutorial. Needs refactoring and testing.
Also move implementation to .cpp!
http://www.gamedev.net/reference/programming/features/scenegraph/page2.asp

*/

class CSceneNode
{
public:
  // constructor
  CSceneNode() { }

  // destructor - calls destroy
  virtual ~CSceneNode() { Destroy(); }

  // release this object from memory
  void Release() { delete this; }

  // update our scene node
  virtual void Update()
  {
    // loop through the list and update the children
    for( std::list<CSceneNode*>::iterator i = m_lstChildren.begin();
         i != m_lstChildren.end(); i++ )
    {
      (*i)->Update();
    }
  }

  // destroy all the children
  void Destroy()
  {
    for( std::list<CSceneNode*>::iterator i = m_lstChildren.begin();
         i != m_lstChildren.end(); i++ )
      (*i)->Release();
  
    m_lstChildren.clear();
  }

  // add a child to our custody
  void AddChild( CSceneNode* pNode )
  {
    m_lstChildren.push_back(pNode);
  }

protected:
  // list of children
  std::list<CSceneNode*> m_lstChildren;
};

class CGeometryNode: public CSceneNode
{
public:
  CGeometryNode() { }
  ~CGeometryNode() { }

  void Update()
  {
  // Draw our geometry here!

  CSceneNode::Update();
  }
};

class CDOFNode: public CSceneNode
{
public:
  CDOFNode() { }
  ~CDOFNode() { }

  void Initialize( float m[4][4] )
  {
    for( int i = 0; i < 4; i++ )
      for( int j = 0; j < 4; j++ )
        m_fvMatrix[i][j] = m[i][j];
  }

  void Update()
  {
    glPushMatrix();
    glLoadMatrixf( (float*)m_fvMatrix );

    CSceneNode::Update();

    glPopMatrix();
  }

private:
  float m_fvMatrix[4][4];
};

