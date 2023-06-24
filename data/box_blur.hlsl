uniform float4x4 ViewProj;
uniform texture2d image;
uniform float targetWidth;
uniform float targetHeight;
uniform float blurSize;

sampler_state clampSampler {
    Filter = LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct VertData {
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};

VertData VSDefault(VertData v_in) {
    VertData vert_out;
    vert_out.pos = mul(float4(v_in.pos.xyz, 1.0), ViewProj);
    vert_out.uv  = v_in.uv;
    return vert_out;
}

float4 PShaderBoxBlur(VertData v_in) : TARGET {
    float2 resolution = float2(targetWidth, targetHeight);
    float2 texelSize = blurSize / resolution;  // Substitute with your actual texture size
    float4 color = float4(0, 0, 0, 0);

    // The 7x7 box blur kernel
    for (int y = -3; y <= 3; ++y) {
        for (int x = -3; x <= 3; ++x) {
            color += image.Sample(clampSampler, v_in.uv + texelSize * float2(x, y));
        }
    }
    
    color /= 49;  // Normalize by the number of samples
    return color;
}

technique Draw {
    pass {
        vertex_shader = VSDefault(v_in);
        pixel_shader  = PShaderBoxBlur(v_in);
    }
  
}
