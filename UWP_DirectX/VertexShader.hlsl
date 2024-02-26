cbuffer ConstantBuffer
{
    //float3 Offset;
    float4x4 matFinal;
    
};
struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


VOut main( float4 position : POSITION, float4 color : COLOR )
{
    VOut output;
    output.position = mul(matFinal, position);
    //output.position.x += Offset.x;
    //output.position.y += Offset.y;
    //output.position.xy *= Offset.z;
    
    output.color =  color;
    
    
    return output;
    
    
}

/* Constant Buffer
1. Adding the desired variabled to the shader.
2. Create abort C++ structure to match the variabled in the shader
3. Create a constant buffer object
4. Set up the constant buffer object
5. Update the contents of the constant using the struct as needed
*/