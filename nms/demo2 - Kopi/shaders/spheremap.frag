uniform sampler2D envMap;
uniform sampler2D colorMap;
uniform int environmentmap;
uniform int textureEnabled;

void main (void)
{
	vec4 color = texture2D( colorMap, gl_TexCoord[0].st);
	vec4 env = texture2D( envMap, gl_TexCoord[1].st);

	if(environmentmap == 0) {
		gl_FragColor = color;
	}
	else if(textureEnabled == 0) {
		gl_FragColor = env*0.7;
	}
	else {
		gl_FragColor = color*0.6 + env*0.4;
	}
}