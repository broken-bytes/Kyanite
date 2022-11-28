struct ModelViewProjection
{
    matrix Model;
    matrix ViewProjection;
};

struct Material {
    float Specular;
    float Emmisive;
};

struct SunLight {
    float4 Color;
    float4 Position;
    bool Active;
};

struct AmbientLight {
    float4 Color;
    float Intensity;
};

struct PointLight {
    float4 Color;
    float4 Position;
    float Radius;
    float Intensity;
    bool Active;
};
#define MAX_POINT_LIGHTS 16


struct LightData
{
    SunLight Sun;
    AmbientLight Ambient;
    PointLight Pointlights[MAX_POINT_LIGHTS];
};


struct TestCBV {
    int IntValue;
    bool BoolValue;
    float FloatValue;
    float2 Float2Value;
    float3 Float3Value;
    float4 Float4Value;
};

Texture2D<float4> DiffuseMap: register(t0);
Texture2D<float4> NormalMap: register(t1);

ConstantBuffer<ModelViewProjection> MVP: register(b0);
ConstantBuffer<LightData> Light: register(b1);
ConstantBuffer<TestCBV> CBV: register(b2);

SamplerState TextureSampler: register(s0);

struct Lighting
{
    float3 Diffuse;
    float3 Specular;
};

struct VS_INPUT
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float2 UV: TEXCOORD;
};

struct PS_INPUT {
    float4 Position: SV_POSITION;
    float4 WorldPosition: WORLD_POSITION;
    float3 Normal : NORMAL;
    float2 UV: TEXCOORD;
    TestCBV CBV: TESTCBV;
};

PS_INPUT VSMain(VS_INPUT vertex) {
    PS_INPUT output;
    output.WorldPosition = vertex.Position;
    output.Position = mul(mul(MVP.ViewProjection, MVP.Model), vertex.Position);
    output.Normal = vertex.Normal;
    output.UV = vertex.UV;

    output.Normal = mul(float4(vertex.Normal, 1), MVP.Model);
    output.Normal = normalize(output.Normal);
    output.CBV = CBV;
    return output;
}

float4 PSMain(PS_INPUT pixel): SV_TARGET {

    float3 norm = normalize(pixel.Normal);
	float3 lightDir = normalize(Light.Sun.Position - pixel.Position);
	
	// Ambient
	float4 ambient = float4(0,0,0,0 );
	ambient = Light.Ambient.Color * Light.Ambient.Intensity / 10;

	// Diffuse
    float4 diffuse = DiffuseMap.Sample(TextureSampler, pixel.UV);
	float diff = max(dot(norm, lightDir), 0.0);
	diffuse = Light.Sun.Color * (diff * diffuse);

    for(int x = 0; x < MAX_POINT_LIGHTS; x++) {
        PointLight light = Light.Pointlights[x];
        if(light.Active) {
            float lDist = distance(light.Position, pixel.Position);
            if(lDist > light.Radius) {
                continue;
            }
            float3 currLightDir = normalize(light.Position - pixel.Position);
            float currDiff = max(dot(norm, lightDir), 0.0);
            float intensity = lerp(light.Intensity, 0, lDist / light.Radius);
            float col = light.Color * intensity;
            diffuse *= col;
        }
    }  
	
	
	// Specular
	float4 specular = float4(0,0,0,0);
		// Specular
	float3 viewDir = normalize(float4(0,0,0,0) - pixel.Position);
	float3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5f);
	specular = Light.Sun.Color * 1 * 0;
	

	// Result
	float4 result = (ambient + diffuse + specular);
    return result;
}
