uniform int environmentmap;

void main()
{	
	gl_Position = ftransform();		
	
	gl_TexCoord[0] = gl_MultiTexCoord0;

	if(environmentmap == 0) {
		gl_TexCoord[1] = gl_MultiTexCoord0;
	}
	else {
		vec3 u = normalize( vec3(gl_ModelViewMatrix * gl_Vertex) );
		vec3 n = normalize( gl_NormalMatrix * gl_Normal );
		vec3 r = reflect( u, n );
		float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
		gl_TexCoord[1].s = r.x/m + 0.5;
		gl_TexCoord[1].t = r.y/m + 0.5;
	}
}