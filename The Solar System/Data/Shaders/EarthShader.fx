// GLOBALS //
matrix worldMatrix, viewMatrix, projectionMatrix; 
Texture2D texturesArray[5];
float3 lightDirection;
float4 diffuseColor;

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
PixelInputType EarthVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
	
	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);
	
	// Calculate the tangent vector against the world matrix only.
	output.tangent = mul(input.tangent, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);

	// Calculate the binormal vector against the world matrix only.
	output.binormal = mul(input.binormal, (float3x3)worldMatrix);
	output.binormal = normalize(output.binormal);
	
	return output;
}

// PIXEL SHADER //
float4 EarthPixelShader(PixelInputType input) : SV_Target
{
	float4
		earthTextureColor,
		nightTextureColor,
		cloudsTextureColor,
		bumpMap,
		color,
		specularIntensity,
		specular;
	float3 bumpNormal,
		lightDir,
		reflection;
	float 
		cloudsIntensity,
		lightIntensity;

	// Sample the texture pixel at this location.
	earthTextureColor = texturesArray[0].Sample(SampleType, input.tex);

	// Sample the night texture
	nightTextureColor = texturesArray[1].Sample(SampleType, input.tex);
	
	// Sample the pixel in the bump map.
	bumpMap = texturesArray[3].Sample(SampleType, input.tex);

	// Expand the range of the normal value from (0, +1) to (-1, +1).
	bumpMap = (bumpMap * 2.0f) - 1.0f;

	// Calculate the normal from the data in the bump map.
	bumpNormal = input.normal + bumpMap.x * input.tangent + bumpMap.y * input.binormal;

	// Normalize the resulting bump normal.
	bumpNormal = normalize(bumpNormal);
	
	// Invert the light direction for calculations.
	lightDir = -lightDirection;

	// Calculate the amount of light on this pixel based on the bump map normal value.
	lightIntensity = saturate(dot(bumpNormal, lightDir));

	// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
	color = saturate(diffuseColor);
	
	// Sample the clouds texture
	cloudsTextureColor = texturesArray[2].Sample(SampleType, input.tex);

	// Calculate the amount of clouds texture in the pixel
	cloudsIntensity = lightIntensity*((cloudsTextureColor.r + cloudsTextureColor.g + cloudsTextureColor.b) / 3.0f);

	// Set the minimum clouds intensity
	if (cloudsIntensity < 0.08f)
		cloudsIntensity = 0.08f;

	// Modify the sampled clouds texture according to the amount calculated before
	cloudsTextureColor *= cloudsIntensity;

	// Calculate the amount of the earth texture in the pixel 
	earthTextureColor = (1.0f - cloudsIntensity)*((lightIntensity * earthTextureColor) + (1.0f - lightIntensity) * nightTextureColor);

	// Combine the final bump light color with the texture color.
	color *= (earthTextureColor + cloudsTextureColor);


	return color;
}

// TECHNIQUE //
technique10 EarthTechnique
{
	pass pass0
	{
		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_4_0, EarthVertexShader()));
		SetPixelShader(CompileShader(ps_4_0, EarthPixelShader()));
		SetGeometryShader(NULL);
	}
}