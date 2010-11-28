uniform int environmentmap;

void main()
{	
	gl_Position = ftransform();		
	
	gl_TexCoord[0] = gl_MultiTexCoord0;

	vec3 u = normalize( vec3(gl_ModelViewMatrix * gl_Vertex) ); //View unit vector
	vec3 n = normalize( gl_NormalMatrix * gl_Normal );          //vertex unit normal vector in view space
	vec3 r = reflect( u, n );                                   //reflection of view vec around normal
	float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
	gl_TexCoord[1].s = r.x/m + 0.5;
	gl_TexCoord[1].t = r.y/m + 0.5;
	
}