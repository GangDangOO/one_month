
struct VertexInput
{
	float4 position : POSITION0; //용도
    float4 color : COLOR0;
};
// PI
struct PixelInput
{
    float4 position : SV_POSITION; //화면좌표계 포지션
    float4 color : COLOR0;
};

//오브젝트의 크기 회전 이동
cbuffer VS_WVP : register(b0) //register(b0)
{
    matrix WVP;
}
cbuffer VS_COLOR : register(b1)
{
    //4 개의 4바이트
    float4 color;
}
//픽셀쉐이더 상수버퍼
cbuffer PS_LIGHT : register(b0)
{
	float2 ScreenPos; //스크린좌표
	float Radius; //반지름크기
	float Select; //남는값
	float4 LightColor; //조명 색
	float4 OutColor; //외곽 색
};



PixelInput VS(VertexInput input)
{
    //공간변환이 있을 예정인 곳
    PixelInput output;
    
    output.position = mul(input.position, WVP);
	output.color = input.color + (color * 2.0f - 1.0f);
	saturate(output.color);
    
	return output;
}
//->pixel  PS   ->color
//픽셀쉐이더 진입 함수
float4 PS(PixelInput input) : SV_TARGET //SV_TARGET 은 타겟이될 색깔 
{
    //뷰포트까지 변환이 끝났을때
    //스크린좌표
    
	float4 OutputColor;
	OutputColor = saturate(input.color);
    
	float2 Minus = input.position.xy - ScreenPos;
	float dis = Minus.x * Minus.x + Minus.y * Minus.y;
	dis = sqrt(dis);
	if (Select == 0.0f)
	{
	if (dis > Radius)
	{
		OutputColor.rgb += (OutColor * 2.0f - 1.0f);
	}
	else
	{
		OutputColor.rgb += (LightColor * 2.0f - 1.0f);
	}
	}
	 else
	{
		float temp = 1.0f - saturate(dis / Radius);
		OutputColor.rgb += (LightColor.rgb * 2.0f - 1.0f);
		OutputColor.rgb *= temp;
	}
	OutputColor = saturate(OutputColor);
    
	return OutputColor;
}