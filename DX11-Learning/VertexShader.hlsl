struct VSO
{
    float4 color : COLOR;
    float4 position : SV_POSITION;
};

cbuffer CBuffer
{
    matrix transform;
};

VSO main(float2 pos : POSITION, float4 color : COLOR)
{
    VSO vso;
    vso.position = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
    vso.color = color;

	return vso;
}