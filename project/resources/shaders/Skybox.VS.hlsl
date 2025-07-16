#include "Skybox.hlsli"

ConstantBuffer<TransformationMatrix> gTransfomationMatrix : register(b0);

struct VertexShaderInput {
	float4 position : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	output.position = mul(input.position, gTransfomationMatrix.WVP).xyww;
    output.texcoord = input.position.xyz;
	return output;
}