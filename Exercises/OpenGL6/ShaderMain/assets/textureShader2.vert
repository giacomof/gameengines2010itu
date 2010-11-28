// Flag received from the engine
uniform int flag;

// Declaration of varying variables that will be passed to the fragment shader
varying vec3 surfaceNormal, lightDirirectionVector, HalfVectorLightEye;
varying vec4 globalAmbientalComponent, ambientalComponent, diffusiveComponent, shadingAmount;
varying float lightDistanceFromVertex;
	
void main()
{
	

	if (flag == -2 || flag == 2 || flag == -3)
	{
		// Set the vertex texture coordinate
		gl_TexCoord[0] = gl_MultiTexCoord0;

		if (flag == 2)
		{
			// Declare local shader variables
			vec3 cameraVertexVector, reflectionVector;

			// Calculate vector from camera to vertex
			cameraVertexVector = normalize( vec3(gl_ModelViewMatrix * gl_Vertex) );

			// Transform the surface normal into camera space
			surfaceNormal = normalize( gl_NormalMatrix * gl_Normal );

			// Calculate the reflection vector
			reflectionVector = reflect(cameraVertexVector, surfaceNormal);

			// Calculate the texture coordinate based on the spherical mapping formula
			float temp = 2.0 * sqrt( reflectionVector.x*reflectionVector.x + reflectionVector.y*reflectionVector.y + (reflectionVector.z+1.0)*(reflectionVector.z+1.0) );
			gl_TexCoord[0].s = reflectionVector.x/temp + 0.5;
			gl_TexCoord[0].t = reflectionVector.y/temp + 0.5;
		}

		if (flag == -3)
		{
			// Declare local shader variables
			vec4 diffusiveComponent, ambientalComponent, globalAmbientalComponent, specularComponent;
			float diffuseDotProduct, highlightsDotProduct;
			
			// Transform the surface normal into camera space
			surfaceNormal = normalize(gl_NormalMatrix * gl_Normal);

			//Calculate the the light direction vector
			lightDirirectionVector = normalize(vec3(gl_LightSource[0].position));	

			// Calculate the dot product for the diffuse component
			diffuseDotProduct = max(dot(surfaceNormal, lightDirirectionVector), 0.0);
	
			// Apply the different component of the light to the differenct component of the material
			diffusiveComponent = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
			ambientalComponent = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
			globalAmbientalComponent = gl_LightModel.ambient * gl_FrontMaterial.ambient;
	
			if (diffuseDotProduct > 0.0) {
				// Calcule the highlight
				highlightsDotProduct = max(dot(surfaceNormal, normalize(gl_LightSource[0].halfVector.xyz)), 0.0);
				specularComponent = gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(diffuseDotProduct, gl_FrontMaterial.shininess);
			}
			
			// Sum all the components of the lighting
			shadingAmount = globalAmbientalComponent + ambientalComponent + diffuseDotProduct * diffusiveComponent + specularComponent;
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
