#include "NMS_DebugDraw.h"


void NMS_DebugDraw::drawLine(const Vector& from,const Vector& to,const Vector& fromColor, const Vector& toColor)
{
 	glBegin(GL_LINES);
	glColor3f(fromColor[NMS_X], fromColor[NMS_Y], fromColor[NMS_Z]);
		glVertex3d(from[NMS_X], from[NMS_Y], from[NMS_Z]);
		glColor3f(toColor[NMS_X], toColor[NMS_Y], toColor[NMS_Z]);
		glVertex3d(to[NMS_X], to[NMS_Y], to[NMS_Z]);
	glEnd();
}

void NMS_DebugDraw::drawLine(const Vector& from,const Vector& to,const Vector& color)
{
 	drawLine(from,to,color,color);
}

void NMS_DebugDraw::drawSphere(const Vector& p, int radius, const Vector& color,int lats,int longs)
{
	int i;
	int j;
	glColor4f (color[NMS_X], color[NMS_Y], color[NMS_Z], 1.0f);
	glPushMatrix ();
	glTranslatef (p[NMS_X], p[NMS_Y], p[NMS_Z]);
    for(i = 0; i <= lats; i++) 
         {
            double lat0 = 3.14159265f * (-0.5 + (double) (i - 1) / lats);
           double z0  = sin(lat0);
           double zr0 =  cos(lat0);
    
          double lat1 = 3.14159265f * (-0.5 + (double) i / lats);
           double z1 = sin(lat1);
           double zr1 = cos(lat1);
    
           glBegin(GL_QUAD_STRIP);
           for(j = 0; j <= longs; j++) {
               double lng = (2 * (double) (j - 1) * 3.14159265f)/ longs;
               double x = cos(lng);
               double y = sin(lng);
    
               glNormal3f(x * zr0, y * zr0, z0);
               glVertex3f(x * zr0, y * zr0, z0);
               glNormal3f(x * zr1, y * zr1, z1);
             glVertex3f(x * zr1, y * zr1, z1);
           }
           glEnd();
       }
	glPopMatrix();
}

void NMS_DebugDraw::drawBox(const Vector& boxMin, const Vector& boxMax, const Vector& color, int alpha)
{
	Vector halfExtent = (boxMax - boxMin) * 0.5f;
	Vector center = (boxMax + boxMin) * 0.5f;
	//glEnable(GL_BLEND); // Turn blending On
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glColor4f (color[NMS_X], color[NMS_Y], color[NMS_Z], alpha);
	glPushMatrix ();
	glTranslatef (center[NMS_X], center[NMS_Y], center[NMS_Z]);
	glScaled(2*halfExtent[0], 2*halfExtent[1], 2*halfExtent[2]);
	//glutSolidCube(1.0);
	glPopMatrix ();
	//glDisable(GL_BLEND);
}

void NMS_DebugDraw::drawTriangle(const Vector& a,const Vector& b,const Vector& c,const Vector& color,int alpha)
{
	//	if (m_debugMode > 0)
	{
		Vector n=(b-a)%(c-a);
		n=n.normal();
		glBegin(GL_TRIANGLES);		
		glColor4f(color[NMS_X], color[NMS_Y], color[NMS_Z],alpha);
		glNormal3d(n[NMS_X],n[NMS_Y],n[NMS_Z]);
		glVertex3d(a[NMS_X],a[NMS_Y],a[NMS_Z]);
		glVertex3d(b[NMS_X],b[NMS_Y],b[NMS_Z]);
		glVertex3d(c[NMS_X],c[NMS_Y],c[NMS_Z]);
		glEnd();
	}
}



void NMS_DebugDraw::draw3dText(const Vector& location,const char* textString)
{
   glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
	glDisable(GL_LIGHTING);     // need to disable lighting for proper text color

    glColor4f(0.1f, 1.0f, 0.1f, 0.2f);  // set text color
	float pos[3];
	pos[1] = location[NMS_Y];
	pos[2] = location[NMS_Z];

	const char * tempName = textString;
	
	int i = 0;
	
	// loop all characters in the string
	while(*tempName)
    {
		pos[0] = location[NMS_X]+i;
		glRasterPos3fv(pos);        // place text position
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *tempName);
        ++tempName;
		i+=5;
    }
	
    glEnable(GL_LIGHTING);
    glPopAttrib();
}