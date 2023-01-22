struct ModelViewProjection
{
    matrix Model;
    matrix ViewProjection;
};

struct Color {
    uint R;
    uint G;
    uint B;
    uint A;
};

ConstantBuffer<ModelViewProjection> MVP: register(b0);
ConstantBuffer<Color> ColorValue: register(b1);


struct VS_INPUT
{
    float4 Position : POSITION;
};

struct PS_INPUT {
    float4 Position: SV_POSITION;
};

PS_INPUT VSMain(VS_INPUT vertex) {
    PS_INPUT output;
    output.Position = mul(mul(MVP.ViewProjection, MVP.Model), vertex.Position);
    return output;
}

uint4 PSMain(PS_INPUT pixel): SV_TARGET {
    return float4(ColorValue.R, ColorValue.G, ColorValue.B,  ColorValue.A);
}
