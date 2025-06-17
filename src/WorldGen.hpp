#pragma once

#include <memory>

#include "Setting/SettingsCache.hpp"
#include "Utils/KRandom.hpp"
#include "Utils/Diagram.hpp"

struct TemplateSpawner {
    Vector2f position;
    const TemplateContainer *container;
};

class WorldGen
{
private:
    int m_seed;
    const SettingsCache &m_settings;
    const World &m_world;
    WorldPlacement *m_placement = nullptr;
    std::vector<Site> m_sites;
    std::vector<TemplateSpawner> m_templates;

public:
    WorldGen(World &world, SettingsCache &settings)
        : m_seed{settings.seed}
        , m_settings{settings}
        , m_world{world}
    {
    }

    bool GenerateOverworld();

    std::vector<Vector3i> GetGeysers(int seed);
    Vector2i GetStarting() { return {m_sites[0].x, m_sites[0].y}; }
    Vector2i GetWorldSize() { return m_world.worldsize; }
    std::vector<Site> &GetSites() { return m_sites; }

private:
    template<typename T>
    const T &GetDefaultData(const std::string &key)
    {
        return m_settings.GetDefaultData<T>(m_world, key);
    }
    std::vector<Site> &GenerateSeedPoints(KRandom &random);
    void PropagateDistanceTags(std::vector<Site> &sites) const;
    void ConvertUnknownCells(std::vector<Site> &allSites, KRandom &random);
    void GenerateChildren(Site &site, KRandom &random, int seed, bool usePD);
    void SetFeatureBiome(Site &site, KRandom &random, const Feature *feature);
    bool DetermineTemplates(std::vector<Site *> &sites, KRandom &random);
};

// clang-format off
inline std::string ZoneTypeToString(ZoneType zone)
{
    const char *dict[] = {
        "FrozenWastes", "CrystalCaverns",    "BoggyMarsh",     "Sandstone",
        "ToxicJungle",  "MagmaCore",         "OilField",       "Space",
        "Ocean",        "Rust",              "Forest",         "Radioactive",
        "Swamp",        "Wasteland",         "RocketInterior", "Metallic",
        "Barren",       "Moo",               "IceCaves",       "CarrotQuarry",
        "SugarWoods",   "PrehistoricGarden", "PrehistoricRaptor",
        "PrehistoricWetlands"};
    return dict[(int)zone];
}
// clang-format on

inline std::string TempRangeToString(Range range)
{
    const char *dict[] = {
        "ExtremelyCold", "VeryVeryCold", "VeryCold",    "Cold",      "Chilly",
        "Cool",          "Mild",         "Room",        "HumanWarm", "HumanHot",
        "Hot",           "VeryHot",      "ExtremelyHot"};
    return dict[(int)range];
}
