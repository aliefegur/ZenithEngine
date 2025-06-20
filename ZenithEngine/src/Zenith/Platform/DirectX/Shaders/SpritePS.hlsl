// TODO: Gather this value from the CPU
#define MAX_TEXTURES 32
#define SAMPLE_CASE(INDEX) case INDEX: texColor = textures[INDEX].Sample(samplerState, input.texCoord); break;

struct PSInput
{
	float4 color	: COLOR;
	float2 texCoord	: TEXCOORD0;
	float  texIndex	: TEXCOORD1;
};

Texture2D textures[MAX_TEXTURES] : register(t0);
SamplerState samplerState : register(s0);

float4 main(PSInput input) : SV_Target
{
	float4 texColor = float4(1.0, 0.0, 1.0, 1.0);;
	int texIndex = int(input.texIndex);

	if (texIndex < 16)
	{
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
	}
	else
	{
		switch (texIndex)
		{
			SAMPLE_CASE(16)
			SAMPLE_CASE(17)
			SAMPLE_CASE(18)
			SAMPLE_CASE(19)
			SAMPLE_CASE(20)
			SAMPLE_CASE(21)
			SAMPLE_CASE(22)
			SAMPLE_CASE(23)
			SAMPLE_CASE(24)
			SAMPLE_CASE(25)
			SAMPLE_CASE(26)
			SAMPLE_CASE(27)
			SAMPLE_CASE(28)
			SAMPLE_CASE(29)
			SAMPLE_CASE(30)
			SAMPLE_CASE(31)
		}
	}
	
	return texColor * input.color;
}
