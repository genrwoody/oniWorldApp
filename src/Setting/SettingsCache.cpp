#include "SettingsCache.hpp"

#include <cstring>
#include <string_view>
#include <set>
#include <algorithm>
#include <filesystem>
#include <ranges>
#include <regex>

#include <json/reader.h>
#include <miniz.h>

#include "JsonDeserializeGen.hpp"
#include "Utils/KRandom.hpp"
#include "Utils/Polygon.hpp"
#include "Utils/PointGenerator.hpp"
#include "Utils/SortHelper.hpp"

template<typename T>
static bool LoadJsonFile(mz_zip_archive &zip, int index, T &result)
{
    size_t size;
    char *ptr = (char *)mz_zip_reader_extract_to_heap(&zip, index, &size, 0);
    if (!ptr) {
        LogE("can not open file %d.", index);
        return false;
    }
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    Json::Value root;
    std::string errs;
    std::unique_ptr<Json::CharReader> reader{builder.newCharReader()};
    auto ret = reader->parse(ptr, ptr + size, &root, &errs);
    mz_free(ptr);
    if (!ret) {
        LogE("parse %d failed, error: %s.", index, errs.c_str());
        return false;
    }
    if (!Setting::deserialize(root, result)) {
        LogE("deserialize failed, file: %d", index);
        return false;
    }
    return true;
}

static std::string GenerateKey(const char *filename)
{
    std::string key;
    unsigned offset = 0;
    if (strstr(filename, "dlc/expansion1") != nullptr) {
        key = "expansion1::";
        offset = 15;
    } else if (strstr(filename, "dlc/dlc2") != nullptr) {
        key = "dlc2::";
        offset = 9;
    } else if (strstr(filename, "dlc/dlc3") != nullptr) {
        key = "dlc3::";
        offset = 9;
    }
    if (strstr(filename, "templates/") != nullptr) {
        offset += 10;
    } else {
        offset += 9; // worldgen/
    }
    key += filename + offset;
    key.resize(key.size() - 5);
    return key;
}

bool SettingsCache::LoadSettingsCache(const std::string_view &content)
{
    if (!defaults.data.empty()) {
        return false;
    }
    mz_zip_archive zip{};
    if (!mz_zip_reader_init_mem(&zip, content.data(), content.size(), 0)) {
        LogE("wrong content");
        return false;
    }
    std::map<std::string, std::vector<std::string>> Asubworlds;
    auto count = mz_zip_reader_get_num_files(&zip);
    for (unsigned i = 0; i < count; ++i) {
        mz_zip_archive_file_stat stat;
        if (!mz_zip_reader_file_stat(&zip, i, &stat)) {
            LogE("Failed to get file stat for file index: %d", i);
            continue;
        }
        if (mz_zip_reader_is_file_a_directory(&zip, i)) {
            continue;
        }
        if (strstr(stat.m_filename, "Asubworlds.json") != nullptr) {
            LoadJsonFile(zip, i, Asubworlds);
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/defaults.json") != nullptr) {
            LoadJsonFile(zip, i, defaults);
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/layers.json") != nullptr) {
            LoadJsonFile(zip, i, layers);
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/rooms.json") != nullptr) {
            LoadJsonFile(zip, i, rooms);
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/rivers.json") != nullptr) {
            LoadJsonFile(zip, i, rivers);
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/temperatures.json") != nullptr) {
            ComposableDictionary<Temperature> temperatureDict;
            LoadJsonFile(zip, i, temperatureDict);
            for (auto &pair : temperatureDict.add) {
                Range range;
                if (Setting::deserialize<Range>(pair.first, range)) {
                    temperatures.emplace(range, pair.second);
                }
            }
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/borders.json") != nullptr) {
            ComposableDictionary<std::vector<WeightedSimHash>> borders2;
            LoadJsonFile(zip, i, borders2);
            borders.Merge(borders2);
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/mobs.json") != nullptr) {
            MobSettings mobs2;
            LoadJsonFile(zip, i, mobs2);
            mobs.MobLookupTable.Merge(mobs2.MobLookupTable);
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/mixing.json") != nullptr) {
            if (strstr(stat.m_filename, "dlc/dlc2") != nullptr) {
                LoadJsonFile(zip, i, dlcMixings["dlc2"]);
            } else {
                LoadJsonFile(zip, i, dlcMixings["dlc3"]);
            }
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/biomes/") != nullptr) {
            std::string key = GenerateKey(stat.m_filename);
            BiomeSettings biomes2;
            LoadJsonFile(zip, i, biomes2);
            for (auto &pair : biomes2.TerrainBiomeLookupTable.add) {
                std::string name = key + "/" + pair.first;
                biomes.emplace(name, std::move(pair.second));
            }
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/clusters/") != nullptr) {
            std::string key = GenerateKey(stat.m_filename);
            LoadJsonFile(zip, i, clusters[key]);
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/features/") != nullptr) {
            std::string key = GenerateKey(stat.m_filename);
            LoadJsonFile(zip, i, features[key]);
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/noise/") != nullptr) {
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/storytraits/") != nullptr) {
            std::string key = GenerateKey(stat.m_filename);
            LoadJsonFile(zip, i, storytraits[key]);
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/subworldMixing/") != nullptr) {
            std::string key = GenerateKey(stat.m_filename);
            LoadJsonFile(zip, i, subworldMixing[key]);
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/subworlds/") != nullptr) {
            std::string key = GenerateKey(stat.m_filename);
            SubWorld &subworld = subworlds[key];
            LoadJsonFile(zip, i, subworld);
            subworld.name = key;
            subworld.EnforceTemplateSpawnRuleSelfConsistency();
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/traits/") != nullptr) {
            std::string key = GenerateKey(stat.m_filename);
            WorldTrait &trait = traits[key];
            LoadJsonFile(zip, i, trait);
            trait.filePath = key;
            for (auto &item : trait.globalFeatureMods) {
                traitFeatures.emplace(item.first);
            }
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/worldMixing/") != nullptr) {
            std::string key = GenerateKey(stat.m_filename);
            LoadJsonFile(zip, i, worldMixing[key]);
            continue;
        }
        if (strstr(stat.m_filename, "worldgen/worlds/") != nullptr) {
            std::string key = GenerateKey(stat.m_filename);
            LoadJsonFile(zip, i, worlds[key]);
            continue;
        }
        if (strstr(stat.m_filename, "templates/") != nullptr) {
            std::string key = GenerateKey(stat.m_filename);
            TemplateContainer &templt = templates[key];
            LoadJsonFile(zip, i, templt);
            templt.RefreshInfo();
            templt.name = key;
            continue;
        }
        LogE("unknown file: %s", stat.m_filename);
    }
    mz_zip_end(&zip);
    for (auto &pair : Asubworlds) {
        auto &list = orderedSubworlds[pair.first];
        for (auto &name : pair.second) {
            auto itr = subworlds.find(name);
            if (itr != subworlds.end()) {
                list.push_back(&itr->second);
            }
        }
    }
    mixConfigs = {
        {"DLC2_ID", 0},
        {"dlc2::subworldMixing/IceCavesMixingSettings", 2},
        {"dlc2::subworldMixing/CarrotQuarryMixingSettings", 2},
        {"dlc2::subworldMixing/SugarWoodsMixingSettings", 2},
        {"dlc2::worldMixing/CeresMixingSettings", 1},
        {"DLC3_ID", 0},
    };
    return true;
}

static std::vector<std::string> ParseSettingCoordinate(const std::string &coord)
{
    std::regex regex("(.+)-(\\d+)-(.+)-(.+)-(.+)");
    std::smatch match;
    std::vector<std::string> result;
    if (std::regex_match(coord, match, regex)) {
        for (auto &item : match) {
            result.emplace_back(item.str());
        }
    }
    return result;
}

uint32_t SettingsCache::Base36ToBinary(const std::string &input)
{
    uint8_t dict[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  // 0-9
                      0,  0,  0,  0,  0,  0,  0,              // 3A-40
                      10, 11, 12, 13, 14, 15, 16, 17, 18, 19, // A-J
                      20, 21, 22, 23, 24, 25, 26, 27, 28, 29, // K-T
                      30, 31, 32, 33, 34, 35};                // U-Z
    uint32_t result = 0;
    for (auto itr = input.rbegin(); itr != input.rend(); ++itr) {
        result *= 36;
        result += dict[*itr - '0'];
    }
    return result;
}

std::string SettingsCache::BinaryToBase36(uint32_t input)
{
    if (input == 0) {
        return "0";
    }
    char dict[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string result;
    while (input > 0) {
        result.push_back(dict[input % 36]);
        input /= 36;
    }
    return result;
}

bool SettingsCache::CoordinateChanged(const std::string &text,
                                      SettingsCache &settings)
{
    std::vector<std::string> codes = ParseSettingCoordinate(text);
    if (codes.size() < 4 || codes.size() > 6) {
        return false;
    }
    seed = std::stoi(codes[2]);
    cluster = nullptr;
    for (auto &pair : settings.clusters) {
        if (pair.second.coordinatePrefix == codes[1]) {
            cluster = &pair.second;
            break;
        }
    }
    if (cluster == nullptr) {
        LogE("cluster %s was wrong.", codes[1].c_str());
        return false;
    }
    m_dlcState = 0;
    for (auto &id : cluster->requiredDlcIds) {
        if (id == "EXPANSION1_ID") {
            m_dlcState |= 1;
        } else if (id == "DLC2_ID") {
            m_dlcState |= 2;
        } else if (id == "DLC3_ID") {
            m_dlcState |= 4;
        }
    }
    ParseAndApplyMixingSettingsCode(codes[5]);
    return true;
}

void SettingsCache::ParseAndApplyMixingSettingsCode(const std::string &code)
{
    auto num = Base36ToBinary(code);
    for (auto itr = mixConfigs.rbegin(); itr != mixConfigs.rend(); ++itr) {
        itr->level = (MixingLevel)(num % 5);
        itr->minCount = itr->level == MixingLevel::GuranteeMixing ? 1 : 0;
        itr->maxCount = 3;
        num /= 5;
    }
}

std::vector<const WorldTrait *>
SettingsCache::GetRandomTraits(World &world) const
{
    if (seed == 0 || world.disableWorldTraits ||
        world.worldTraitRules.empty()) {
        return {};
    }
    KRandom kRandom(seed);
    std::vector<const WorldTrait *> total;
    for (auto &pair : traits) {
        if (pair.first[0] == 't') {
            if (IsSpaceOutEnabled() && pair.second.ForbiddenSpaceOut()) {
                continue;
            }
            total.push_back(&pair.second);
        }
    }
    for (auto &pair : traits) {
        if (pair.first[0] == 'e' && IsSpaceOutEnabled()) {
            total.push_back(&pair.second);
        }
    }
    std::vector<const WorldTrait *> result;
    std::vector<std::string> names;
    std::set<std::string> except;
    for (auto &rule : world.worldTraitRules) {
        for (auto &specificTrait : rule.specificTraits) {
            if (traits.find(specificTrait) != traits.end()) {
                names.emplace_back(specificTrait);
            }
        }
        std::vector<const WorldTrait *> subtotal;
        for (auto &trait : total) {
            if (!rule.requiredTags.empty() &&
                !std::ranges::all_of(
                    trait->traitTags, [&rule](const std::string &elem) {
                        return std::ranges::contains(rule.requiredTags, elem);
                    })) {
                continue;
            }
            if (!rule.forbiddenTags.empty() &&
                std::ranges::any_of(
                    trait->traitTags, [&rule](const std::string &elem) {
                        return std::ranges::contains(rule.forbiddenTags, elem);
                    })) {
                continue;
            }
            if (std::ranges::contains(rule.forbiddenTraits, trait->filePath)) {
                continue;
            }
            if (trait->IsValid(world)) {
                subtotal.push_back(trait);
            }
        }
        int num = kRandom.Next(rule.min, std::max(rule.min, rule.max + 1));
        int count = (int)names.size();
        while ((int)names.size() < count + num && subtotal.size() > 0) {
            int index = kRandom.Next((int)subtotal.size());
            auto &worldTrait = subtotal[index];
            bool flag = false;
            for (auto &exclusiveId : worldTrait->exclusiveWith) {
                if (std::ranges::contains(names, exclusiveId)) {
                    flag = true;
                    break;
                }
            }
            for (auto &exclusiveWithTag : worldTrait->exclusiveWithTags) {
                if (std::ranges::contains(except, exclusiveWithTag)) {
                    flag = true;
                    break;
                }
            }
            if (!flag) {
                names.emplace_back(worldTrait->filePath);
                result.emplace_back(worldTrait);
                for (auto &exclusiveWithTag2 : worldTrait->exclusiveWithTags) {
                    except.emplace(exclusiveWithTag2);
                }
                auto itr = std::remove(total.begin(), total.end(), worldTrait);
                total.erase(itr, total.end());
            }
            subtotal.erase(subtotal.begin() + index);
        }
        if ((int)names.size() != count + num) {
            LogI("TraitRule on %s tried to generate %d but only generated %d",
                 world.name.c_str(), num, (int)names.size() - count);
        }
    }
    return result;
}

void SettingsCache::DoSubworldMixing(std::vector<World *> worlds)
{
    std::vector<MixingConfig *> filtered;
    for (auto &config : mixConfigs) {
        if (config.level == MixingLevel::Disabled || config.type != 2) {
            continue;
        }
        auto itr = subworldMixing.find(config.path);
        if (itr == subworldMixing.end()) {
            continue;
        }
        bool forbidden = false;
        for (auto &tag : itr->second.forbiddenClusterTags) {
            auto find = std::ranges::find(cluster->clusterTags, tag);
            if (find != cluster->clusterTags.end()) {
                forbidden = true;
                break;
            }
        }
        if (!forbidden) {
            config.setting = &(itr->second);
            filtered.push_back(&config);
        }
    }
    KRandom random(seed);
    ShuffleSeeded(worlds, random);
    ArraySortHelper::Sort(
        worlds, 0, (int)worlds.size(), [](World *a, World *b) {
            const int dict[] = {3, 1, 2};
            return dict[(int)a->locationType] < dict[(int)b->locationType];
        });
    for (auto world : worlds) {
        ShuffleSeeded(filtered, random);
        world->ClearMixingsAndTraits();
        world->ApplayMixings(filtered);
    }
}
