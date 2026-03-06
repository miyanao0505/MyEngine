#include "DebugLine.hlsli"

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(DebugLineVSOutput input)
{
    PixelShaderOutput output;
    
    output.color = input.color;
    
    return output;
}