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

float sdTriangle(vec2 pos, float radius)
{
    pos.y = -pos.y;
    const float k = sqrt(3.0);

    pos.x = abs(pos.x) - radius;
    pos.y = pos.y + radius / k;

    if (pos.x + k * pos.y > 0.0)
    {
        pos = vec2(pos.x - k * pos.y, -k * pos.x - pos.y) / 2.0;
    }

    pos.x -= clamp(pos.x, -2.0 * radius, 0.0);
    return -length(pos) * sign(pos.y);
}

float GetSignedDistance(int shape, vec2 pos, float radius)
{
    if (shape == 0) return sdCircle(pos, radius);
    if (shape == 1) return sdSquare(pos, radius);
    if (shape == 2) return sdTriangle(pos, radius);
    return 1e10;
}
