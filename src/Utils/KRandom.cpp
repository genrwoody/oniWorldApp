#include "KRandom.hpp"

#include <cmath>

KRandom::KRandom(int seed)
{
    constexpr int MAGIC = 161803398; // magic number based on Phi (golden ratio)
    int subtraction = std::numeric_limits<int>::max();
    if (seed != std::numeric_limits<int>::min())
        subtraction = std::abs(seed);
    int num = MAGIC - subtraction;
    m_seeds[55] = num;
    int num2 = 1;
    for (int i = 1; i < 55; i++) {
        int num3 = 21 * i % 55;
        m_seeds[num3] = num2;
        num2 = num - num2;
        if (num2 < 0) {
            num2 += std::numeric_limits<int>::max();
        }
        num = m_seeds[num3];
    }
    for (int j = 1; j < 5; j++) {
        for (int k = 1; k < 56; k++) {
            m_seeds[k] -= m_seeds[1 + (k + 30) % 55];
            if (m_seeds[k] < 0) {
                m_seeds[k] += std::numeric_limits<int>::max();
            }
        }
    }
    m_inext = 0;
    m_inextp = 21;
}

int KRandom::InternalSample()
{
    int num = m_inext;
    int num2 = m_inextp;
    if (++num >= 56) {
        num = 1;
    }
    if (++num2 >= 56) {
        num2 = 1;
    }
    int num3 = m_seeds[num] - m_seeds[num2];
    if (num3 == std::numeric_limits<int>::max()) {
        num3--;
    }
    if (num3 < 0) {
        num3 += std::numeric_limits<int>::max();
    }
    m_seeds[num] = num3;
    m_inext = num;
    m_inextp = num2;
    return num3;
}

double KRandom::GetSampleForLargeRange()
{
    int num = InternalSample();
    if (InternalSample() % 2 == 0) {
        num = -num;
    }
    return (std::numeric_limits<int>::max() - 1.0 + num) /
           (std::numeric_limits<unsigned>::max() - 2.0);
}
