/* Thu Mar  6 21:27:19 2025 Generate by tool, Do *NOT* edit! */
#pragma once

#include "JsonDeserialize.hpp"
#include "ClusterLayout.hpp"
#include "DefaultSettings.hpp"
#include "SampleDescriber.hpp"
#include "SimHashes.hpp"
#include "TemplateContainer.hpp"
#include "World.hpp"
#include "WorldGenClasses.hpp"

template<>
struct Deserializer<WorldPlacement>
{
    static bool deserialize(const Json::Value &value, WorldPlacement &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("world")) != nullptr)
            count += Setting::deserialize(*ptr, obj.world) ? 1 : 0;
        if ((ptr = value.find("worldMixing")) != nullptr)
            count += Setting::deserialize(*ptr, obj.worldMixing) ? 1 : 0;
        if ((ptr = value.find("allowedRings")) != nullptr)
            count += Setting::deserialize(*ptr, obj.allowedRings) ? 1 : 0;
        if ((ptr = value.find("buffer")) != nullptr)
            count += Setting::deserialize(*ptr, obj.buffer) ? 1 : 0;
        if ((ptr = value.find("locationType")) != nullptr)
            count += Setting::deserialize(*ptr, obj.locationType) ? 1 : 0;
        if ((ptr = value.find("x")) != nullptr)
            count += Setting::deserialize(*ptr, obj.x) ? 1 : 0;
        if ((ptr = value.find("y")) != nullptr)
            count += Setting::deserialize(*ptr, obj.y) ? 1 : 0;
        if ((ptr = value.find("width")) != nullptr)
            count += Setting::deserialize(*ptr, obj.width) ? 1 : 0;
        if ((ptr = value.find("height")) != nullptr)
            count += Setting::deserialize(*ptr, obj.height) ? 1 : 0;
        if ((ptr = value.find("startWorld")) != nullptr)
            count += Setting::deserialize(*ptr, obj.startWorld) ? 1 : 0;

        if (value.size() != count) {
            printf("object WorldPlacement parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<ClusterAudioSettings>
{
    static bool deserialize(const Json::Value &value, ClusterAudioSettings &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("musicWelcome")) != nullptr)
            count += Setting::deserialize(*ptr, obj.musicWelcome) ? 1 : 0;
        if ((ptr = value.find("musicFirst")) != nullptr)
            count += Setting::deserialize(*ptr, obj.musicFirst) ? 1 : 0;
        if ((ptr = value.find("stingerDay")) != nullptr)
            count += Setting::deserialize(*ptr, obj.stingerDay) ? 1 : 0;
        if ((ptr = value.find("stingerNight")) != nullptr)
            count += Setting::deserialize(*ptr, obj.stingerNight) ? 1 : 0;

        if (value.size() != count) {
            printf("object ClusterAudioSettings parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<ClusterLayout>
{
    static bool deserialize(const Json::Value &value, ClusterLayout &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("worldPlacements")) != nullptr)
            count += Setting::deserialize(*ptr, obj.worldPlacements) ? 1 : 0;
        if ((ptr = value.find("poiPlacements")) != nullptr)
            count += Setting::deserialize(*ptr, obj.poiPlacements) ? 1 : 0;
        if ((ptr = value.find("name")) != nullptr)
            count += Setting::deserialize(*ptr, obj.name) ? 1 : 0;
        if ((ptr = value.find("description")) != nullptr)
            count += Setting::deserialize(*ptr, obj.description) ? 1 : 0;
        if ((ptr = value.find("welcomeMessage")) != nullptr)
            count += Setting::deserialize(*ptr, obj.welcomeMessage) ? 1 : 0;
        if ((ptr = value.find("clusterAudio")) != nullptr)
            count += Setting::deserialize(*ptr, obj.clusterAudio) ? 1 : 0;
        if ((ptr = value.find("clusterUnlocks")) != nullptr)
            count += Setting::deserialize(*ptr, obj.clusterUnlocks) ? 1 : 0;
        if ((ptr = value.find("dlcIdFrom")) != nullptr)
            count += Setting::deserialize(*ptr, obj.dlcIdFrom) ? 1 : 0;
        if ((ptr = value.find("requiredDlcIds")) != nullptr)
            count += Setting::deserialize(*ptr, obj.requiredDlcIds) ? 1 : 0;
        if ((ptr = value.find("forbiddenDlcIds")) != nullptr)
            count += Setting::deserialize(*ptr, obj.forbiddenDlcIds) ? 1 : 0;
        if ((ptr = value.find("difficulty")) != nullptr)
            count += Setting::deserialize(*ptr, obj.difficulty) ? 1 : 0;
        if ((ptr = value.find("disableStoryTraits")) != nullptr)
            count += Setting::deserialize(*ptr, obj.disableStoryTraits) ? 1 : 0;
        if ((ptr = value.find("fixedCoordinate")) != nullptr)
            count += Setting::deserialize(*ptr, obj.fixedCoordinate) ? 1 : 0;
        if ((ptr = value.find("skip")) != nullptr)
            count += Setting::deserialize(*ptr, obj.skip) ? 1 : 0;
        if ((ptr = value.find("clusterCategory")) != nullptr)
            count += Setting::deserialize(*ptr, obj.clusterCategory) ? 1 : 0;
        if ((ptr = value.find("startWorldIndex")) != nullptr)
            count += Setting::deserialize(*ptr, obj.startWorldIndex) ? 1 : 0;
        if ((ptr = value.find("width")) != nullptr)
            count += Setting::deserialize(*ptr, obj.width) ? 1 : 0;
        if ((ptr = value.find("height")) != nullptr)
            count += Setting::deserialize(*ptr, obj.height) ? 1 : 0;
        if ((ptr = value.find("numRings")) != nullptr)
            count += Setting::deserialize(*ptr, obj.numRings) ? 1 : 0;
        if ((ptr = value.find("menuOrder")) != nullptr)
            count += Setting::deserialize(*ptr, obj.menuOrder) ? 1 : 0;
        if ((ptr = value.find("coordinatePrefix")) != nullptr)
            count += Setting::deserialize(*ptr, obj.coordinatePrefix) ? 1 : 0;
        if ((ptr = value.find("clusterTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.clusterTags) ? 1 : 0;
        if ((ptr = value.find("mixedPoiPlacements")) != nullptr)
            count += Setting::deserialize(*ptr, obj.mixedPoiPlacements) ? 1 : 0;

        if (value.size() != count) {
            printf("object ClusterLayout parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<BaseLocation>
{
    static bool deserialize(const Json::Value &value, BaseLocation &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("left")) != nullptr)
            count += Setting::deserialize(*ptr, obj.left) ? 1 : 0;
        if ((ptr = value.find("right")) != nullptr)
            count += Setting::deserialize(*ptr, obj.right) ? 1 : 0;
        if ((ptr = value.find("top")) != nullptr)
            count += Setting::deserialize(*ptr, obj.top) ? 1 : 0;
        if ((ptr = value.find("bottom")) != nullptr)
            count += Setting::deserialize(*ptr, obj.bottom) ? 1 : 0;

        if (value.size() != count) {
            printf("object BaseLocation parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<StartingWorldElementSetting>
{
    static bool deserialize(const Json::Value &value, StartingWorldElementSetting &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("element")) != nullptr)
            count += Setting::deserialize(*ptr, obj.element) ? 1 : 0;
        if ((ptr = value.find("amount")) != nullptr)
            count += Setting::deserialize(*ptr, obj.amount) ? 1 : 0;

        if (value.size() != count) {
            printf("object StartingWorldElementSetting parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<DefaultSettings>
{
    static bool deserialize(const Json::Value &value, DefaultSettings &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("baseData")) != nullptr)
            count += Setting::deserialize(*ptr, obj.baseData) ? 1 : 0;
        if ((ptr = value.find("data")) != nullptr)
            count += Setting::deserialize(*ptr, obj.data) ? 1 : 0;
        if ((ptr = value.find("defaultMoveTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.defaultMoveTags) ? 1 : 0;
        if ((ptr = value.find("overworldAddTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.overworldAddTags) ? 1 : 0;
        if ((ptr = value.find("startingWorldElements")) != nullptr)
            count += Setting::deserialize(*ptr, obj.startingWorldElements) ? 1 : 0;

        if (value.size() != count) {
            printf("object DefaultSettings parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<WeightedSimHash>
{
    static bool deserialize(const Json::Value &value, WeightedSimHash &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("element")) != nullptr)
            count += Setting::deserialize(*ptr, obj.element) ? 1 : 0;
        if ((ptr = value.find("weight")) != nullptr)
            count += Setting::deserialize(*ptr, obj.weight) ? 1 : 0;
        if ((ptr = value.find("overrides")) != nullptr)
            count += Setting::deserialize(*ptr, obj.overrides) ? 1 : 0;

        if (value.size() != count) {
            printf("object WeightedSimHash parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<LevelLayer>
{
    static bool deserialize(const Json::Value &value, LevelLayer &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("content")) != nullptr)
            count += Setting::deserialize(*ptr, obj.content) ? 1 : 0;
        if ((ptr = value.find("bandSize")) != nullptr)
            count += Setting::deserialize(*ptr, obj.bandSize) ? 1 : 0;
        if ((ptr = value.find("maxValue")) != nullptr)
            count += Setting::deserialize(*ptr, obj.maxValue) ? 1 : 0;

        if (value.size() != count) {
            printf("object LevelLayer parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<LevelLayerSettings>
{
    static bool deserialize(const Json::Value &value, LevelLayerSettings &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("LevelLayers")) != nullptr)
            count += Setting::deserialize(*ptr, obj.LevelLayers) ? 1 : 0;

        if (value.size() != count) {
            printf("object LevelLayerSettings parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<Mob>
{
    static bool deserialize(const Json::Value &value, Mob &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("name")) != nullptr)
            count += Setting::deserialize(*ptr, obj.name) ? 1 : 0;
        if ((ptr = value.find("selectMethod")) != nullptr)
            count += Setting::deserialize(*ptr, obj.selectMethod) ? 1 : 0;
        if ((ptr = value.find("density")) != nullptr)
            count += Setting::deserialize(*ptr, obj.density) ? 1 : 0;
        if ((ptr = value.find("avoidRadius")) != nullptr)
            count += Setting::deserialize(*ptr, obj.avoidRadius) ? 1 : 0;
        if ((ptr = value.find("sampleBehaviour")) != nullptr)
            count += Setting::deserialize(*ptr, obj.sampleBehaviour) ? 1 : 0;
        if ((ptr = value.find("doAvoidPoints")) != nullptr)
            count += Setting::deserialize(*ptr, obj.doAvoidPoints) ? 1 : 0;
        if ((ptr = value.find("dontRelaxChildren")) != nullptr)
            count += Setting::deserialize(*ptr, obj.dontRelaxChildren) ? 1 : 0;
        if ((ptr = value.find("blobSize")) != nullptr)
            count += Setting::deserialize(*ptr, obj.blobSize) ? 1 : 0;
        if ((ptr = value.find("prefabName")) != nullptr)
            count += Setting::deserialize(*ptr, obj.prefabName) ? 1 : 0;
        if ((ptr = value.find("width")) != nullptr)
            count += Setting::deserialize(*ptr, obj.width) ? 1 : 0;
        if ((ptr = value.find("height")) != nullptr)
            count += Setting::deserialize(*ptr, obj.height) ? 1 : 0;
        if ((ptr = value.find("location")) != nullptr)
            count += Setting::deserialize(*ptr, obj.location) ? 1 : 0;

        if (value.size() != count) {
            printf("object Mob parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<MobSettings>
{
    static bool deserialize(const Json::Value &value, MobSettings &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("MobLookupTable")) != nullptr)
            count += Setting::deserialize(*ptr, obj.MobLookupTable) ? 1 : 0;

        if (value.size() != count) {
            printf("object MobSettings parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<River>
{
    static bool deserialize(const Json::Value &value, River &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("element")) != nullptr)
            count += Setting::deserialize(*ptr, obj.element) ? 1 : 0;
        if ((ptr = value.find("backgroundElement")) != nullptr)
            count += Setting::deserialize(*ptr, obj.backgroundElement) ? 1 : 0;
        if ((ptr = value.find("widthCenter")) != nullptr)
            count += Setting::deserialize(*ptr, obj.widthCenter) ? 1 : 0;
        if ((ptr = value.find("widthBorder")) != nullptr)
            count += Setting::deserialize(*ptr, obj.widthBorder) ? 1 : 0;
        if ((ptr = value.find("temperature")) != nullptr)
            count += Setting::deserialize(*ptr, obj.temperature) ? 1 : 0;
        if ((ptr = value.find("maxMass")) != nullptr)
            count += Setting::deserialize(*ptr, obj.maxMass) ? 1 : 0;
        if ((ptr = value.find("flowIn")) != nullptr)
            count += Setting::deserialize(*ptr, obj.flowIn) ? 1 : 0;
        if ((ptr = value.find("flowOut")) != nullptr)
            count += Setting::deserialize(*ptr, obj.flowOut) ? 1 : 0;

        if (value.size() != count) {
            printf("object River parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<WeightedMob>
{
    static bool deserialize(const Json::Value &value, WeightedMob &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("weight")) != nullptr)
            count += Setting::deserialize(*ptr, obj.weight) ? 1 : 0;
        if ((ptr = value.find("tag")) != nullptr)
            count += Setting::deserialize(*ptr, obj.tag) ? 1 : 0;

        if (value.size() != count) {
            printf("object WeightedMob parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<Room>
{
    static bool deserialize(const Json::Value &value, Room &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("name")) != nullptr)
            count += Setting::deserialize(*ptr, obj.name) ? 1 : 0;
        if ((ptr = value.find("selectMethod")) != nullptr)
            count += Setting::deserialize(*ptr, obj.selectMethod) ? 1 : 0;
        if ((ptr = value.find("density")) != nullptr)
            count += Setting::deserialize(*ptr, obj.density) ? 1 : 0;
        if ((ptr = value.find("avoidRadius")) != nullptr)
            count += Setting::deserialize(*ptr, obj.avoidRadius) ? 1 : 0;
        if ((ptr = value.find("sampleBehaviour")) != nullptr)
            count += Setting::deserialize(*ptr, obj.sampleBehaviour) ? 1 : 0;
        if ((ptr = value.find("doAvoidPoints")) != nullptr)
            count += Setting::deserialize(*ptr, obj.doAvoidPoints) ? 1 : 0;
        if ((ptr = value.find("dontRelaxChildren")) != nullptr)
            count += Setting::deserialize(*ptr, obj.dontRelaxChildren) ? 1 : 0;
        if ((ptr = value.find("blobSize")) != nullptr)
            count += Setting::deserialize(*ptr, obj.blobSize) ? 1 : 0;
        if ((ptr = value.find("shape")) != nullptr)
            count += Setting::deserialize(*ptr, obj.shape) ? 1 : 0;
        if ((ptr = value.find("mobselection")) != nullptr)
            count += Setting::deserialize(*ptr, obj.mobselection) ? 1 : 0;
        if ((ptr = value.find("mobs")) != nullptr)
            count += Setting::deserialize(*ptr, obj.mobs) ? 1 : 0;

        if (value.size() != count) {
            printf("object Room parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<Temperature>
{
    static bool deserialize(const Json::Value &value, Temperature &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("min")) != nullptr)
            count += Setting::deserialize(*ptr, obj.min) ? 1 : 0;
        if ((ptr = value.find("max")) != nullptr)
            count += Setting::deserialize(*ptr, obj.max) ? 1 : 0;

        if (value.size() != count) {
            printf("object Temperature parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<MinMax>
{
    static bool deserialize(const Json::Value &value, MinMax &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("min")) != nullptr)
            count += Setting::deserialize(*ptr, obj.min) ? 1 : 0;
        if ((ptr = value.find("max")) != nullptr)
            count += Setting::deserialize(*ptr, obj.max) ? 1 : 0;

        if (value.size() != count) {
            printf("object MinMax parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<Override>
{
    static bool deserialize(const Json::Value &value, Override &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("massOverride")) != nullptr)
            count += Setting::deserialize(*ptr, obj.massOverride) ? 1 : 0;
        if ((ptr = value.find("massMultiplier")) != nullptr)
            count += Setting::deserialize(*ptr, obj.massMultiplier) ? 1 : 0;
        if ((ptr = value.find("temperatureOverride")) != nullptr)
            count += Setting::deserialize(*ptr, obj.temperatureOverride) ? 1 : 0;
        if ((ptr = value.find("temperatureMultiplier")) != nullptr)
            count += Setting::deserialize(*ptr, obj.temperatureMultiplier) ? 1 : 0;
        if ((ptr = value.find("diseaseOverride")) != nullptr)
            count += Setting::deserialize(*ptr, obj.diseaseOverride) ? 1 : 0;
        if ((ptr = value.find("diseaseAmountOverride")) != nullptr)
            count += Setting::deserialize(*ptr, obj.diseaseAmountOverride) ? 1 : 0;

        if (value.size() != count) {
            printf("object Override parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<SampleDescriber>
{
    static bool deserialize(const Json::Value &value, SampleDescriber &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("name")) != nullptr)
            count += Setting::deserialize(*ptr, obj.name) ? 1 : 0;
        if ((ptr = value.find("selectMethod")) != nullptr)
            count += Setting::deserialize(*ptr, obj.selectMethod) ? 1 : 0;
        if ((ptr = value.find("density")) != nullptr)
            count += Setting::deserialize(*ptr, obj.density) ? 1 : 0;
        if ((ptr = value.find("avoidRadius")) != nullptr)
            count += Setting::deserialize(*ptr, obj.avoidRadius) ? 1 : 0;
        if ((ptr = value.find("sampleBehaviour")) != nullptr)
            count += Setting::deserialize(*ptr, obj.sampleBehaviour) ? 1 : 0;
        if ((ptr = value.find("doAvoidPoints")) != nullptr)
            count += Setting::deserialize(*ptr, obj.doAvoidPoints) ? 1 : 0;
        if ((ptr = value.find("dontRelaxChildren")) != nullptr)
            count += Setting::deserialize(*ptr, obj.dontRelaxChildren) ? 1 : 0;
        if ((ptr = value.find("blobSize")) != nullptr)
            count += Setting::deserialize(*ptr, obj.blobSize) ? 1 : 0;

        if (value.size() != count) {
            printf("object SampleDescriber parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<SimHashes>
{
    static bool deserialize(const Json::Value &value, SimHashes &obj)
    {
        if (value.isInt()) {
            obj = (SimHashes)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, SimHashes> dict = {
            { "AluminumGas", SimHashes::AluminumGas },
            { "CarbonDioxide", SimHashes::CarbonDioxide },
            { "CarbonGas", SimHashes::CarbonGas },
            { "ChlorineGas", SimHashes::ChlorineGas },
            { "ContaminatedOxygen", SimHashes::ContaminatedOxygen },
            { "CopperGas", SimHashes::CopperGas },
            { "GoldGas", SimHashes::GoldGas },
            { "Helium", SimHashes::Helium },
            { "Hydrogen", SimHashes::Hydrogen },
            { "IronGas", SimHashes::IronGas },
            { "CobaltGas", SimHashes::CobaltGas },
            { "LeadGas", SimHashes::LeadGas },
            { "MercuryGas", SimHashes::MercuryGas },
            { "Methane", SimHashes::Methane },
            { "NiobiumGas", SimHashes::NiobiumGas },
            { "Fallout", SimHashes::Fallout },
            { "Oxygen", SimHashes::Oxygen },
            { "PhosphorusGas", SimHashes::PhosphorusGas },
            { "Propane", SimHashes::Propane },
            { "RockGas", SimHashes::RockGas },
            { "SaltGas", SimHashes::SaltGas },
            { "SourGas", SimHashes::SourGas },
            { "Steam", SimHashes::Steam },
            { "SteelGas", SimHashes::SteelGas },
            { "SulfurGas", SimHashes::SulfurGas },
            { "SuperCoolantGas", SimHashes::SuperCoolantGas },
            { "TungstenGas", SimHashes::TungstenGas },
            { "EthanolGas", SimHashes::EthanolGas },
            { "Syngas", SimHashes::Syngas },
            { "Brine", SimHashes::Brine },
            { "Chlorine", SimHashes::Chlorine },
            { "CrudeOil", SimHashes::CrudeOil },
            { "DirtyWater", SimHashes::DirtyWater },
            { "LiquidCarbonDioxide", SimHashes::LiquidCarbonDioxide },
            { "LiquidGunk", SimHashes::LiquidGunk },
            { "LiquidHelium", SimHashes::LiquidHelium },
            { "LiquidHydrogen", SimHashes::LiquidHydrogen },
            { "LiquidMethane", SimHashes::LiquidMethane },
            { "LiquidOxygen", SimHashes::LiquidOxygen },
            { "LiquidPhosphorus", SimHashes::LiquidPhosphorus },
            { "LiquidPropane", SimHashes::LiquidPropane },
            { "LiquidSulfur", SimHashes::LiquidSulfur },
            { "Milk", SimHashes::Milk },
            { "Magma", SimHashes::Magma },
            { "Mercury", SimHashes::Mercury },
            { "MoltenAluminum", SimHashes::MoltenAluminum },
            { "MoltenCarbon", SimHashes::MoltenCarbon },
            { "MoltenCopper", SimHashes::MoltenCopper },
            { "MoltenGlass", SimHashes::MoltenGlass },
            { "MoltenGold", SimHashes::MoltenGold },
            { "MoltenIron", SimHashes::MoltenIron },
            { "MoltenCobalt", SimHashes::MoltenCobalt },
            { "MoltenLead", SimHashes::MoltenLead },
            { "MoltenNiobium", SimHashes::MoltenNiobium },
            { "MoltenSalt", SimHashes::MoltenSalt },
            { "MoltenSteel", SimHashes::MoltenSteel },
            { "MoltenTungsten", SimHashes::MoltenTungsten },
            { "MoltenUranium", SimHashes::MoltenUranium },
            { "Naphtha", SimHashes::Naphtha },
            { "NuclearWaste", SimHashes::NuclearWaste },
            { "Petroleum", SimHashes::Petroleum },
            { "Resin", SimHashes::Resin },
            { "SaltWater", SimHashes::SaltWater },
            { "SugarWater", SimHashes::SugarWater },
            { "SuperCoolant", SimHashes::SuperCoolant },
            { "ViscoGel", SimHashes::ViscoGel },
            { "Water", SimHashes::Water },
            { "Ethanol", SimHashes::Ethanol },
            { "MoltenSyngas", SimHashes::MoltenSyngas },
            { "PhytoOil", SimHashes::PhytoOil },
            { "MoltenSucrose", SimHashes::MoltenSucrose },
            { "Aerogel", SimHashes::Aerogel },
            { "Algae", SimHashes::Algae },
            { "Aluminum", SimHashes::Aluminum },
            { "AluminumOre", SimHashes::AluminumOre },
            { "Bitumen", SimHashes::Bitumen },
            { "BleachStone", SimHashes::BleachStone },
            { "Brick", SimHashes::Brick },
            { "BrineIce", SimHashes::BrineIce },
            { "MilkIce", SimHashes::MilkIce },
            { "Carbon", SimHashes::Carbon },
            { "CarbonFibre", SimHashes::CarbonFibre },
            { "Cement", SimHashes::Cement },
            { "CementMix", SimHashes::CementMix },
            { "Ceramic", SimHashes::Ceramic },
            { "Clay", SimHashes::Clay },
            { "Copper", SimHashes::Copper },
            { "Creature", SimHashes::Creature },
            { "CrushedIce", SimHashes::CrushedIce },
            { "CrushedRock", SimHashes::CrushedRock },
            { "Cuprite", SimHashes::Cuprite },
            { "DepletedUranium", SimHashes::DepletedUranium },
            { "Diamond", SimHashes::Diamond },
            { "Dirt", SimHashes::Dirt },
            { "DirtyIce", SimHashes::DirtyIce },
            { "Electrum", SimHashes::Electrum },
            { "EnrichedUranium", SimHashes::EnrichedUranium },
            { "Fertilizer", SimHashes::Fertilizer },
            { "FoolsGold", SimHashes::FoolsGold },
            { "Fossil", SimHashes::Fossil },
            { "FrozenPhytoOil", SimHashes::FrozenPhytoOil },
            { "Fullerene", SimHashes::Fullerene },
            { "Glass", SimHashes::Glass },
            { "Gold", SimHashes::Gold },
            { "GoldAmalgam", SimHashes::GoldAmalgam },
            { "Granite", SimHashes::Granite },
            { "Ice", SimHashes::Ice },
            { "IgneousRock", SimHashes::IgneousRock },
            { "Iron", SimHashes::Iron },
            { "IronOre", SimHashes::IronOre },
            { "Cobalt", SimHashes::Cobalt },
            { "Cobaltite", SimHashes::Cobaltite },
            { "Isoresin", SimHashes::Isoresin },
            { "Katairite", SimHashes::Katairite },
            { "Lead", SimHashes::Lead },
            { "Lime", SimHashes::Lime },
            { "MilkFat", SimHashes::MilkFat },
            { "MaficRock", SimHashes::MaficRock },
            { "Niobium", SimHashes::Niobium },
            { "Corium", SimHashes::Corium },
            { "Obsidian", SimHashes::Obsidian },
            { "OxyRock", SimHashes::OxyRock },
            { "PhosphateNodules", SimHashes::PhosphateNodules },
            { "Phosphorite", SimHashes::Phosphorite },
            { "Phosphorus", SimHashes::Phosphorus },
            { "HardPolypropylene", SimHashes::HardPolypropylene },
            { "Polypropylene", SimHashes::Polypropylene },
            { "Radium", SimHashes::Radium },
            { "RefinedCarbon", SimHashes::RefinedCarbon },
            { "Regolith", SimHashes::Regolith },
            { "Rust", SimHashes::Rust },
            { "Salt", SimHashes::Salt },
            { "Sand", SimHashes::Sand },
            { "SandCement", SimHashes::SandCement },
            { "SandStone", SimHashes::SandStone },
            { "Sandstone", SimHashes::Sandstone },
            { "SedimentaryRock", SimHashes::SedimentaryRock },
            { "Slabs", SimHashes::Slabs },
            { "SlimeMold", SimHashes::SlimeMold },
            { "Snow", SimHashes::Snow },
            { "StableSnow", SimHashes::StableSnow },
            { "SolidCarbonDioxide", SimHashes::SolidCarbonDioxide },
            { "SolidChlorine", SimHashes::SolidChlorine },
            { "SolidCrudeOil", SimHashes::SolidCrudeOil },
            { "SolidHydrogen", SimHashes::SolidHydrogen },
            { "SolidMercury", SimHashes::SolidMercury },
            { "SolidMethane", SimHashes::SolidMethane },
            { "SolidNaphtha", SimHashes::SolidNaphtha },
            { "SolidOxygen", SimHashes::SolidOxygen },
            { "SolidPetroleum", SimHashes::SolidPetroleum },
            { "SolidPropane", SimHashes::SolidPropane },
            { "SolidResin", SimHashes::SolidResin },
            { "SolidSuperCoolant", SimHashes::SolidSuperCoolant },
            { "SolidViscoGel", SimHashes::SolidViscoGel },
            { "Steel", SimHashes::Steel },
            { "Sulfur", SimHashes::Sulfur },
            { "SuperInsulator", SimHashes::SuperInsulator },
            { "TempConductorSolid", SimHashes::TempConductorSolid },
            { "ToxicSand", SimHashes::ToxicSand },
            { "Tungsten", SimHashes::Tungsten },
            { "Unobtanium", SimHashes::Unobtanium },
            { "UraniumOre", SimHashes::UraniumOre },
            { "Wolframite", SimHashes::Wolframite },
            { "Yellowcake", SimHashes::Yellowcake },
            { "SolidEthanol", SimHashes::SolidEthanol },
            { "SolidSyngas", SimHashes::SolidSyngas },
            { "ToxicMud", SimHashes::ToxicMud },
            { "Mud", SimHashes::Mud },
            { "Sucrose", SimHashes::Sucrose },
            { "Graphite", SimHashes::Graphite },
            { "SolidNuclearWaste", SimHashes::SolidNuclearWaste },
            { "Cinnabar", SimHashes::Cinnabar },
            { "WoodLog", SimHashes::WoodLog },
            { "Tallow", SimHashes::Tallow },
            { "Gunk", SimHashes::Gunk },
            { "Vacuum", SimHashes::Vacuum },
            { "Void", SimHashes::Void },
            { "COMPOSITION", SimHashes::COMPOSITION }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum SimHashes::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<TagCommand>
{
    static bool deserialize(const Json::Value &value, TagCommand &obj)
    {
        if (value.isInt()) {
            obj = (TagCommand)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, TagCommand> dict = {
            { "Default", TagCommand::Default },
            { "AtTag", TagCommand::AtTag },
            { "NotAtTag", TagCommand::NotAtTag },
            { "DistanceFromTag", TagCommand::DistanceFromTag }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum TagCommand::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<Command>
{
    static bool deserialize(const Json::Value &value, Command &obj)
    {
        if (value.isInt()) {
            obj = (Command)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, Command> dict = {
            { "Clear", Command::Clear },
            { "Replace", Command::Replace },
            { "UnionWith", Command::UnionWith },
            { "IntersectWith", Command::IntersectWith },
            { "ExceptWith", Command::ExceptWith },
            { "SymmetricExceptWith", Command::SymmetricExceptWith },
            { "All", Command::All }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum Command::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<ListRule>
{
    static bool deserialize(const Json::Value &value, ListRule &obj)
    {
        if (value.isInt()) {
            obj = (ListRule)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, ListRule> dict = {
            { "GuaranteeOne", ListRule::GuaranteeOne },
            { "GuaranteeSome", ListRule::GuaranteeSome },
            { "GuaranteeSomeTryMore", ListRule::GuaranteeSomeTryMore },
            { "GuaranteeAll", ListRule::GuaranteeAll },
            { "GuaranteeRange", ListRule::GuaranteeRange },
            { "TryOne", ListRule::TryOne },
            { "TrySome", ListRule::TrySome },
            { "Trysome", ListRule::Trysome },
            { "TryRange", ListRule::TryRange },
            { "TryAll", ListRule::TryAll }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum ListRule::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<OrderRule>
{
    static bool deserialize(const Json::Value &value, OrderRule &obj)
    {
        if (value.isInt()) {
            obj = (OrderRule)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, OrderRule> dict = {
            { "Prepend", OrderRule::Prepend },
            { "Append", OrderRule::Append },
            { "Replace", OrderRule::Replace },
            { "Invalid", OrderRule::Invalid }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum OrderRule::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<ZoneType>
{
    static bool deserialize(const Json::Value &value, ZoneType &obj)
    {
        if (value.isInt()) {
            obj = (ZoneType)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, ZoneType> dict = {
            { "FrozenWastes", ZoneType::FrozenWastes },
            { "CrystalCaverns", ZoneType::CrystalCaverns },
            { "BoggyMarsh", ZoneType::BoggyMarsh },
            { "Sandstone", ZoneType::Sandstone },
            { "ToxicJungle", ZoneType::ToxicJungle },
            { "MagmaCore", ZoneType::MagmaCore },
            { "OilField", ZoneType::OilField },
            { "Space", ZoneType::Space },
            { "Ocean", ZoneType::Ocean },
            { "Rust", ZoneType::Rust },
            { "Forest", ZoneType::Forest },
            { "Radioactive", ZoneType::Radioactive },
            { "Swamp", ZoneType::Swamp },
            { "Wasteland", ZoneType::Wasteland },
            { "RocketInterior", ZoneType::RocketInterior },
            { "Metallic", ZoneType::Metallic },
            { "Barren", ZoneType::Barren },
            { "Moo", ZoneType::Moo },
            { "IceCaves", ZoneType::IceCaves },
            { "CarrotQuarry", ZoneType::CarrotQuarry },
            { "SugarWoods", ZoneType::SugarWoods }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum ZoneType::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<LocationType>
{
    static bool deserialize(const Json::Value &value, LocationType &obj)
    {
        if (value.isInt()) {
            obj = (LocationType)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, LocationType> dict = {
            { "Cluster", LocationType::Cluster },
            { "StartWorld", LocationType::StartWorld },
            { "InnerCluster", LocationType::InnerCluster }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum LocationType::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<WorldCategory>
{
    static bool deserialize(const Json::Value &value, WorldCategory &obj)
    {
        if (value.isInt()) {
            obj = (WorldCategory)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, WorldCategory> dict = {
            { "Asteroid", WorldCategory::Asteroid },
            { "Moon", WorldCategory::Moon }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum WorldCategory::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<Skip>
{
    static bool deserialize(const Json::Value &value, Skip &obj)
    {
        if (value.isInt()) {
            obj = (Skip)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, Skip> dict = {
            { "Never", Skip::Never },
            { "False", Skip::False },
            { "Always", Skip::Always },
            { "True", Skip::True },
            { "EditorOnly", Skip::EditorOnly }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum Skip::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<LayoutMethod>
{
    static bool deserialize(const Json::Value &value, LayoutMethod &obj)
    {
        if (value.isInt()) {
            obj = (LayoutMethod)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, LayoutMethod> dict = {
            { "Default", LayoutMethod::Default },
            { "VoronoiTree", LayoutMethod::VoronoiTree },
            { "PowerTree", LayoutMethod::PowerTree }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum LayoutMethod::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<SampleBehaviour>
{
    static bool deserialize(const Json::Value &value, SampleBehaviour &obj)
    {
        if (value.isInt()) {
            obj = (SampleBehaviour)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, SampleBehaviour> dict = {
            { "UniformSquare", SampleBehaviour::UniformSquare },
            { "UniformHex", SampleBehaviour::UniformHex },
            { "UniformScaledHex", SampleBehaviour::UniformScaledHex },
            { "UniformSpiral", SampleBehaviour::UniformSpiral },
            { "UniformCircle", SampleBehaviour::UniformCircle },
            { "PoissonDisk", SampleBehaviour::PoissonDisk },
            { "StdRand", SampleBehaviour::StdRand }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum SampleBehaviour::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<PointSelectionMethod>
{
    static bool deserialize(const Json::Value &value, PointSelectionMethod &obj)
    {
        if (value.isInt()) {
            obj = (PointSelectionMethod)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, PointSelectionMethod> dict = {
            { "RandomPoints", PointSelectionMethod::RandomPoints },
            { "Centroid", PointSelectionMethod::Centroid }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum PointSelectionMethod::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<Location>
{
    static bool deserialize(const Json::Value &value, Location &obj)
    {
        if (value.isInt()) {
            obj = (Location)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, Location> dict = {
            { "Floor", Location::Floor },
            { "Ceiling", Location::Ceiling },
            { "Air", Location::Air },
            { "BackWall", Location::BackWall },
            { "NearWater", Location::NearWater },
            { "NearLiquid", Location::NearLiquid },
            { "Solid", Location::Solid },
            { "Water", Location::Water },
            { "ShallowLiquid", Location::ShallowLiquid },
            { "Surface", Location::Surface },
            { "LiquidFloor", Location::LiquidFloor },
            { "AnyFloor", Location::AnyFloor }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum Location::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<Shape>
{
    static bool deserialize(const Json::Value &value, Shape &obj)
    {
        if (value.isInt()) {
            obj = (Shape)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, Shape> dict = {
            { "Circle", Shape::Circle },
            { "Oval", Shape::Oval },
            { "Blob", Shape::Blob },
            { "Line", Shape::Line },
            { "Square", Shape::Square },
            { "TallThin", Shape::TallThin },
            { "ShortWide", Shape::ShortWide },
            { "Template", Shape::Template },
            { "PhysicalLayout", Shape::PhysicalLayout },
            { "Splat", Shape::Splat }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum Shape::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<Selection>
{
    static bool deserialize(const Json::Value &value, Selection &obj)
    {
        if (value.isInt()) {
            obj = (Selection)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, Selection> dict = {
            { "None", Selection::None },
            { "OneOfEach", Selection::OneOfEach },
            { "NOfEach", Selection::NOfEach },
            { "Weighted", Selection::Weighted },
            { "WeightedBucket", Selection::WeightedBucket },
            { "WeightedResample", Selection::WeightedResample },
            { "PickOneWeighted", Selection::PickOneWeighted },
            { "HorizontalSlice", Selection::HorizontalSlice }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum Selection::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<Range>
{
    static bool deserialize(const Json::Value &value, Range &obj)
    {
        if (value.isInt()) {
            obj = (Range)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, Range> dict = {
            { "ExtremelyCold", Range::ExtremelyCold },
            { "VeryVeryCold", Range::VeryVeryCold },
            { "VeryCold", Range::VeryCold },
            { "Cold", Range::Cold },
            { "Chilly", Range::Chilly },
            { "Cool", Range::Cool },
            { "Mild", Range::Mild },
            { "Room", Range::Room },
            { "HumanWarm", Range::HumanWarm },
            { "HumanHot", Range::HumanHot },
            { "Hot", Range::Hot },
            { "VeryHot", Range::VeryHot },
            { "ExtremelyHot", Range::ExtremelyHot }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum Range::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<Type>
{
    static bool deserialize(const Json::Value &value, Type &obj)
    {
        if (value.isInt()) {
            obj = (Type)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, Type> dict = {
            { "Building", Type::Building },
            { "Ore", Type::Ore },
            { "Pickupable", Type::Pickupable },
            { "Other", Type::Other }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum Type::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<Orientation>
{
    static bool deserialize(const Json::Value &value, Orientation &obj)
    {
        if (value.isInt()) {
            obj = (Orientation)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, Orientation> dict = {
            { "Neutral", Orientation::Neutral },
            { "R90", Orientation::R90 },
            { "R180", Orientation::R180 },
            { "R270", Orientation::R270 },
            { "NumRotations", Orientation::NumRotations },
            { "FlipH", Orientation::FlipH },
            { "FlipV", Orientation::FlipV }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum Orientation::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<ClusterCategory>
{
    static bool deserialize(const Json::Value &value, ClusterCategory &obj)
    {
        if (value.isInt()) {
            obj = (ClusterCategory)value.asInt();
            return true;
        }
        auto name = value.asString();
        static std::map<std::string, ClusterCategory> dict = {
            { "Vanilla", ClusterCategory::Vanilla },
            { "SpacedOutVanillaStyle", ClusterCategory::SpacedOutVanillaStyle },
            { "SpacedOutStyle", ClusterCategory::SpacedOutStyle },
            { "Special", ClusterCategory::Special }
        };
        auto itr = dict.find(name);
        if (itr == dict.end()) {
            printf("enum ClusterCategory::%s parse failed.\n", name.c_str());
            return false;
        }
        obj = itr->second;
        return true;
    }
};

template<>
struct Deserializer<Cell>
{
    static bool deserialize(const Json::Value &value, Cell &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("element")) != nullptr)
            count += Setting::deserialize(*ptr, obj.element) ? 1 : 0;
        if ((ptr = value.find("mass")) != nullptr)
            count += Setting::deserialize(*ptr, obj.mass) ? 1 : 0;
        if ((ptr = value.find("temperature")) != nullptr)
            count += Setting::deserialize(*ptr, obj.temperature) ? 1 : 0;
        if ((ptr = value.find("diseaseName")) != nullptr)
            count += Setting::deserialize(*ptr, obj.diseaseName) ? 1 : 0;
        if ((ptr = value.find("diseaseCount")) != nullptr)
            count += Setting::deserialize(*ptr, obj.diseaseCount) ? 1 : 0;
        if ((ptr = value.find("location_x")) != nullptr)
            count += Setting::deserialize(*ptr, obj.location_x) ? 1 : 0;
        if ((ptr = value.find("location_y")) != nullptr)
            count += Setting::deserialize(*ptr, obj.location_y) ? 1 : 0;
        if ((ptr = value.find("preventFoWReveal")) != nullptr)
            count += Setting::deserialize(*ptr, obj.preventFoWReveal) ? 1 : 0;

        if (value.size() != count) {
            printf("object Cell parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<Rottable>
{
    static bool deserialize(const Json::Value &value, Rottable &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("rotAmount")) != nullptr)
            count += Setting::deserialize(*ptr, obj.rotAmount) ? 1 : 0;

        if (value.size() != count) {
            printf("object Rottable parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<StorageItem>
{
    static bool deserialize(const Json::Value &value, StorageItem &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("id")) != nullptr)
            count += Setting::deserialize(*ptr, obj.id) ? 1 : 0;
        if ((ptr = value.find("element")) != nullptr)
            count += Setting::deserialize(*ptr, obj.element) ? 1 : 0;
        if ((ptr = value.find("units")) != nullptr)
            count += Setting::deserialize(*ptr, obj.units) ? 1 : 0;
        if ((ptr = value.find("isOre")) != nullptr)
            count += Setting::deserialize(*ptr, obj.isOre) ? 1 : 0;
        if ((ptr = value.find("temperature")) != nullptr)
            count += Setting::deserialize(*ptr, obj.temperature) ? 1 : 0;
        if ((ptr = value.find("diseaseName")) != nullptr)
            count += Setting::deserialize(*ptr, obj.diseaseName) ? 1 : 0;
        if ((ptr = value.find("diseaseCount")) != nullptr)
            count += Setting::deserialize(*ptr, obj.diseaseCount) ? 1 : 0;
        if ((ptr = value.find("rottable")) != nullptr)
            count += Setting::deserialize(*ptr, obj.rottable) ? 1 : 0;

        if (value.size() != count) {
            printf("object StorageItem parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<TemplateAmountValue>
{
    static bool deserialize(const Json::Value &value, TemplateAmountValue &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("id")) != nullptr)
            count += Setting::deserialize(*ptr, obj.id) ? 1 : 0;
        if ((ptr = value.find("value")) != nullptr)
            count += Setting::deserialize(*ptr, obj.value) ? 1 : 0;

        if (value.size() != count) {
            printf("object TemplateAmountValue parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<Prefab>
{
    static bool deserialize(const Json::Value &value, Prefab &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("id")) != nullptr)
            count += Setting::deserialize(*ptr, obj.id) ? 1 : 0;
        if ((ptr = value.find("location_x")) != nullptr)
            count += Setting::deserialize(*ptr, obj.location_x) ? 1 : 0;
        if ((ptr = value.find("location_y")) != nullptr)
            count += Setting::deserialize(*ptr, obj.location_y) ? 1 : 0;
        if ((ptr = value.find("element")) != nullptr)
            count += Setting::deserialize(*ptr, obj.element) ? 1 : 0;
        if ((ptr = value.find("temperature")) != nullptr)
            count += Setting::deserialize(*ptr, obj.temperature) ? 1 : 0;
        if ((ptr = value.find("units")) != nullptr)
            count += Setting::deserialize(*ptr, obj.units) ? 1 : 0;
        if ((ptr = value.find("diseaseName")) != nullptr)
            count += Setting::deserialize(*ptr, obj.diseaseName) ? 1 : 0;
        if ((ptr = value.find("diseaseCount")) != nullptr)
            count += Setting::deserialize(*ptr, obj.diseaseCount) ? 1 : 0;
        if ((ptr = value.find("rotationOrientation")) != nullptr)
            count += Setting::deserialize(*ptr, obj.rotationOrientation) ? 1 : 0;
        if ((ptr = value.find("storage")) != nullptr)
            count += Setting::deserialize(*ptr, obj.storage) ? 1 : 0;
        if ((ptr = value.find("type")) != nullptr)
            count += Setting::deserialize(*ptr, obj.type) ? 1 : 0;
        if ((ptr = value.find("facadeId")) != nullptr)
            count += Setting::deserialize(*ptr, obj.facadeId) ? 1 : 0;
        if ((ptr = value.find("connections")) != nullptr)
            count += Setting::deserialize(*ptr, obj.connections) ? 1 : 0;
        if ((ptr = value.find("rottable")) != nullptr)
            count += Setting::deserialize(*ptr, obj.rottable) ? 1 : 0;
        if ((ptr = value.find("amounts")) != nullptr)
            count += Setting::deserialize(*ptr, obj.amounts) ? 1 : 0;
        if ((ptr = value.find("other_values")) != nullptr)
            count += Setting::deserialize(*ptr, obj.other_values) ? 1 : 0;

        if (value.size() != count) {
            printf("object Prefab parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<TemplateInfo>
{
    static bool deserialize(const Json::Value &value, TemplateInfo &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("size")) != nullptr)
            count += Setting::deserialize(*ptr, obj.size) ? 1 : 0;
        if ((ptr = value.find("min")) != nullptr)
            count += Setting::deserialize(*ptr, obj.min) ? 1 : 0;
        if ((ptr = value.find("area")) != nullptr)
            count += Setting::deserialize(*ptr, obj.area) ? 1 : 0;
        if ((ptr = value.find("discover_tags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.discover_tags) ? 1 : 0;

        if (value.size() != count) {
            printf("object TemplateInfo parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<TemplateContainer>
{
    static bool deserialize(const Json::Value &value, TemplateContainer &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("name")) != nullptr)
            count += Setting::deserialize(*ptr, obj.name) ? 1 : 0;
        if ((ptr = value.find("info")) != nullptr)
            count += Setting::deserialize(*ptr, obj.info) ? 1 : 0;
        if ((ptr = value.find("cells")) != nullptr)
            count += Setting::deserialize(*ptr, obj.cells) ? 1 : 0;
        if ((ptr = value.find("buildings")) != nullptr)
            count += Setting::deserialize(*ptr, obj.buildings) ? 1 : 0;
        if ((ptr = value.find("pickupables")) != nullptr)
            count += Setting::deserialize(*ptr, obj.pickupables) ? 1 : 0;
        if ((ptr = value.find("elementalOres")) != nullptr)
            count += Setting::deserialize(*ptr, obj.elementalOres) ? 1 : 0;
        if ((ptr = value.find("otherEntities")) != nullptr)
            count += Setting::deserialize(*ptr, obj.otherEntities) ? 1 : 0;

        if (value.size() != count) {
            printf("object TemplateContainer parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<SubWorld>
{
    static bool deserialize(const Json::Value &value, SubWorld &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("name")) != nullptr)
            count += Setting::deserialize(*ptr, obj.name) ? 1 : 0;
        if ((ptr = value.find("selectMethod")) != nullptr)
            count += Setting::deserialize(*ptr, obj.selectMethod) ? 1 : 0;
        if ((ptr = value.find("density")) != nullptr)
            count += Setting::deserialize(*ptr, obj.density) ? 1 : 0;
        if ((ptr = value.find("avoidRadius")) != nullptr)
            count += Setting::deserialize(*ptr, obj.avoidRadius) ? 1 : 0;
        if ((ptr = value.find("sampleBehaviour")) != nullptr)
            count += Setting::deserialize(*ptr, obj.sampleBehaviour) ? 1 : 0;
        if ((ptr = value.find("doAvoidPoints")) != nullptr)
            count += Setting::deserialize(*ptr, obj.doAvoidPoints) ? 1 : 0;
        if ((ptr = value.find("dontRelaxChildren")) != nullptr)
            count += Setting::deserialize(*ptr, obj.dontRelaxChildren) ? 1 : 0;
        if ((ptr = value.find("blobSize")) != nullptr)
            count += Setting::deserialize(*ptr, obj.blobSize) ? 1 : 0;
        if ((ptr = value.find("nameKey")) != nullptr)
            count += Setting::deserialize(*ptr, obj.nameKey) ? 1 : 0;
        if ((ptr = value.find("descriptionKey")) != nullptr)
            count += Setting::deserialize(*ptr, obj.descriptionKey) ? 1 : 0;
        if ((ptr = value.find("utilityKey")) != nullptr)
            count += Setting::deserialize(*ptr, obj.utilityKey) ? 1 : 0;
        if ((ptr = value.find("biomeNoise")) != nullptr)
            count += Setting::deserialize(*ptr, obj.biomeNoise) ? 1 : 0;
        if ((ptr = value.find("overrideNoise")) != nullptr)
            count += Setting::deserialize(*ptr, obj.overrideNoise) ? 1 : 0;
        if ((ptr = value.find("densityNoise")) != nullptr)
            count += Setting::deserialize(*ptr, obj.densityNoise) ? 1 : 0;
        if ((ptr = value.find("borderOverride")) != nullptr)
            count += Setting::deserialize(*ptr, obj.borderOverride) ? 1 : 0;
        if ((ptr = value.find("borderOverridePriority")) != nullptr)
            count += Setting::deserialize(*ptr, obj.borderOverridePriority) ? 1 : 0;
        if ((ptr = value.find("borderSizeOverride")) != nullptr)
            count += Setting::deserialize(*ptr, obj.borderSizeOverride) ? 1 : 0;
        if ((ptr = value.find("temperatureRange")) != nullptr)
            count += Setting::deserialize(*ptr, obj.temperatureRange) ? 1 : 0;
        if ((ptr = value.find("centralFeature")) != nullptr)
            count += Setting::deserialize(*ptr, obj.centralFeature) ? 1 : 0;
        if ((ptr = value.find("features")) != nullptr)
            count += Setting::deserialize(*ptr, obj.features) ? 1 : 0;
        if ((ptr = value.find("overrides")) != nullptr)
            count += Setting::deserialize(*ptr, obj.overrides) ? 1 : 0;
        if ((ptr = value.find("tags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.tags) ? 1 : 0;
        if ((ptr = value.find("minChildCount")) != nullptr)
            count += Setting::deserialize(*ptr, obj.minChildCount) ? 1 : 0;
        if ((ptr = value.find("singleChildCount")) != nullptr)
            count += Setting::deserialize(*ptr, obj.singleChildCount) ? 1 : 0;
        if ((ptr = value.find("extraBiomeChildren")) != nullptr)
            count += Setting::deserialize(*ptr, obj.extraBiomeChildren) ? 1 : 0;
        if ((ptr = value.find("biomes")) != nullptr)
            count += Setting::deserialize(*ptr, obj.biomes) ? 1 : 0;
        if ((ptr = value.find("featureTemplates")) != nullptr)
            count += Setting::deserialize(*ptr, obj.featureTemplates) ? 1 : 0;
        if ((ptr = value.find("subworldTemplateRules")) != nullptr)
            count += Setting::deserialize(*ptr, obj.subworldTemplateRules) ? 1 : 0;
        if ((ptr = value.find("iterations")) != nullptr)
            count += Setting::deserialize(*ptr, obj.iterations) ? 1 : 0;
        if ((ptr = value.find("minEnergy")) != nullptr)
            count += Setting::deserialize(*ptr, obj.minEnergy) ? 1 : 0;
        if ((ptr = value.find("zoneType")) != nullptr)
            count += Setting::deserialize(*ptr, obj.zoneType) ? 1 : 0;
        if ((ptr = value.find("samplers")) != nullptr)
            count += Setting::deserialize(*ptr, obj.samplers) ? 1 : 0;
        if ((ptr = value.find("pdWeight")) != nullptr)
            count += Setting::deserialize(*ptr, obj.pdWeight) ? 1 : 0;

        if (value.size() != count) {
            printf("object SubWorld parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<WorldTrait>
{
    static bool deserialize(const Json::Value &value, WorldTrait &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("filePath")) != nullptr)
            count += Setting::deserialize(*ptr, obj.filePath) ? 1 : 0;
        if ((ptr = value.find("name")) != nullptr)
            count += Setting::deserialize(*ptr, obj.name) ? 1 : 0;
        if ((ptr = value.find("description")) != nullptr)
            count += Setting::deserialize(*ptr, obj.description) ? 1 : 0;
        if ((ptr = value.find("colorHex")) != nullptr)
            count += Setting::deserialize(*ptr, obj.colorHex) ? 1 : 0;
        if ((ptr = value.find("icon")) != nullptr)
            count += Setting::deserialize(*ptr, obj.icon) ? 1 : 0;
        if ((ptr = value.find("forbiddenDLCIds")) != nullptr)
            count += Setting::deserialize(*ptr, obj.forbiddenDLCIds) ? 1 : 0;
        if ((ptr = value.find("exclusiveWith")) != nullptr)
            count += Setting::deserialize(*ptr, obj.exclusiveWith) ? 1 : 0;
        if ((ptr = value.find("exclusiveWithTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.exclusiveWithTags) ? 1 : 0;
        if ((ptr = value.find("traitTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.traitTags) ? 1 : 0;
        if ((ptr = value.find("startingBasePositionHorizontalMod")) != nullptr)
            count += Setting::deserialize(*ptr, obj.startingBasePositionHorizontalMod) ? 1 : 0;
        if ((ptr = value.find("startingBasePositionVerticalMod")) != nullptr)
            count += Setting::deserialize(*ptr, obj.startingBasePositionVerticalMod) ? 1 : 0;
        if ((ptr = value.find("additionalSubworldFiles")) != nullptr)
            count += Setting::deserialize(*ptr, obj.additionalSubworldFiles) ? 1 : 0;
        if ((ptr = value.find("additionalUnknownCellFilters")) != nullptr)
            count += Setting::deserialize(*ptr, obj.additionalUnknownCellFilters) ? 1 : 0;
        if ((ptr = value.find("additionalWorldTemplateRules")) != nullptr)
            count += Setting::deserialize(*ptr, obj.additionalWorldTemplateRules) ? 1 : 0;
        if ((ptr = value.find("globalFeatureMods")) != nullptr)
            count += Setting::deserialize(*ptr, obj.globalFeatureMods) ? 1 : 0;
        if ((ptr = value.find("removeWorldTemplateRulesById")) != nullptr)
            count += Setting::deserialize(*ptr, obj.removeWorldTemplateRulesById) ? 1 : 0;
        if ((ptr = value.find("elementBandModifiers")) != nullptr)
            count += Setting::deserialize(*ptr, obj.elementBandModifiers) ? 1 : 0;

        if (value.size() != count) {
            printf("object WorldTrait parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<World>
{
    static bool deserialize(const Json::Value &value, World &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("name")) != nullptr)
            count += Setting::deserialize(*ptr, obj.name) ? 1 : 0;
        if ((ptr = value.find("description")) != nullptr)
            count += Setting::deserialize(*ptr, obj.description) ? 1 : 0;
        if ((ptr = value.find("nameTables")) != nullptr)
            count += Setting::deserialize(*ptr, obj.nameTables) ? 1 : 0;
        if ((ptr = value.find("overrideName")) != nullptr)
            count += Setting::deserialize(*ptr, obj.overrideName) ? 1 : 0;
        if ((ptr = value.find("asteroidIcon")) != nullptr)
            count += Setting::deserialize(*ptr, obj.asteroidIcon) ? 1 : 0;
        if ((ptr = value.find("iconScale")) != nullptr)
            count += Setting::deserialize(*ptr, obj.iconScale) ? 1 : 0;
        if ((ptr = value.find("worldTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.worldTags) ? 1 : 0;
        if ((ptr = value.find("dlcIdFrom")) != nullptr)
            count += Setting::deserialize(*ptr, obj.dlcIdFrom) ? 1 : 0;
        if ((ptr = value.find("requiredDlcIds")) != nullptr)
            count += Setting::deserialize(*ptr, obj.requiredDlcIds) ? 1 : 0;
        if ((ptr = value.find("forbiddenDlcIds")) != nullptr)
            count += Setting::deserialize(*ptr, obj.forbiddenDlcIds) ? 1 : 0;
        if ((ptr = value.find("disableWorldTraits")) != nullptr)
            count += Setting::deserialize(*ptr, obj.disableWorldTraits) ? 1 : 0;
        if ((ptr = value.find("worldTraitRules")) != nullptr)
            count += Setting::deserialize(*ptr, obj.worldTraitRules) ? 1 : 0;
        if ((ptr = value.find("worldTraitScale")) != nullptr)
            count += Setting::deserialize(*ptr, obj.worldTraitScale) ? 1 : 0;
        if ((ptr = value.find("skip")) != nullptr)
            count += Setting::deserialize(*ptr, obj.skip) ? 1 : 0;
        if ((ptr = value.find("moduleInterior")) != nullptr)
            count += Setting::deserialize(*ptr, obj.moduleInterior) ? 1 : 0;
        if ((ptr = value.find("category")) != nullptr)
            count += Setting::deserialize(*ptr, obj.category) ? 1 : 0;
        if ((ptr = value.find("worldsize")) != nullptr)
            count += Setting::deserialize(*ptr, obj.worldsize) ? 1 : 0;
        if ((ptr = value.find("defaultsOverrides")) != nullptr)
            count += Setting::deserialize(*ptr, obj.defaultsOverrides) ? 1 : 0;
        if ((ptr = value.find("layoutMethod")) != nullptr)
            count += Setting::deserialize(*ptr, obj.layoutMethod) ? 1 : 0;
        if ((ptr = value.find("subworldFiles")) != nullptr)
            count += Setting::deserialize(*ptr, obj.subworldFiles) ? 1 : 0;
        if ((ptr = value.find("unknownCellsAllowedSubworlds")) != nullptr)
            count += Setting::deserialize(*ptr, obj.unknownCellsAllowedSubworlds) ? 1 : 0;
        if ((ptr = value.find("subworldMixingRules")) != nullptr)
            count += Setting::deserialize(*ptr, obj.subworldMixingRules) ? 1 : 0;
        if ((ptr = value.find("startSubworldName")) != nullptr)
            count += Setting::deserialize(*ptr, obj.startSubworldName) ? 1 : 0;
        if ((ptr = value.find("startingBaseTemplate")) != nullptr)
            count += Setting::deserialize(*ptr, obj.startingBaseTemplate) ? 1 : 0;
        if ((ptr = value.find("startingBasePositionHorizontal")) != nullptr)
            count += Setting::deserialize(*ptr, obj.startingBasePositionHorizontal) ? 1 : 0;
        if ((ptr = value.find("startingBasePositionVertical")) != nullptr)
            count += Setting::deserialize(*ptr, obj.startingBasePositionVertical) ? 1 : 0;
        if ((ptr = value.find("worldTemplateRules")) != nullptr)
            count += Setting::deserialize(*ptr, obj.worldTemplateRules) ? 1 : 0;
        if ((ptr = value.find("seasons")) != nullptr)
            count += Setting::deserialize(*ptr, obj.seasons) ? 1 : 0;
        if ((ptr = value.find("fixedTraits")) != nullptr)
            count += Setting::deserialize(*ptr, obj.fixedTraits) ? 1 : 0;
        if ((ptr = value.find("adjacentTemporalTear")) != nullptr)
            count += Setting::deserialize(*ptr, obj.adjacentTemporalTear) ? 1 : 0;

        if (value.size() != count) {
            printf("object World parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<Feature>
{
    static bool deserialize(const Json::Value &value, Feature &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("type")) != nullptr)
            count += Setting::deserialize(*ptr, obj.type) ? 1 : 0;
        if ((ptr = value.find("tags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.tags) ? 1 : 0;
        if ((ptr = value.find("excludesTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.excludesTags) ? 1 : 0;

        if (value.size() != count) {
            printf("object Feature parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<WeightedBiome>
{
    static bool deserialize(const Json::Value &value, WeightedBiome &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("name")) != nullptr)
            count += Setting::deserialize(*ptr, obj.name) ? 1 : 0;
        if ((ptr = value.find("weight")) != nullptr)
            count += Setting::deserialize(*ptr, obj.weight) ? 1 : 0;
        if ((ptr = value.find("tags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.tags) ? 1 : 0;

        if (value.size() != count) {
            printf("object WeightedBiome parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<AllowedCellsFilter>
{
    static bool deserialize(const Json::Value &value, AllowedCellsFilter &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("tagcommand")) != nullptr)
            count += Setting::deserialize(*ptr, obj.tagcommand) ? 1 : 0;
        if ((ptr = value.find("tag")) != nullptr)
            count += Setting::deserialize(*ptr, obj.tag) ? 1 : 0;
        if ((ptr = value.find("minDistance")) != nullptr)
            count += Setting::deserialize(*ptr, obj.minDistance) ? 1 : 0;
        if ((ptr = value.find("maxDistance")) != nullptr)
            count += Setting::deserialize(*ptr, obj.maxDistance) ? 1 : 0;
        if ((ptr = value.find("command")) != nullptr)
            count += Setting::deserialize(*ptr, obj.command) ? 1 : 0;
        if ((ptr = value.find("temperatureRanges")) != nullptr)
            count += Setting::deserialize(*ptr, obj.temperatureRanges) ? 1 : 0;
        if ((ptr = value.find("zoneTypes")) != nullptr)
            count += Setting::deserialize(*ptr, obj.zoneTypes) ? 1 : 0;
        if ((ptr = value.find("subworldNames")) != nullptr)
            count += Setting::deserialize(*ptr, obj.subworldNames) ? 1 : 0;
        if ((ptr = value.find("sortOrder")) != nullptr)
            count += Setting::deserialize(*ptr, obj.sortOrder) ? 1 : 0;
        if ((ptr = value.find("ignoreIfMissingTag")) != nullptr)
            count += Setting::deserialize(*ptr, obj.ignoreIfMissingTag) ? 1 : 0;

        if (value.size() != count) {
            printf("object AllowedCellsFilter parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<TemplateSpawnRules>
{
    static bool deserialize(const Json::Value &value, TemplateSpawnRules &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("ruleId")) != nullptr)
            count += Setting::deserialize(*ptr, obj.ruleId) ? 1 : 0;
        if ((ptr = value.find("names")) != nullptr)
            count += Setting::deserialize(*ptr, obj.names) ? 1 : 0;
        if ((ptr = value.find("listRule")) != nullptr)
            count += Setting::deserialize(*ptr, obj.listRule) ? 1 : 0;
        if ((ptr = value.find("someCount")) != nullptr)
            count += Setting::deserialize(*ptr, obj.someCount) ? 1 : 0;
        if ((ptr = value.find("moreCount")) != nullptr)
            count += Setting::deserialize(*ptr, obj.moreCount) ? 1 : 0;
        if ((ptr = value.find("range")) != nullptr)
            count += Setting::deserialize(*ptr, obj.range) ? 1 : 0;
        if ((ptr = value.find("times")) != nullptr)
            count += Setting::deserialize(*ptr, obj.times) ? 1 : 0;
        if ((ptr = value.find("priority")) != nullptr)
            count += Setting::deserialize(*ptr, obj.priority) ? 1 : 0;
        if ((ptr = value.find("allowDuplicates")) != nullptr)
            count += Setting::deserialize(*ptr, obj.allowDuplicates) ? 1 : 0;
        if ((ptr = value.find("allowExtremeTemperatureOverlap")) != nullptr)
            count += Setting::deserialize(*ptr, obj.allowExtremeTemperatureOverlap) ? 1 : 0;
        if ((ptr = value.find("allowNearStart")) != nullptr)
            count += Setting::deserialize(*ptr, obj.allowNearStart) ? 1 : 0;
        if ((ptr = value.find("useRelaxedFiltering")) != nullptr)
            count += Setting::deserialize(*ptr, obj.useRelaxedFiltering) ? 1 : 0;
        if ((ptr = value.find("overrideOffset")) != nullptr)
            count += Setting::deserialize(*ptr, obj.overrideOffset) ? 1 : 0;
        if ((ptr = value.find("overridePlacement")) != nullptr)
            count += Setting::deserialize(*ptr, obj.overridePlacement) ? 1 : 0;
        if ((ptr = value.find("allowedCellsFilter")) != nullptr)
            count += Setting::deserialize(*ptr, obj.allowedCellsFilter) ? 1 : 0;

        if (value.size() != count) {
            printf("object TemplateSpawnRules parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<WeightedSubworldName>
{
    static bool deserialize(const Json::Value &value, WeightedSubworldName &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("name")) != nullptr)
            count += Setting::deserialize(*ptr, obj.name) ? 1 : 0;
        if ((ptr = value.find("overrideName")) != nullptr)
            count += Setting::deserialize(*ptr, obj.overrideName) ? 1 : 0;
        if ((ptr = value.find("overridePower")) != nullptr)
            count += Setting::deserialize(*ptr, obj.overridePower) ? 1 : 0;
        if ((ptr = value.find("weight")) != nullptr)
            count += Setting::deserialize(*ptr, obj.weight) ? 1 : 0;
        if ((ptr = value.find("minCount")) != nullptr)
            count += Setting::deserialize(*ptr, obj.minCount) ? 1 : 0;
        if ((ptr = value.find("maxCount")) != nullptr)
            count += Setting::deserialize(*ptr, obj.maxCount) ? 1 : 0;
        if ((ptr = value.find("priority")) != nullptr)
            count += Setting::deserialize(*ptr, obj.priority) ? 1 : 0;

        if (value.size() != count) {
            printf("object WeightedSubworldName parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<WorldMixing>
{
    static bool deserialize(const Json::Value &value, WorldMixing &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("requiredTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.requiredTags) ? 1 : 0;
        if ((ptr = value.find("forbiddenTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.forbiddenTags) ? 1 : 0;
        if ((ptr = value.find("additionalWorldTemplateRules")) != nullptr)
            count += Setting::deserialize(*ptr, obj.additionalWorldTemplateRules) ? 1 : 0;
        if ((ptr = value.find("additionalUnknownCellFilters")) != nullptr)
            count += Setting::deserialize(*ptr, obj.additionalUnknownCellFilters) ? 1 : 0;
        if ((ptr = value.find("additionalSubworldFiles")) != nullptr)
            count += Setting::deserialize(*ptr, obj.additionalSubworldFiles) ? 1 : 0;
        if ((ptr = value.find("additionalSeasons")) != nullptr)
            count += Setting::deserialize(*ptr, obj.additionalSeasons) ? 1 : 0;

        if (value.size() != count) {
            printf("object WorldMixing parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<WorldMixingSettings>
{
    static bool deserialize(const Json::Value &value, WorldMixingSettings &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("name")) != nullptr)
            count += Setting::deserialize(*ptr, obj.name) ? 1 : 0;
        if ((ptr = value.find("description")) != nullptr)
            count += Setting::deserialize(*ptr, obj.description) ? 1 : 0;
        if ((ptr = value.find("icon")) != nullptr)
            count += Setting::deserialize(*ptr, obj.icon) ? 1 : 0;
        if ((ptr = value.find("forbiddenClusterTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.forbiddenClusterTags) ? 1 : 0;
        if ((ptr = value.find("world")) != nullptr)
            count += Setting::deserialize(*ptr, obj.world) ? 1 : 0;

        if (value.size() != count) {
            printf("object WorldMixingSettings parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<TraitRule>
{
    static bool deserialize(const Json::Value &value, TraitRule &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("min")) != nullptr)
            count += Setting::deserialize(*ptr, obj.min) ? 1 : 0;
        if ((ptr = value.find("max")) != nullptr)
            count += Setting::deserialize(*ptr, obj.max) ? 1 : 0;
        if ((ptr = value.find("requiredTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.requiredTags) ? 1 : 0;
        if ((ptr = value.find("specificTraits")) != nullptr)
            count += Setting::deserialize(*ptr, obj.specificTraits) ? 1 : 0;
        if ((ptr = value.find("forbiddenTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.forbiddenTags) ? 1 : 0;
        if ((ptr = value.find("forbiddenTraits")) != nullptr)
            count += Setting::deserialize(*ptr, obj.forbiddenTraits) ? 1 : 0;

        if (value.size() != count) {
            printf("object TraitRule parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<SubworldMixingRule>
{
    static bool deserialize(const Json::Value &value, SubworldMixingRule &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("name")) != nullptr)
            count += Setting::deserialize(*ptr, obj.name) ? 1 : 0;
        if ((ptr = value.find("minCount")) != nullptr)
            count += Setting::deserialize(*ptr, obj.minCount) ? 1 : 0;
        if ((ptr = value.find("maxCount")) != nullptr)
            count += Setting::deserialize(*ptr, obj.maxCount) ? 1 : 0;
        if ((ptr = value.find("forbiddenTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.forbiddenTags) ? 1 : 0;
        if ((ptr = value.find("requiredTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.requiredTags) ? 1 : 0;

        if (value.size() != count) {
            printf("object SubworldMixingRule parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<ElementBandModifier>
{
    static bool deserialize(const Json::Value &value, ElementBandModifier &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("element")) != nullptr)
            count += Setting::deserialize(*ptr, obj.element) ? 1 : 0;
        if ((ptr = value.find("massMultiplier")) != nullptr)
            count += Setting::deserialize(*ptr, obj.massMultiplier) ? 1 : 0;
        if ((ptr = value.find("bandMultiplier")) != nullptr)
            count += Setting::deserialize(*ptr, obj.bandMultiplier) ? 1 : 0;

        if (value.size() != count) {
            printf("object ElementBandModifier parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<MobReference>
{
    static bool deserialize(const Json::Value &value, MobReference &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("type")) != nullptr)
            count += Setting::deserialize(*ptr, obj.type) ? 1 : 0;
        if ((ptr = value.find("count")) != nullptr)
            count += Setting::deserialize(*ptr, obj.count) ? 1 : 0;

        if (value.size() != count) {
            printf("object MobReference parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<ElementChoiceGroup>
{
    static bool deserialize(const Json::Value &value, ElementChoiceGroup &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("selectionMethod")) != nullptr)
            count += Setting::deserialize(*ptr, obj.selectionMethod) ? 1 : 0;
        if ((ptr = value.find("choices")) != nullptr)
            count += Setting::deserialize(*ptr, obj.choices) ? 1 : 0;

        if (value.size() != count) {
            printf("object ElementChoiceGroup parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<FeatureSettings>
{
    static bool deserialize(const Json::Value &value, FeatureSettings &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("shape")) != nullptr)
            count += Setting::deserialize(*ptr, obj.shape) ? 1 : 0;
        if ((ptr = value.find("borders")) != nullptr)
            count += Setting::deserialize(*ptr, obj.borders) ? 1 : 0;
        if ((ptr = value.find("blobSize")) != nullptr)
            count += Setting::deserialize(*ptr, obj.blobSize) ? 1 : 0;
        if ((ptr = value.find("forceBiome")) != nullptr)
            count += Setting::deserialize(*ptr, obj.forceBiome) ? 1 : 0;
        if ((ptr = value.find("biomeTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.biomeTags) ? 1 : 0;
        if ((ptr = value.find("internalMobs")) != nullptr)
            count += Setting::deserialize(*ptr, obj.internalMobs) ? 1 : 0;
        if ((ptr = value.find("tags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.tags) ? 1 : 0;
        if ((ptr = value.find("ElementChoiceGroups")) != nullptr)
            count += Setting::deserialize(*ptr, obj.ElementChoiceGroups) ? 1 : 0;
        if ((ptr = value.find("excludeTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.excludeTags) ? 1 : 0;

        if (value.size() != count) {
            printf("object FeatureSettings parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<ElementGradient>
{
    static bool deserialize(const Json::Value &value, ElementGradient &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("content")) != nullptr)
            count += Setting::deserialize(*ptr, obj.content) ? 1 : 0;
        if ((ptr = value.find("bandSize")) != nullptr)
            count += Setting::deserialize(*ptr, obj.bandSize) ? 1 : 0;
        if ((ptr = value.find("maxValue")) != nullptr)
            count += Setting::deserialize(*ptr, obj.maxValue) ? 1 : 0;
        if ((ptr = value.find("overrides")) != nullptr)
            count += Setting::deserialize(*ptr, obj.overrides) ? 1 : 0;

        if (value.size() != count) {
            printf("object ElementGradient parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<BiomeSettings>
{
    static bool deserialize(const Json::Value &value, BiomeSettings &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("TerrainBiomeLookupTable")) != nullptr)
            count += Setting::deserialize(*ptr, obj.TerrainBiomeLookupTable) ? 1 : 0;

        if (value.size() != count) {
            printf("object BiomeSettings parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<SubworldMixingSettings>
{
    static bool deserialize(const Json::Value &value, SubworldMixingSettings &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("name")) != nullptr)
            count += Setting::deserialize(*ptr, obj.name) ? 1 : 0;
        if ((ptr = value.find("description")) != nullptr)
            count += Setting::deserialize(*ptr, obj.description) ? 1 : 0;
        if ((ptr = value.find("icon")) != nullptr)
            count += Setting::deserialize(*ptr, obj.icon) ? 1 : 0;
        if ((ptr = value.find("forbiddenClusterTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.forbiddenClusterTags) ? 1 : 0;
        if ((ptr = value.find("subworld")) != nullptr)
            count += Setting::deserialize(*ptr, obj.subworld) ? 1 : 0;
        if ((ptr = value.find("mixingTags")) != nullptr)
            count += Setting::deserialize(*ptr, obj.mixingTags) ? 1 : 0;
        if ((ptr = value.find("additionalWorldTemplateRules")) != nullptr)
            count += Setting::deserialize(*ptr, obj.additionalWorldTemplateRules) ? 1 : 0;

        if (value.size() != count) {
            printf("object SubworldMixingSettings parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<SpaceMapPOIPlacement>
{
    static bool deserialize(const Json::Value &value, SpaceMapPOIPlacement &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("pois")) != nullptr)
            count += Setting::deserialize(*ptr, obj.pois) ? 1 : 0;
        if ((ptr = value.find("numToSpawn")) != nullptr)
            count += Setting::deserialize(*ptr, obj.numToSpawn) ? 1 : 0;
        if ((ptr = value.find("allowedRings")) != nullptr)
            count += Setting::deserialize(*ptr, obj.allowedRings) ? 1 : 0;
        if ((ptr = value.find("avoidClumping")) != nullptr)
            count += Setting::deserialize(*ptr, obj.avoidClumping) ? 1 : 0;
        if ((ptr = value.find("canSpawnDuplicates")) != nullptr)
            count += Setting::deserialize(*ptr, obj.canSpawnDuplicates) ? 1 : 0;
        if ((ptr = value.find("guarantee")) != nullptr)
            count += Setting::deserialize(*ptr, obj.guarantee) ? 1 : 0;

        if (value.size() != count) {
            printf("object SpaceMapPOIPlacement parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<LoreCollectionOverride>
{
    static bool deserialize(const Json::Value &value, LoreCollectionOverride &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("id")) != nullptr)
            count += Setting::deserialize(*ptr, obj.id) ? 1 : 0;
        if ((ptr = value.find("collection")) != nullptr)
            count += Setting::deserialize(*ptr, obj.collection) ? 1 : 0;
        if ((ptr = value.find("orderRule")) != nullptr)
            count += Setting::deserialize(*ptr, obj.orderRule) ? 1 : 0;

        if (value.size() != count) {
            printf("object LoreCollectionOverride parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<SpaceDestinationMix>
{
    static bool deserialize(const Json::Value &value, SpaceDestinationMix &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("minTier")) != nullptr)
            count += Setting::deserialize(*ptr, obj.minTier) ? 1 : 0;
        if ((ptr = value.find("maxTier")) != nullptr)
            count += Setting::deserialize(*ptr, obj.maxTier) ? 1 : 0;
        if ((ptr = value.find("type")) != nullptr)
            count += Setting::deserialize(*ptr, obj.type) ? 1 : 0;

        if (value.size() != count) {
            printf("object SpaceDestinationMix parse failed.\n");
            return false;
        }
        return true;
    }
};

template<>
struct Deserializer<DlcMixingSetting>
{
    static bool deserialize(const Json::Value &value, DlcMixingSetting &obj)
    {
        const Json::Value *ptr = nullptr;
        int count = 0;
        if ((ptr = value.find("spaceDesinations")) != nullptr)
            count += Setting::deserialize(*ptr, obj.spaceDesinations) ? 1 : 0;
        if ((ptr = value.find("spacePois")) != nullptr)
            count += Setting::deserialize(*ptr, obj.spacePois) ? 1 : 0;
        if ((ptr = value.find("globalLoreUnlocks")) != nullptr)
            count += Setting::deserialize(*ptr, obj.globalLoreUnlocks) ? 1 : 0;

        if (value.size() != count) {
            printf("object DlcMixingSetting parse failed.\n");
            return false;
        }
        return true;
    }
};
