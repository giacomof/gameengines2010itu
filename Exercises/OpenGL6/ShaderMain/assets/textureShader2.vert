uniform int flag;

// Declaration of varying variables that will be passed to the fragment shader
varying vec3 surfaceNormal, lightDirirectionVector, HalfVectorLightEye;
varying vec4 globalAmbientalComponent, ambientalComponent, diffusiveComponent;
varying float lightDistanceFromVertex;
	
void main()
{
	if (flag == -2 || flag == 2)
	{
		// Set the vertex texture coordinate
		gl_TexCoord[0] = gl_MultiTexCoord0;

		if (flag == 2)
		{
			vec3 u = normalize( vec3(gl_ModelViewMatrix * gl_Vertex) );
			vec3 n = normalize( gl_NormalMatrix * gl_Normal );
			vec3 r = reflect( u, n );
			float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
			gl_TexCoord[0].s = r.x/m + 0.5;
			gl_TexCoord[0].t = r.y/m + 0.5;
		}
	} 

	else if (flag == 0 || flag == 1)
	{
		// Declariation of local variables
		vec3 subtractionVector;
	
		// Transform the surface normal into camera space
		surfaceNormal = normalize(gl_NormalMatrix * gl_Normal);

		//Calculate the distance between the light and the actual vertex
		subtractionVector = vec3(gl_LightSource[0].position - gl_Vertex);
		lightDirirectionVector = normalize(subtractionVector);
		lightDistanceFromVertex = length(subtractionVector);

		// Calculate and normalize the "half vector"
		HalfVectorLightEye = normalize(gl_LightSource[0].halfVector.xyz);

		// Calculate the global ambiental, ambiental and diffusive components of the light
		globalAmbientalComponent = gl_LightModel.ambient * gl_FrontMaterial.ambient;
		ambientalComponent = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
		diffusiveComponent = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
		
		// Set the vertex texture coordinate
		gl_TexCoord[0] = gl_MultiTexCoord0;
	}

	
		

	// Calculate the position of the vertex from its transformation
	gl_Position = ftransform();
} 
