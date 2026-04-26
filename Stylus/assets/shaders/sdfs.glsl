float sdCircle(vec2 pos, float radius)
{
    return length(pos) - radius;
}

float sdSquare(vec2 pos, float radius)
{
    return length(min(pos, -radius) + max(pos, radius));
}

float sdTriangle(vec2 pos, float radius)
{
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
