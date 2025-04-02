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

    void Initialize() { m_settings.LoadSettingsCache(SETTING_ASSERT_FILEPATH); }

    bool Generate(const std::string &code);
    void SetReault(WorldGen &worldGen, std::vector<WorldTrait *> &traits);
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
    m_settings.DoSubworldMixing(worlds);
    for (size_t i = 0; i < worlds.size(); ++i) {
        auto world = worlds[i];
        if (worlds.size() > 1 &&
            world->locationType != LocationType::StartWorld) {
            continue;
        }
        auto traits = m_settings.GetRandomTraits(*world);
        for (auto trait : traits) {
            world->ApplayTraits(*trait, m_settings);
        }
        WorldGen worldGen(*world, m_settings);
        if (!worldGen.GenerateOverworld()) {
            LogE("generate overworld failed.");
            return false;
        }
        SetReault(worldGen, traits);
        break;
    }
    return true;
}

void App::SetReault(WorldGen &worldGen, std::vector<WorldTrait *> &traits)
{
    // 0 starting base, 1 traits, 2 geysers, 3 polygons, 4 world size
    Vector2i starting = worldGen.GetStarting();
    jsSetGeyserInfo(0, 0, (size_t)&starting);
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
    jsSetGeyserInfo(1, (uint32_t)result.size(), (size_t)result.data());
    result.clear();
    int seed = m_settings.seed - 1;
    seed += (int)m_settings.cluster->worldPlacements.size();
    auto geysers = worldGen.GetGeysers(seed);
    for (auto &item : geysers) {
        result.insert(result.end(), {item.z, item.x, item.y});
    }
    jsSetGeyserInfo(2, (uint32_t)result.size(), (size_t)result.data());
    result.clear();
    auto &sites = worldGen.GetSites();
    for (auto &item : sites) {
        result.push_back((int)item.subworld->zoneType);
        result.push_back((int)item.polygon.Vertices.size());
        for (auto &vex : item.polygon.Vertices) {
            result.push_back(vex.x);
            result.push_back(vex.y);
        }
    }
    jsSetGeyserInfo(3, (uint32_t)result.size(), (size_t)result.data());
    Vector2i worldSize = worldGen.GetWorldSize();
    jsSetGeyserInfo(4, 0, (size_t)&worldSize);
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
    if (type < 0 || sizeof(worlds) / sizeof(worlds[0]) <= type) {
        return false;
    }
    if (type == 9 || type == 10) {
        mix = mix % 5;
    }
    std::string code = worlds[type];
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
    const char *configs[] = {
        "低温蒸汽喷孔", "蒸汽喷孔",     "清水泉",       "低温泥浆泉",
        "污水泉",       "低温盐泥泉",   "盐水泉",       "小型火山",
        "火山",         "二氧化碳泉",   "二氧化碳喷孔", "氢气喷孔",
        "高温污氧喷孔", "含菌污氧喷孔", "氯气喷孔",     "天然气喷孔",
        "铜火山",       "铁火山",       "金火山",       "铝火山",
        "钴火山",       "渗油裂缝",     "液硫泉",       "钨火山",
        "铌火山",       "打印仓",       "储油石",       "输出端",
        "输入端",       "传送器",       "低温箱"};
    if (type == 2) {
        auto ptr = (uint32_t *)data;
        auto end = ptr + count;
        while (ptr < end) {
            auto index = *ptr++;
            auto x = *ptr++;
            auto y = *ptr++;
            LogI("%s: %d, %d", configs[index], x, y);
        }
    } else if (type >= 10) {
        std::string filename("sites");
        filename += std::to_string(type - 10) + ".bin";
        std::ofstream fstm(filename, std::ios::binary | std::ios::trunc);
        fstm.write((char *)data, sizeof(uint32_t) * count);
        fstm.close();
    }
}

#endif
