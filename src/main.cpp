#ifdef EMSCRIPTEN
#include <emscripten.h>
#else
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#define EMSCRIPTEN_KEEPALIVE
#endif

#include "config.h"
#include "Setting/SettingsCache.hpp"
#include "WorldGen.hpp"

extern "C" void jsSetGeyserInfo(uint32_t type, uint32_t count, size_t data);

enum ResultType {
    RT_Starting,
    RT_Trait,
    RT_Geyser,
    RT_Polygon,
    RT_WorldSize,
    RT_Resource
};

void WriteToBinary(const std::vector<Site> &sites)
{
    static int index = 10;
    std::vector<uint32_t> data;
    for (auto &site : sites) {
        data.push_back(site.idx);
        data.push_back(*(uint32_t *)&site.x);
        data.push_back(*(uint32_t *)&site.y);
        int count = (int)site.polygon.Vertices.size();
        if (count != 0) {
            data.push_back(count);
            for (auto &point : site.polygon.Vertices) {
                data.push_back(*(uint32_t *)&point.x);
                data.push_back(*(uint32_t *)&point.y);
            }
        }
        if (site.children && !site.children->empty()) {
            for (auto &child : *site.children) {
                data.push_back(child.idx);
                data.push_back(*(uint32_t *)&child.x);
                data.push_back(*(uint32_t *)&child.y);
                int count = (int)child.polygon.Vertices.size();
                if (count != 0) {
                    data.push_back(count);
                    for (auto &point : child.polygon.Vertices) {
                        data.push_back(*(uint32_t *)&point.x);
                        data.push_back(*(uint32_t *)&point.y);
                    }
                }
            }
        }
    }
    jsSetGeyserInfo(index++, (uint32_t)data.size(), (uint32_t)data.data());
}

class App
{
private:
    SettingsCache m_settings;

    App() = default;

public:
    static App *Instance()
    {
        static App inst;
        return &inst;
    }

    void Initialize()
    {
        uint32_t count = SETTING_ASSET_FILESIZE;
        auto data = std::make_unique<char[]>(count);
        jsSetGeyserInfo(RT_Resource, count, (size_t)data.get());
        std::string_view content(data.get(), count);
        m_settings.LoadSettingsCache(content);
    }

    bool Generate(const std::string &code);
    void SetReault(int seed, int worldType, WorldGen &worldGen,
                   std::vector<const WorldTrait *> &traits);
};

bool App::Generate(const std::string &code)
{
    if (!m_settings.CoordinateChanged(code, m_settings)) {
        LogE("parse seed code %s failed.", code.c_str());
        return false;
    }
    auto &cluster = *m_settings.cluster;
    std::vector<World *> worlds;
    for (auto &worldPlacement : cluster.worldPlacements) {
        auto itr = m_settings.worlds.find(worldPlacement.world);
        if (itr == m_settings.worlds.end()) {
            LogE("world %s was wrong.", worldPlacement.world.c_str());
            return false;
        }
        itr->second.locationType = worldPlacement.locationType;
        worlds.push_back(&itr->second);
    }
    if (worlds.size() == 1) {
        worlds[0]->locationType = LocationType::StartWorld;
    }
    m_settings.DoSubworldMixing(worlds);
    int seed = m_settings.seed;
    bool genWarpWorld = code.find("M-") == 0;
    for (size_t i = 0; i < worlds.size(); ++i) {
        auto world = worlds[i];
        int worldType = 1; // second world
        if (world->locationType == LocationType::Cluster) {
            continue;
        } else if (world->locationType == LocationType::StartWorld) {
            worldType = 0;
        } else if (!world->startingBaseTemplate.contains("::bases/warpworld")) {
            continue; // other inner cluster
        } else if (!genWarpWorld) {
            continue;
        }
        m_settings.seed = seed + i;
        auto traits = m_settings.GetRandomTraits(*world);
        for (auto trait : traits) {
            world->ApplayTraits(*trait, m_settings);
        }
        WorldGen worldGen(*world, m_settings);
        if (!worldGen.GenerateOverworld()) {
            LogE("generate overworld failed.");
            return false;
        }
        SetReault(seed, worldType, worldGen, traits);
    }
    return true;
}

void App::SetReault(int seed, int worldType, WorldGen &worldGen,
                    std::vector<const WorldTrait *> &traits)
{
    // 0 starting base, 1 traits, 2 geysers, 3 polygons, 4 world size
    Vector2i starting = worldGen.GetStarting();
    Vector2i worldSize = worldGen.GetWorldSize();
    starting.y = worldSize.y - starting.y;
    jsSetGeyserInfo(RT_Starting, worldType, (size_t)&starting);
    jsSetGeyserInfo(RT_WorldSize, 0, (size_t)&worldSize);
    std::vector<int> result;
    for (auto &item : traits) {
        uint32_t index = 0;
        for (auto &pair : m_settings.traits) {
            if (item == &pair.second) {
                result.push_back(index);
                break;
            } else {
                index++;
            }
        }
    }
    jsSetGeyserInfo(RT_Trait, (uint32_t)result.size(), (size_t)result.data());
    result.clear();
    seed += (int)m_settings.cluster->worldPlacements.size() - 1;
    auto geysers = worldGen.GetGeysers(seed);
    for (auto &item : geysers) {
        result.insert(result.end(), {item.z, item.x, worldSize.y - item.y});
    }
    jsSetGeyserInfo(RT_Geyser, (uint32_t)result.size(), (size_t)result.data());
    result.clear();
    auto &sites = worldGen.GetSites();
    for (auto &item : sites) {
        result.push_back((int)item.subworld->zoneType);
        result.push_back((int)item.polygon.Vertices.size());
        for (auto &vex : item.polygon.Vertices) {
            result.push_back(vex.x);
            result.push_back(worldSize.y - vex.y);
        }
    }
    jsSetGeyserInfo(RT_Polygon, (uint32_t)result.size(), (size_t)result.data());
}

extern "C" void EMSCRIPTEN_KEEPALIVE app_init()
{
    App::Instance()->Initialize();
}

extern "C" bool EMSCRIPTEN_KEEPALIVE app_generate(int type, int seed, int mix)
{
    const char *worlds[] = {
        "SNDST-A-",   "OCAN-A-",    "S-FRZ-",     "LUSH-A-",   "FRST-A-",
        "VOLCA-",     "BAD-A-",     "HTFST-A-",   "OASIS-A-",  "CER-A-",
        "CERS-A-",    "V-SNDST-C-", "V-OCAN-C-",  "V-SWMP-C-", "V-SFRZ-C-",
        "V-LUSH-C-",  "V-FRST-C-",  "V-VOLCA-C-", "V-BAD-C-",  "V-HTFST-C-",
        "V-OASIS-C-", "V-CER-C-",   "V-CERS-C-",  "SNDST-C-",  "CER-C-",
        "FRST-C-",    "SWMP-C-",    "M-SWMP-C-",  "M-BAD-C-",  "M-FRZ-C-",
        "M-FLIP-C-",  "M-RAD-C-",   "M-CERS-C-"};
    if (type < 0 || std::size(worlds) <= type) {
        return false;
    }
    std::string code = worlds[type];
    if (code.find("CER") != code.npos) {
        mix = mix % 5;
    }
    code += std::to_string(seed);
    code += "-0-D3-";
    code += SettingsCache::BinaryToBase36(mix);
    LogI("generate with code: %s", code.c_str());
    return App::Instance()->Generate(code);
}

#ifndef EMSCRIPTEN

int main()
{
    int type, seed, mixing;
    app_init();
    while (true) {
        std::cout << "input type, seed, mixing: ";
        std::cin >> type >> seed >> mixing;
        if (seed == 0) {
            break;
        }
        if (!app_generate(type, seed, mixing)) {
            LogE("generate failed.");
        }
    }
    return 0;
}

void jsSetGeyserInfo(uint32_t type, uint32_t count, size_t data)
{
    const char *geysers[] = {
        "低温蒸汽喷孔", "蒸汽喷孔",     "清水泉",       "低温泥浆泉",
        "污水泉",       "低温盐泥泉",   "盐水泉",       "小型火山",
        "火山",         "二氧化碳泉",   "二氧化碳喷孔", "氢气喷孔",
        "高温污氧喷孔", "含菌污氧喷孔", "氯气喷孔",     "天然气喷孔",
        "铜火山",       "铁火山",       "金火山",       "铝火山",
        "钴火山",       "渗油裂缝",     "液硫泉",       "钨火山",
        "铌火山",       "打印舱",       "储油石",       "输出端",
        "输入端",       "传送器",       "低温箱"};
    const char *traits[] = {
        "坠毁的卫星群", "冰封之友",   "不规则的原油区",   "繁茂核心",
        "金属洞穴",     "放射性地壳", "地下海洋",         "火山活跃",
        "大型石块",     "中型石块",   "混合型石块",       "小型石块",
        "被圈闭的原油", "冰冻核心",   "活跃性地质",       "晶洞",
        "休眠性地质",   "大型冰川",   "不规则的原油区",   "岩浆通道",
        "金属贫瘠",     "金属富足",   "备选的打印舱位置", "粘液菌团",
        "地下海洋",     "火山活跃"};
    switch (type) {
    default:
        break;
    case RT_Starting:
        break;
    case RT_Trait: {
        auto ptr = (uint32_t *)data;
        auto end = ptr + count;
        while (ptr < end) {
            auto index = *ptr++;
            LogI("%s", traits[index]);
        }
        break;
    }
    case RT_Geyser: {
        auto ptr = (uint32_t *)data;
        auto end = ptr + count;
        while (ptr < end) {
            auto index = *ptr++;
            auto x = *ptr++;
            auto y = *ptr++;
            LogI("%s: %d, %d", geysers[index], x, y);
        }
        break;
    }
    case RT_Polygon:
        break;
    case RT_Resource: {
        auto ptr = (char *)data;
        *ptr = 'E';
        std::ifstream fstm(SETTING_ASSET_FILEPATH, std::ios::binary);
        if (fstm.is_open()) {
            auto size = fstm.seekg(0, std::ios::end).tellg();
            if (size == count) {
                fstm.seekg(0).read(ptr, count);
            } else {
                LogE("wrong count.");
            }
        } else {
            LogE("can not open file.");
        }
        break;
    }
    }
}

#endif
