// TODO: Gather this value from the CPU
#define MAX_TEXTURES 16
#define SAMPLE_CASE(INDEX) case INDEX: texColor = textures[INDEX].Sample(samplerStates[INDEX], input.texCoord); break;

struct PSInput
{
	float4 color	: COLOR;
	float2 texCoord	: TEXCOORD;
	float  texIndex	: TEXINDEX;
};

Texture2D textures[MAX_TEXTURES] : register(t0);
SamplerState samplerStates[MAX_TEXTURES] : register(s0);

float4 main(PSInput input) : SV_Target
{
	float4 texColor = float4(1.0, 0.0, 1.0, 1.0);;
	int texIndex = int(input.texIndex);

	switch (texIndex)
	{
		SAMPLE_CASE(0)
		SAMPLE_CASE(1)
		SAMPLE_CASE(2)
		SAMPLE_CASE(3)
		SAMPLE_CASE(4)
		SAMPLE_CASE(5)
		SAMPLE_CASE(6)
		SAMPLE_CASE(7)
		SAMPLE_CASE(8)
		SAMPLE_CASE(9)
		SAMPLE_CASE(10)
		SAMPLE_CASE(11)
		SAMPLE_CASE(12)
		SAMPLE_CASE(13)
		SAMPLE_CASE(14)
		SAMPLE_CASE(15)
	}
	
	return texColor * input.color;
}
