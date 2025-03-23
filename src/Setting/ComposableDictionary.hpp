#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <variant>

#include "Utils/Vector2f.hpp"
#include "Utils/KRandom.hpp"

template<typename TValue>
struct ComposableDictionary {
    std::map<std::string, TValue> add;
    std::vector<std::string> remove;

    void Merge(const ComposableDictionary &other)
    {
        for (auto &item : other.remove) {
            add.erase(item);
        }
        for (auto &item : other.add) {
            add.emplace(item.first, std::move(item.second));
        }
    }
};

using Variant = std::variant<bool, float, std::string>;

// forward declaration
struct SubWorld;
struct MixingConfig;
class SettingsCache;
