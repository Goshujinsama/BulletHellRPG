#ifndef COMMON_H
#define COMMON_H

#define SAFE_DELETE(x) { if(x != NULL) { delete(x); x = NULL; } }
#define SAFE_DELETE_ARRAY(x) { if(x != NULL) { delete [] (x); x = NULL; } }
#define SAFE_RELEASE(x)  { if(x != NULL) { x->Release(); x = NULL; } }
#define RETURN_FAIL(x) { if(FAILED(x)) { return false; } }
#define RETURN_ERROR(x) { if((x) != 0) { return false; } }

typedef struct FLOAT4
{
	float x;
	float y;
	float z;
	float w;
} FLOAT4, *PFLOAT4;

typedef struct FLOAT3
{
	float x;
	float y;
	float z;
} FLOAT3, *PFLOAT3;

typedef struct FLOAT2
{
	float x;
	float y;
} FLOAT2, *PFLOAT2;

#endif