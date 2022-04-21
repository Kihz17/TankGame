// Fragment shader
#version 420

in vec4 fNormal;
in vec4 fVertWorldLocation;
in vec4 fColor;

out vec4 Color;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform vec4 cameraPosition;
uniform bool isIgnoreLighting;
uniform bool isOverrideColor;
uniform vec4 colorOverride;

struct sLight
{
	vec4 position;			
	vec4 diffuse;	
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 attenuation; // x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
};

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

const int MAX_LIGHTS = 10;
uniform sLight lightArray[MAX_LIGHTS];

vec4 calculateLightContrib(vec3 vertexColor, vec3 vertexNormal, vec3 vertexWorldPos, vec4 vertexSpecular);

void main()
{
	vec4 diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	if(isOverrideColor)
	{
		diffuse = colorOverride;
	}
	else
	{
		diffuse = fColor;
	}
	
	if(isIgnoreLighting)
	{
		Color = diffuse;
		return;
	}
	
	
	vec4 specularColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); // White
	Color = calculateLightContrib(diffuse.rgb, fNormal.xyz, fVertWorldLocation.xyz, specularColor.rgba); 
}

vec4 calculateLightContrib(vec3 vertexColor, vec3 vertexNormal, vec3 vertexWorldPos, vec4 vertexSpecular)
{
	vec3 normal = normalize(vertexNormal);
	vec4 vertexColorResult = vec4(vertexColor.x * 0.03f, vertexColor.y * 0.03f, vertexColor.z * 0.03f, 1.0f);
	
	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		if(lightArray[i].param2.x == 0.0f) // Light is off, do nothing
		{
			continue;
		}
		
		int lightTypeInt = int(lightArray[i].param1.x);
		if(lightTypeInt == DIRECTIONAL_LIGHT_TYPE) // Do directional light first because it has no attenuation
		{
			vec3 lightContrib = lightArray[i].diffuse.rgb;
			
			float dotProduct = dot(-lightArray[i].direction.xyz, normalize(normal.xyz)); // -1 to 1

			dotProduct = max(0.0f, dotProduct);
			
			lightContrib *= dotProduct;
			
			vertexColorResult.rgb += (vertexColor.rgb * lightArray[i].diffuse.rgb * lightContrib);
			
			continue;
		}
		
		vec3 lightDirection = lightArray[i].position.xyz - vertexWorldPos.xyz;
		float lightDistance = length(lightDirection);
		lightDirection = normalize(lightDirection);
		float dotProduct = dot(lightDirection, vertexNormal.xyz);
		dotProduct = max(0.0f, dotProduct);
		
		vec3 lightDiffuseContrib = dotProduct * lightArray[i].diffuse.rgb;
		
		vec3 reflectVector = reflect(-lightDirection, normalize(normal.xyz));
		vec3 cameraVector = normalize(cameraPosition.xyz - vertexWorldPos.xyz);
		
		vec3 lightSpecularContrib = pow(max(0.0f, dot(cameraVector, reflectVector)), vertexSpecular.w) * vertexSpecular.rgb;
		
		// Attenuation
		float attenuation = 1.0f / (lightArray[i].attenuation.x + lightArray[i].attenuation.y * lightDistance + lightArray[i].attenuation.z * lightDistance * lightDistance);
		
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;
		
		if(lightTypeInt == SPOT_LIGHT_TYPE)
		{
			vec3 directionToLight = normalize(vertexWorldPos.xyz - lightArray[i].position.xyz);
			
			float currentLightRayAngle = max(0.0f, dot(directionToLight.xyz, lightArray[i].direction.xyz));
			
			float outerConeAngleCos = cos(radians(lightArray[i].param1.z));
			float innerConeAngleCos = cos(radians(lightArray[i].param1.y));
			
			if(currentLightRayAngle < outerConeAngleCos) // We aren't in the spot light
			{
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if(currentLightRayAngle < innerConeAngleCos) // Angle is between the inner and outer cone
			{
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / (innerConeAngleCos - outerConeAngleCos);
				
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}
		}
		
		vertexColorResult.rgb += (vertexColor.rgb * lightDiffuseContrib.rgb) + (vertexSpecular.rgb * lightSpecularContrib.rgb);
	}
	
	vertexColorResult.a = 1.0f;
	return vertexColorResult;
}

// LIGHTING NOTES
// Directional Lights have no position (only direction) and are the cheapest to calculate