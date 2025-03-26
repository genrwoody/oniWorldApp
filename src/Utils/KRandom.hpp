#pragma once

#include <cstdint>
#include <limits>

class KRandom
{
private:
    int m_inext;
    int m_inextp;
    int m_seeds[56];

public:
    KRandom(int seed);

    int Next() { return InternalSample(); }

    int Next(int maxValue) { return (int)(Sample() * maxValue); }

    int Next(int minValue, int maxValue)
    {
        int64_t num = (int64_t)maxValue - (int64_t)minValue;
        if (num <= std::numeric_limits<int>::max()) {
            return (int)(Sample() * num) + minValue;
        }
        return (int)(GetSampleForLargeRange() * num) + minValue;
    }

    float Next(float minValue, float maxValue)
    {
        float num = maxValue - minValue;
        return minValue + (float)(Sample() * num);
    }

    double NextDouble() { return Sample(); }

    float NextSingle() { return (float)Sample(); }

private:
    int InternalSample();
    double Sample()
    {
        return InternalSample() * 4.656612875245797E-10; // 1/max_int
    }
    double GetSampleForLargeRange();
};
