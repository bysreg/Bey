struct v2f
{
	float3 posH : SV_POSITION;
	float4 color : COLOR;
}

float FS(v2f input) : SV_Target
{
	return input.color;
}