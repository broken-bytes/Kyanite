struct ModelViewProjection
{
    matrix Model;
    matrix ViewProjection;
};

struct Entity {
    float4 Color;
};

ConstantBuffer<ModelViewProjection> MVP: register(b0);
ConstantBuffer<Entity> EntityId: register(b1);


struct VS_INPUT
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float2 UV: TEXCOORD;
};

struct PS_INPUT {
    float4 Position: SV_POSITION;
};

PS_INPUT VSMain(VS_INPUT vertex) {
    PS_INPUT output;
    output.Position = mul(mul(MVP.ViewProjection, MVP.Model), vertex.Position);
    return output;
}

float4 PSMain(PS_INPUT pixel): SV_TARGET {
    return EntityId.Color;
}
