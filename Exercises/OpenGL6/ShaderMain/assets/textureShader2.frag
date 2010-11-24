varying vec4 diffuse,ambientGlobal, ambient;
varying vec3 normal,lightDir,halfVector;
varying float dist;
uniform sampler2D tex;
	
void main()
{
	vec3 ct,cf,n,halfV,viewV,ldir;
	vec4 texel;
	vec4 color = ambientGlobal;
	float at,af,NdotL,NdotHV,att;

	n = normalize(normal);

	NdotL = max(dot(n,normalize(lightDir)),0.0);

	if (NdotL > 0.0) 
	{
	
		att = 1.0 / (gl_LightSource[0].constantAttenuation +
				gl_LightSource[0].linearAttenuation * dist +
				gl_LightSource[0].quadraticAttenuation * dist * dist);
		color += att * (diffuse * NdotL + ambient);
		
		halfV = normalize(halfVector);
		NdotHV = max(dot(n,halfV),0.0);
		color += att * gl_FrontMaterial.specular * gl_LightSource[0].specular * 
						pow(NdotHV,gl_FrontMaterial.shininess);
	}
		
	cf =  gl_FrontMaterial.diffuse.rgb + gl_FrontMaterial.ambient.rgb;
	af = gl_FrontMaterial.diffuse.a;
		
	texel = texture2D(tex,gl_TexCoord[0].st);
	ct = texel.rgb;
	at = texel.a;
		
	gl_FragColor = vec4(ct * cf, at * af) * color;	
}

