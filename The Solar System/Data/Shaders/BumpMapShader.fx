// GLOBALS //
matrix worldMatrix, viewMatrix, projectionMatrix; 
Texture2D texturesArray[2];
float3 lightDirection;
float4 diffuseColor;
float4 ambientColor;

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
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

// VERTEX SHADER //
PixelInputType BumpMapVertexShader(VertexInputType input)
{
	PixelInputType output;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only and then normalize the final value.
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	// Calculate the tangent vector against the world matrix only and then normalize the final value.
	output.tangent = mul(input.tangent, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);

	// Calculate the binormal vector against the world matrix only and then normalize the final value.
	output.binormal = mul(input.binormal, (float3x3)worldMatrix);
	output.binormal = normalize(output.binormal);

	return output;
}

// PIXEL SHADER //
float4 BumpMapPixelShader(PixelInputType input) : SV_Target
{
	float4 textureColor;
	float4 bumpMap;
	float3 bumpNormal;
	float3 lightDir;
	float lightIntensity;
	float4 color;


	// Sample the texture pixel at this location.
	textureColor = texturesArray[0].Sample(SampleType, input.tex);

	// Sample the pixel in the bump map.
	bumpMap = texturesArray[1].Sample(SampleType, input.tex);

	// Expand the range of the normal value from (0, +1) to (-1, +1).
	bumpMap = (bumpMap * 2.0f) - 1.0f;

	// Calculate the normal from the data in the bump map.
	bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);

	// Normalize the resulting bump normal.
	bumpNormal = normalize(bumpNormal);

	// Invert the light direction for calculations.
	lightDir = -lightDirection;

	// Calculate the amount of light on this pixel based on the bump map normal value.
	lightIntensity = saturate(dot(bumpNormal, lightDir));

	color = ambientColor;

	// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
	color += saturate(diffuseColor * lightIntensity);

	// Combine the final bump light color with the texture color.
	color = color * textureColor;

	return color;
}

// TECHNIQUE //
technique10 BumpMapTechnique
{
	pass pass0
	{
		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_4_0, BumpMapVertexShader()));
		SetPixelShader(CompileShader(ps_4_0, BumpMapPixelShader()));
		SetGeometryShader(NULL);
	}
}