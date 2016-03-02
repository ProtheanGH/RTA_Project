//////////////////////////////////////////////////////////////////////////
// DefaultPixel.hlsl
// Description:	Nothing special about this shader. Just the default.
// 
// Created By:		Doug Berg
// Last Edited:		3.2.2016
//////////////////////////////////////////////////////////////////////////
#ifndef DEFAULT_PIXEL_HLSL
#define DEFAULT_PIXEL_HLSL



// === Globals === //
struct OUTPUT_VERTEX
{
	float4 colorOut : COLOR;
	float4 projectedCoordinate : SV_POSITION;
	float4 worldPosition : WORLDPOS;
	float4 normals : NORMALS;
	float2 uv : UV;
};
// ===


// === BUFFERS === //
cbuffer DIRECTIONAL_LIGHT_BUFFER : register(b0)
{
	float4 lightDirection : DIRECTION;
	float4 lightColor : COLOR;
}

cbuffer POINTLIGHT_BUFFER : register(b1)
{
	float4 pointLightPosition : POSITION;
	float4 pointLightColor : COLOR;
}

cbuffer SPOTLIGHT_BUFFER : register(b2)
{
	float4 spotlightPosition : POSITION;
	float4 spotlightDirection : DIRECTION;
	float4 spotlightColor : COLOR;
	float4 spotlightConeRatio : CONE_RATIO;
}

cbuffer SPECULAR_BUFFER : register(b3)
{
	float4 specularPosition : POSITION;
}
// ===


float4 main( OUTPUT_VERTEX _input ) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}


#endif	// DEFAULT_PIXEL_HLSL

