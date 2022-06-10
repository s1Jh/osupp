#include "Math.hpp"

#include "Random.hpp"

NS_BEGIN
// Quadratic functions where roots = { -a; +a }
float QuadRR(float x, float a) {
    return float(-std::pow(x / a, 2)) + 1.f;
}

// Linear function which equals to 0 at -a and 1 at +a
float LinearES(float x, float a) {
    return (x + a) / (2 * a);
}

// Linear function which equals to 1 at -a and 0 at +a
float LinearSE(float x, float a) {
    return 1.f - LinearES(x, a);
}

// Linear function which equals to 1 at 0 and 0 at -a and +a
float LinearUD(float x, float a) {
    return 1 - std::abs(x / a);
}

double Perlin2D(double x, double y, unsigned int octaves, double persistence, int seed, double amplitude) {
    double ret = 0;

    int ix = (int) x;
    int iy = (int) y;

    int s = (ix + iy) * (ix + iy + 1) / 2 + iy + seed;

    for (unsigned int i = 0; i < octaves; i++) {
        std::srand(s);
        ret += amplitude * Random::Scalar<float>(0.0f, 1.0f);
        amplitude *= persistence;
    }
    return ret;
}

double BiLerp(double a, double b, double c, double d, double x, double y) {
    double i0 = Lerp(a, b, x);
    double i1 = Lerp(c, d, x);

    double value = Lerp(i0, i1, y);
    return value;
}

NS_END
