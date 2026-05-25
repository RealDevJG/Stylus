#include "utils.glsl"

float sdCircle(vec2 pos, float radius)
{
    return length(pos) - radius;
}

float sdSquare(vec2 pos, float side)
{
    float radius = side * 0.5;
    vec2 d = abs(pos) - radius;
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

float sdTriangleCore(vec2 pos, float radius)
{
    const float sqrt3 = 1.732050808;

    pos.x = abs(pos.x) - radius;
    pos.y += radius / sqrt3;

    if (pos.x + sqrt3 * pos.y > 0.0)
    {
        pos = 0.5 * vec2(
            pos.x - sqrt3 * pos.y,
            -sqrt3 * pos.x - pos.y
        );
    }

    pos.x -= clamp(pos.x, -2.0 * radius, 0.0);
    return -length(pos) * sign(pos.y);
}

float sdTriangle(vec2 pos, float radius)
{
    pos.y = -pos.y;
    return sdTriangleCore(pos, radius);
}

float sdFlippedTriangle(vec2 pos, float radius)
{
    return sdTriangleCore(pos, radius);
}

float sdPentagon(vec2 pos, float radius)
{
    const vec3 k = vec3(0.809016994, 0.587785252, 0.726542528);

    pos.x = abs(pos.x);
    pos -= 2.0 * min(dot(vec2(-k.x, k.y), pos), 0.0) * vec2(-k.x, k.y);
    pos -= 2.0 * min(dot(vec2( k.x, k.y), pos), 0.0) * vec2( k.x, k.y);
    pos -= vec2(clamp(pos.x, -radius * k.z, radius * k.z), radius);

    return length(pos) * sign(pos.y);
}

float sdDiamond(vec2 pos, float radius)
{
    pos = abs(pos);
    return (pos.x + pos.y - radius) * 0.7071067811865476;
}

float sdHexagon(vec2 pos, float radius)
{
    const vec3 k = vec3(-0.866025404, 0.5, 0.577350269);

    pos = abs(pos);
    pos -= 2.0 * min(dot(k.xy, pos), 0.0) * k.xy;
    pos -= vec2(clamp(pos.x, -k.z * radius, k.z * radius), radius);

    return length(pos) * sign(pos.y);
}

float sdOctagon(vec2 pos, float radius)
{
    const vec3 k = vec3(-0.9238795325, 0.3826834323, 0.4142135623);

    pos = abs(pos);
    pos -= 2.0 * min(dot(vec2( k.x, k.y), pos), 0.0) * vec2( k.x, k.y);
    pos -= 2.0 * min(dot(vec2(-k.x, k.y), pos), 0.0) * vec2(-k.x, k.y);
    pos -= vec2(clamp(pos.x, -k.z * radius, k.z * radius), radius);

    return length(pos) * sign(pos.y);
}

float sdHexagram(vec2 pos, float radius)
{
    const vec4 k = vec4(-0.5, 0.8660254038, 0.5773502692, 1.7320508076);

    pos = abs(pos);
    pos -= 2.0 * min(dot(k.xy, pos), 0.0) * k.xy;
    pos -= 2.0 * min(dot(k.yx, pos), 0.0) * k.yx;
    pos -= vec2(clamp(pos.x, radius * k.z, radius * k.w), radius);

    return length(pos) * sign(pos.y);
}

float sdPentagram(vec2 pos, float radius)
{
    const float cos18 = 0.951056516;
    const float sin18 = 0.309016994;
    const float cos36 = 0.809016994;
    const float sin36 = 0.587785252;
    const float tan36 = 0.726542528;

    const vec2 v1 = vec2( cos36, -sin36);
    const vec2 v2 = vec2(-cos36, -sin36);
    const vec2 v3 = vec2( sin18, -cos18);

    pos.x = abs(pos.x);
    pos -= 2.0 * max(dot(v1, pos), 0.0) * v1;
    pos -= 2.0 * max(dot(v2, pos), 0.0) * v2;
    pos.x = abs(pos.x);
    pos.y -= radius;

    return length(pos - v3 * clamp(dot(pos, v3), 0.0, tan36 * radius)) * sign(pos.y * v3.x - pos.x * v3.y);
}

float sdHeart(vec2 pos, float radius)
{
    pos.y = -pos.y;
    pos /= radius;

    float shape = dot(pos, pos) - 1.0;
    return (shape * shape * shape - pos.x * pos.x * pos.y * pos.y * pos.y) * radius;
}

float GetSignedDistance(int shape, vec2 pos, float radius)
{
    if (shape == 0) return sdCircle(pos, radius);
    if (shape == 1) return sdSquare(pos, radius);
    if (shape == 2) return sdTriangle(pos, radius);
    if (shape == 3) return sdFlippedTriangle(pos, radius);
    if (shape == 4) return sdPentagon(pos, radius);
    if (shape == 5) return sdDiamond(pos, radius);
    if (shape == 6) return sdHexagon(pos, radius);
    if (shape == 7) return sdOctagon(pos, radius);
    if (shape == 8) return sdHexagram(pos, radius);
    if (shape == 9) return sdPentagram(pos, radius);
    if (shape == 10) return sdHeart(pos, radius);
    return 1e10;
}
