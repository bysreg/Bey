uniform float4x4 wvp;

struct a2v
{
	float3 posL : POSITION;
	float4 color : COLOR;
};

struct v2f
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
};

v2f VS(a2v input)
{
	v2f output;
	
	//transform to homogenous clip space
	output.posH = mul( float4(input.posL, 1.0) , wvp );
	
	return output;
}