struct ModelViewProjection
{
    matrix Model;
    matrix ViewProjection;
};

ConstantBuffer<ModelViewProjection> MVP: register(b0);

struct VS_INPUT
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float2 UV: TEXCOORD;
    float4 Color: COLOR;
};

struct PS_INPUT {
    float4 Position: SV_POSITION;
    float4 WorldPosition: WORLD_POSITION;
    float3 Normal : NORMAL;
    float2 UV: TEXCOORD;
    float4 Color: COLOR;
};

PS_INPUT VSMain(VS_INPUT vertex) {
    PS_INPUT output;
    output.WorldPosition = vertex.Position;
    output.Position = mul(mul(MVP.ViewProjection, MVP.Model), vertex.Position);
    output.Normal = vertex.Normal;
    output.UV = vertex.UV;
    output.Color = vertex.Color;

    output.Normal = mul(float4(vertex.Normal, 1), MVP.Model);
    output.Normal = normalize(output.Normal);
    return output;
}

float4 PSMain(PS_INPUT pixel): SV_TARGET {
    return pixel.Color;
}
