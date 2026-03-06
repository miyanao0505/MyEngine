#include "DebugLine.hlsli"

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
ConstantBuffer<DebugLineMaterial> gMaterial : register(b1);

struct DebugLineVSInput
{
    float32_t3 position : POSITION0;
    float32_t4 color : COLOR0;
};

DebugLineVSOutput main(DebugLineVSInput input)
{
    DebugLineVSOutput output;
    
    output.position = mul(float32_t4(input.position, 1.0f), gTransformationMatrix.WVP);
    
    output.color = input.color * gMaterial.color;
    
    return output;
}