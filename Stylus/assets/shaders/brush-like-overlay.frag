#version 450

#include "sdfs.glsl"

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform PushData {
    vec4 OverlayColour;
    vec2 MousePos;
    float Width;
    float Scale;
    int Shape;
} PushConstants;

bool IsFilled(vec2 fragCoord)
{
    vec2 samplePos = floor(PushConstants.MousePos) + vec2(0.5f);
    vec2 localPos = fragCoord - samplePos;
    float radius = floor(PushConstants.Width + 0.5f) * 0.5f;

    return GetSignedDistance(PushConstants.Shape, localPos, radius) <= 0.0f;
}

void main()
{
    vec2 fragCoord = gl_FragCoord.xy;
    bool filled = IsFilled(fragCoord);

    const vec2 offsets[4] = vec2[4](
        vec2( 1.0f,  0.0f),
        vec2(-1.0f,  0.0f),
        vec2( 0.0f,  1.0f),
        vec2( 0.0f, -1.0f)
    );

    bool nearFilled = false;
    for (int i = 0; i < 4; ++i)
    {
        nearFilled = nearFilled || IsFilled(fragCoord + offsets[i]);
    }

    float outline = (!filled && nearFilled) ? 1.0f : 0.0f;
    outColor = vec4(PushConstants.OverlayColour.rgb, PushConstants.OverlayColour.a * outline);
}
