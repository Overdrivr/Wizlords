#version 430

#define FLAG_DEFERRED 0
#define FLAG_FLIP_UVS 0
#define FLAG_INSTANCING 0

#define ALPHA_MAPPING 0
#define ALPHA_TEST 0
#define DIFFUSE_MAPPING 1
#define EMISSIVE_MAPPING 0
#define LIGHTING 1
#define NORMAL_MAPPING 0
#define PARALLAX_MAPPING 0
#define SPECULAR_MAPPING 0

/********************Entrant********************/
in mat4 InstanceData0;
in vec3 VertexPosition;
in vec3 VertexNormal;
in vec3 VertexTangent;
in vec3 VertexTexCoord;

/********************Sortant********************/
out mat3 vLightToWorld;
out vec3 vNormal;
out vec3 vTexCoord;
out vec3 vWorldPos;

/********************Uniformes********************/
uniform mat4 ViewProjMatrix;
uniform mat4 WorldMatrix;
uniform mat4 WorldViewProjMatrix;

/********************Fonctions********************/
void main()
{
#if FLAG_INSTANCING
	gl_Position = ViewProjMatrix * InstanceData0 * vec4(VertexPosition, 1.0);
#else
	gl_Position = WorldViewProjMatrix * vec4(VertexPosition, 1.0);
#endif

#if LIGHTING
	#if FLAG_INSTANCING
	mat3 rotationMatrix = mat3(InstanceData0);
	#else
	mat3 rotationMatrix = mat3(WorldMatrix);
	#endif
	
	#if NORMAL_MAPPING
	vec3 binormal = cross(VertexNormal, VertexTangent);
	vLightToWorld[0] = normalize(rotationMatrix * VertexTangent);
	vLightToWorld[1] = normalize(rotationMatrix * binormal);
	vLightToWorld[2] = normalize(rotationMatrix * VertexNormal);
	#else
	vNormal = normalize(rotationMatrix * VertexNormal);
	#endif
#endif

#if ALPHA_MAPPING || DIFFUSE_MAPPING || EMISSIVE_MAPPING || NORMAL_MAPPING || PARALLAX_MAPPING || SPECULAR_MAPPING
	#if FLAG_FLIP_UVS
	vTexCoord = vec3(VertexTexCoord.x, 1.0 - VertexTexCoord.y, VertexTexCoord.z);
	#else
	vTexCoord = VertexTexCoord;
	#endif
#endif

#if LIGHTING
	#if FLAG_INSTANCING
	vWorldPos = vec3(InstanceData0 * vec4(VertexPosition, 1.0));
	#else
	vWorldPos = vec3(WorldMatrix * vec4(VertexPosition, 1.0));
	#endif
#endif
}
