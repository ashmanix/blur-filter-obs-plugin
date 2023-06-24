uniform float4x4 ViewProj;
uniform texture2d image;
uniform float targetWidth;
uniform float targetHeight;
uniform float blurSize;
uniform float sigma = 30.0;
// uniform float downscale; // downscale factor, should be > 1.0

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

float Gaussian(float val, float sigma)
{
    float pi = 3.14159265;
    float sigmaSq = sigma * sigma;
    return exp(-(val * val) / (2 * sigmaSq)) / sqrt(2 * pi * sigmaSq);
}

float4 PShaderHorizontal(VertData v_in) : TARGET
{
    float2 uv = v_in.uv;
    float2 resolution = float2(targetWidth, targetHeight);
    float4 color = float4(0.0, 0.0, 0.0, 0.0);
    float weights[9];
    float blurAmount = blurSize / resolution.x;

    // Calculate weights
    float weightSum = 0.0;
    for (int i = 0; i < 9; ++i)
    {
        weights[i] = Gaussian(float(i - 4), blurSize);
        weightSum += weights[i];
    }

    // Blur horizontally
    for (int i = -4; i <= 4; ++i)
    {
        color += image.Sample(clampSampler, uv + float2(i * blurAmount, 0.0)) * weights[i + 4];
    }
    return color / weightSum; // Normalize by the sum of the weights
}

float4 PShaderVertical(VertData v_in) : TARGET
{
    float2 uv = v_in.uv;
    float2 resolution = float2(targetWidth, targetHeight);
    float4 color = float4(0.0, 0.0, 0.0, 0.0);
    float weights[9];
    float blurAmount = blurSize / resolution.y;

    // Calculate weights
    float weightSum = 0.0;
    for (int i = 0; i < 9; ++i)
    {
        weights[i] = Gaussian(float(i - 4), blurSize);
        weightSum += weights[i];
    }

    // Blur vertically
    for (int i = -4; i <= 4; ++i)
    {
        color += image.Sample(clampSampler, uv + float2(0.0, i * blurAmount)) * weights[i + 4];
    }
    return color / weightSum; // Normalize by the sum of the weights
}

float4 PShaderBoth(VertData v_in) : TARGET
{
    float2 uv = v_in.uv;
    float2 resolution = float2(targetWidth, targetHeight);
    float4 color = float4(0.0, 0.0, 0.0, 0.0);
    float weights[9];
    float blurAmount = blurSize / resolution.y;

    // Calculate weights
    float weightSum = 0.0;
    for (int i = 0; i < 9; ++i)
    {
        weights[i] = Gaussian(float(i - 4), blurSize);
        weightSum += weights[i];
    }

    // Blur vertically
    for (int i = -4; i <= 4; ++i)
    {
        color += image.Sample(clampSampler, uv + float2(i * blurAmount, i * blurAmount)) * weights[i + 4];
    }
    return color / weightSum; // Normalize by the sum of the weights
}

float4 PShader(VertData v_in) : TARGET
{
    float2 uv = v_in.uv;
    float2 resolution = float2(targetWidth, targetHeight);
    float4 color = float4(0.0, 0.0, 0.0, 1.0);
    float2 texelSize = 1.0 / resolution;

    // Apply Gaussian horizontal blur
    for (int i = -blurSize; i <= blurSize; i++)
    {
        float weight = Gaussian(i, sigma);
        color += image.Sample(clampSampler, uv + float2(i * texelSize.x, 0)) * weight;
    }

    // Apply Gaussian vertical blur
    for (int i = -blurSize; i <= blurSize; i++)
    {
        float weight = Gaussian(i, sigma);
        color += image.Sample(clampSampler, uv + float2(0, i * texelSize.y)) * weight;
    }

    return color;
}


technique Draw {
    pass {
        vertex_shader = VSDefault(v_in);
        pixel_shader  = PShaderBoth(v_in);
    }
}
