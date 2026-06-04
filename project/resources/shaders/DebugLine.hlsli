struct DebugLineVSOutput
{
    float32_t4 position : SV_POSITION;
    float32_t4 color : COLOR0;
};

struct TransformationMatrix
{
    float32_t4x4 WVP;
};

struct DebugLineMaterial
{
    float32_t4 color;
};