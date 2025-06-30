cbuffer CameraBuffer : register(b0)
{
	matrix viewProj;
};

struct VSInput
{
	float3 position	: POSITION;
	float4 color		: COLOR;
	float2 texCoord	: TEXCOORD;
	float  texIndex	: TEXINDEX;
};

struct VSOut
{
	float4 pos			: SV_Position;
	float4 color		: COLOR;
	float2 texCoord	: TEXCOORD;
	float  texIndex	: TEXINDEX;
};

VSOut main(VSInput input)
{
	VSOut vso;

	vso.color = input.color;
	vso.texCoord = input.texCoord;
	vso.texIndex = input.texIndex;
	vso.pos = mul(viewProj, float4(input.position, 1.0f));

	return vso;
}
