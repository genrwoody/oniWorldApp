#pragma once

#include "World.hpp"

struct WorldPlacement {
    std::string world;
    WorldMixing worldMixing;
    MinMax allowedRings = {0, 9999};
    int buffer = 2;
    LocationType locationType{};
    int x{};
    int y{};
    int width{};
    int height{};
    bool startWorld{};

    // ignore lines

    void SetSize(Vector2f size)
    {
        width = (int)size.x;
        height = (int)size.y;
    }

    void SetPosition(Vector2f pos)
    {
        x = (int)pos.x;
        y = (int)pos.y;
    }

    bool IsMixingPlacement() const
    {
        if (worldMixing.requiredTags.size() == 0) {
            return worldMixing.forbiddenTags.size() != 0;
        }
        return true;
    }
};

struct ClusterAudioSettings {
    std::string musicWelcome;
    std::string musicFirst;
    std::string stingerDay;
    std::string stingerNight;
};

struct ClusterLayout {
    std::vector<WorldPlacement> worldPlacements;
    std::vector<SpaceMapPOIPlacement> poiPlacements;
    std::string name;
    std::string description;
    std::string welcomeMessage;
    ClusterAudioSettings clusterAudio;
    std::vector<LoreCollectionOverride> clusterUnlocks;
    std::string dlcIdFrom;
    std::vector<std::string> requiredDlcIds;
    std::vector<std::string> forbiddenDlcIds;
    int difficulty{};
    bool disableStoryTraits{};
    int fixedCoordinate{};
    Skip skip{};
    ClusterCategory clusterCategory{};
    int startWorldIndex{};
    int width{};
    int height{};
    int numRings{};
    int menuOrder{};
    std::string coordinatePrefix;
    std::vector<std::string> clusterTags;
    std::vector<SpaceMapPOIPlacement> mixedPoiPlacements;
};
