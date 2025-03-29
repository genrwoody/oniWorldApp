#pragma once

#include "ClusterLayout.hpp"
#include "TemplateContainer.hpp"

enum class MixingLevel {
    Disabled,
    Enabled,
    TryMixing = Enabled,
    GuranteeMixing
};

struct MixingConfig {
    std::string path;
    int type; // 0 dlc, 1 world, 2 subworld
    MixingLevel level{};
    int minCount = 0;
    int maxCount = 3;
    void *setting = nullptr;

    bool operator<(const MixingConfig &other)
    {
        if (other.minCount != minCount) {
            return other.minCount < minCount;
        }
        return other.maxCount < maxCount;
    }
};

class SettingsCache
{
public:
    ComposableDictionary<std::vector<WeightedSimHash>> borders;
    DefaultSettings defaults;
    LevelLayerSettings layers;
    MobSettings mobs;
    ComposableDictionary<River> rivers;
    ComposableDictionary<Room> rooms;
    std::map<Range, Temperature> temperatures;

    std::map<std::string, std::vector<ElementGradient>> biomes;
    std::map<std::string, ClusterLayout> clusters;
    std::map<std::string, FeatureSettings> features;
    // std::map<std::string, NoiseTree> noise;
    std::map<std::string, WorldTrait> storytraits;
    std::map<std::string, SubworldMixingSettings> subworldMixing;
    std::map<std::string, SubWorld> subworlds;
    std::map<std::string, std::vector<SubWorld *>> orderedSubworlds;
    std::map<std::string, WorldTrait> traits;
    std::map<std::string, WorldMixingSettings> worldMixing;
    std::map<std::string, World> worlds;
    std::map<std::string, DlcMixingSetting> dlcMixings;
    std::map<std::string, TemplateContainer> templates;

    std::set<Feature> traitFeatures;
    std::vector<MixingConfig> mixConfigs;

public:
    ClusterLayout *cluster = nullptr;
    int seed = 0;

private:
    int m_dlcState = 0;

public:
    bool LoadSettingsCache(const std::string &assetPath);
    bool CoordinateChanged(const std::string &text, SettingsCache &settings);
    bool IsSpaceOutEnabled() const { return (m_dlcState & 1) == 1; }
    std::vector<WorldTrait *> GetRandomTraits(World &world);
    void DoSubworldMixing(std::vector<World *> worlds);
    static uint32_t Base36ToBinary(const std::string &input);
    static std::string BinaryToBase36(uint32_t input);

    template<typename T>
    const T &GetDefaultData(const World &world, const std::string &key) const
    {
        static Variant nil;
        if (world.defaultsOverrides.has_value()) {
            auto itr = world.defaultsOverrides.value().data.find(key);
            if (itr != world.defaultsOverrides.value().data.end()) {
                return std::get<T>(itr->second);
            }
        }
        auto itr = defaults.data.find(key);
        if (itr == defaults.data.end()) {
            printf("Can not get value for '%s'.", key.c_str());
            return std::get<T>(nil);
        }
        return std::get<T>(itr->second);
    }

private:
    void ParseAndApplyMixingSettingsCode(const std::string &code);
};

inline std::string ZoneTypeToString(ZoneType zone)
{
    const char *dict[] = {"FrozenWastes", "CrystalCaverns", "BoggyMarsh",
                          "Sandstone",    "ToxicJungle",    "MagmaCore",
                          "OilField",     "Space",          "Ocean",
                          "Rust",         "Forest",         "Radioactive",
                          "Swamp",        "Wasteland",      "RocketInterior",
                          "Metallic",     "Barren",         "Moo",
                          "IceCaves",     "CarrotQuarry",   "SugarWoods"};
    return dict[(int)zone];
}

inline std::string TempRangeToString(Range range)
{
    const char *dict[] = {
        "ExtremelyCold", "VeryVeryCold", "VeryCold",    "Cold",      "Chilly",
        "Cool",          "Mild",         "Room",        "HumanWarm", "HumanHot",
        "Hot",           "VeryHot",      "ExtremelyHot"};
    return dict[(int)range];
}

#define LogI(format, ...)                                                      \
    printf("info %s:%d " format "\n", __func__, __LINE__, ##__VA_ARGS__)
#define LogE(format, ...)                                                      \
    printf("error %s:%d " format "\n", __func__, __LINE__, ##__VA_ARGS__)
