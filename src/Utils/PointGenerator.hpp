#pragma once

#include <vector>
#include <ranges>
#include <map>

#include "Polygon.hpp"
#include "KRandom.hpp"
#include "Setting/SimHashes.hpp"

template<std::ranges::range T>
void ShuffleSeeded(T &list, KRandom &random)
{
    int num = (int)list.size();
    auto itr = list.begin();
    while (num > 1) {
        num--;
        int index = random.Next(num + 1);
        auto &first = *(itr + num);
        auto &second = *(itr + index);
        std::swap(first, second);
    }
}

template<typename T>
static T WeightedRandom_Choose(std::vector<T> &list, KRandom &random)
{
    if (list.empty()) {
        return nullptr;
    }
    float num = 0.0f;
    for (auto &item : list) {
        num += item->weight;
    }
    float num2 = random.NextSingle() * num;
    float num3 = 0.0f;
    for (auto &item : list) {
        num3 += item->weight;
        if (num3 > num2) {
            return item;
        }
    }
    return list.back();
}

template<typename T, typename Enable = void>
struct HashSet {
    static_assert(false, "unsupport");
};

// ugly hashset, only for pointer
template<typename T>
struct HashSet<T, std::enable_if_t<std::is_pointer<T>::value>> {
    std::map<T, size_t> map;

    void Append(const T &value)
    {
        auto &index = map[value];
        if (index == 0) {
            index = map.size();
        }
    }

    void UnionWith(const HashSet &rhs)
    {
        auto list = rhs.ToList();
        for (auto &item : list) {
            Append(item);
        }
    }

    void ExceptWith(const HashSet &rhs)
    {
        for (auto &item : rhs.map) {
            auto itr = map.find(item.first);
            if (itr != map.end()) {
                for (auto &pair : map) {
                    if (itr->second < pair.second) {
                        pair.second--;
                    }
                }
                map.erase(itr);
            }
        }
    }

    void IntersectWith(const HashSet &rhs)
    {
        for (auto &item : rhs.map) {
            auto itr = map.find(item.first);
            if (itr == map.end()) {
                for (auto &pair : map) {
                    if (itr->second < pair.second) {
                        pair.second--;
                    }
                }
                map.erase(itr);
            }
        }
    }

    void SymmetricExceptWith(const HashSet &rhs)
    {
        auto list = rhs.ToList();
        for (auto item : list) {
            auto itr = map.find(item);
            if (itr == map.end()) {
                Append(item);
            } else {
                for (auto &pair : map) {
                    if (itr->second < pair.second) {
                        pair.second--;
                    }
                }
                map.erase(itr);
            }
        }
    }

    size_t Size() const { return map.size(); }

    void Clear() { map.clear(); }

    std::vector<T> ToList() const
    {
        std::vector<T> list(map.size());
        for (auto &pair : map) {
            list[pair.second - 1] = pair.first;
        }
        return list;
    }
};

std::vector<Vector2f> GetRandomPoints(const Polygon &boundingArea,
                                      float density, float avoidRadius,
                                      const std::vector<Vector2f> &avoidPoints,
                                      SampleBehaviour behaviour,
                                      bool testInsideBounds, KRandom &random,
                                      bool doShuffle = true,
                                      bool testAvoidPoints = true);
