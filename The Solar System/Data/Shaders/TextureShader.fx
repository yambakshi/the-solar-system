// GLOBALS //
matrix worldMatrix, viewMatrix, projectionMatrix;
Texture2D shaderTexture;

// SAMPLE STATES //
SamplerState SampleType
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

// BLEND STATES //
BlendState AlphaBlendingOn
{
	BlendEnable[0] = TRUE;
	DestBlend = INV_SRC_ALPHA;
	SrcBlend = SRC_ALPHA;
};

// TYPES //
struct VertexInputType
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

// VERTEX SHADER //
PixelInputType TextureVertexShader(VertexInputType input)
{
	PixelInputType output;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.texCoord = input.texCoord;

	return output;
}

// PIXEL SHADER //
float4 TexturePixelShader(PixelInputType input) : SV_Target
{
	float4 color;

	color = shaderTexture.Sample(SampleType, input.texCoord);	

	return color;
}

// TECHNIQUE //
technique10 TextureTechnique
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, TextureVertexShader()));
		SetPixelShader(CompileShader(ps_4_0, TexturePixelShader()));
		SetGeometryShader(NULL);
	}
}