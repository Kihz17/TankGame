// Vertex shader
#version 420

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec4 vColor;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModelInverseTranspose;	// For normal calculation
	
out vec4 fVertWorldLocation;
out vec4 fNormal;
out vec2 fTextureCoordinates;
out vec4 fColor;

void main()
{	
	// Apply transforms to this vertex
	mat4 MVP = matProjection * matView * matModel;
    gl_Position = MVP * vec4(vPosition, 1.0f); 	

	fColor = vColor; 
	
	// The location of the vertex in "world" space (not screen space)
	fVertWorldLocation = matModel * vec4(vPosition, 1.0f);
	
	// Calculate the normal based on any rotation we've applied.
	// This inverse transpose removes scaling and translation (movement) 
	// 	from the matrix.
	fNormal = matModelInverseTranspose * vec4(vNormal, 1.0f);
	fNormal = normalize(fNormal); // Make sure normal is a unit vector
};
