uniform int textureReference;

// Declaration of varying variables that will be received from the vertex shader
varying vec3 surfaceNormal, lightDirirectionVector, HalfVectorLightEye;
varying vec4 diffusiveComponent, globalAmbientalComponent, ambientalComponent;
varying float lightDistanceFromVertex;

// Uniform variable to access the texture shadingColor
uniform sampler2D texture;
	
void main()
{
	// Declariation of local variables
	vec3 texColor,fragColor,pixelNormal,pixelHalfVector,viewV,ldir;
	vec4 texel, shadingColor;
	float texAlpha, fragAlpha, diffuseDotProduct, highlightsDotProduct, lightAttenuation;

	// Put in the shader color the global ambiental component
	shadingColor = globalAmbientalComponent;
	// Used to disable global ambient light
	//shadingColor = vec4(0.0, 0.0, 0.0, 0.0);

	// Normalize the vertex normal per pixel
	pixelNormal = normalize(surfaceNormal);

	// Calculate the dot product for the diffuse component
	diffuseDotProduct = max(dot(pixelNormal, normalize(lightDirirectionVector)), 0.0);

	if (diffuseDotProduct > 0.0) 
	{
		// Calculate the attenuation only considering the constant one (as dafault in OpenGL)
		lightAttenuation = 1.0 / gl_LightSource[0].constantAttenuation;

		// Add to the shading color the diffusive component without considering eventual highlights
		shadingColor += lightAttenuation * ((diffusiveComponent * diffuseDotProduct) + ambientalComponent);
		
		// Normalize the half vector per pixel
		pixelHalfVector = normalize(HalfVectorLightEye);
		// Calcule the highlight
		highlightsDotProduct = max(dot(pixelNormal, pixelHalfVector), 0.0);

		// Add to the shading color the specular component
		shadingColor +=	lightAttenuation * (gl_FrontMaterial.specular * gl_LightSource[0].specular) * 
						pow(highlightsDotProduct, gl_FrontMaterial.shininess);
	}
		
	if (textureReference == 0)
	{
		gl_FragColor = shadingColor;	
	}
	else
	{
		// Extract components from OpenGL material to do the blending with the texture
		fragColor =  gl_FrontMaterial.diffuse.rgb + gl_FrontMaterial.ambient.rgb;
		fragAlpha = gl_FrontMaterial.diffuse.a;
		
		// Extract the actual texel from the texture using the UV coordinates
		texel = texture2D(texture, gl_TexCoord[0].st);
		// Extract components from the texture to do the blending with OpenGL material
		texColor = texel.rgb;
		texAlpha = texel.a;
		
		// Calculate the final color modulating the shading and the texture
		gl_FragColor = vec4(texColor * fragColor, texAlpha * fragAlpha) * shadingColor;	
	}
}

