struct v2f
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
};

float4 FS(v2f input) : SV_TARGET
{
	return input.color;
}