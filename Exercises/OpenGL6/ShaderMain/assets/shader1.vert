	void main() {
	
		vec3 normal, lightDir;
		vec4 diffuse;
		float NdotL;
		
		/* first transform the normal into eye space and normalize the result */
		normal = normalize(gl_NormalMatrix * gl_Normal);
		
		/* now normalize the light's direction. Note that according to the
		OpenGL specification, the light is stored in eye space. Also since 
		we're talking about a directional light, the position field is actually 
		direction */
		lightDir = normalize(vec3(gl_LightSource[0].position));
		
		/* compute the cos of the angle between the normal and lights direction. 
		The light is directional so the direction is constant for every vertex.
		Since these two are normalized the cosine is the dot product. We also 
		need to clamp the result to the [0,1] range. */
		NdotL = max(dot(normal, lightDir), 0.0);
		
		/* Compute the diffuse term */
		diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;

		/* compute the specular term if NdotL is  larger than zero */
		//if (NdotL > 0.0) {
	
		// normalize the half-vector, and then compute the 
		// cosine (dot product) with the normal
		//NdotHV = max(dot(normal, gl_LightSource[0].halfVector.xyz),0.0);
		//specular = gl_FrontMaterial.specular * gl_LightSource[0].specular * 
		//		pow(NdotHV,gl_FrontMaterial.shininess);
		//}

		
		gl_FrontColor =  NdotL * diffuse;
		
		gl_Position = ftransform();
	}
