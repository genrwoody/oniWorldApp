interface Description {
    type:number;
    key?:string;
    name:string;
}

interface Configuration {
    languages: Array<Description>;
    game: Array<Description>;
    cluster: Array<Description>;
    mixing: Array<Description>;
    traits: Array<Description>;
    geyser: Array<Description>;
}

export const configuration: Configuration = {
    languages: [
        { type: 0, key: "en", name: "English" },
        { type: 1, key: "zh-CN", name: "简体中文" },
    ],
    game: [
        { type: 0, name: "Base Game" },
        { type: 1, name: "Classic" },
        { type: 2, name: "Spaced Out!" },
    ],
    cluster: [
        { type: 0, key: "SNDST-A", name: "Terra" },
        { type: 0, key: "OCAN-A", name: "Oceania" },
        { type: 0, key: "S-FRZ", name: "Rime" },
        { type: 0, key: "LUSH-A", name: "Verdante" },
        { type: 0, key: "FRST-A", name: "Arboria" },
        { type: 0, key: "VOLCA", name: "Volcanea" },
        { type: 0, key: "BAD-A", name: "The Badlands" },
        { type: 0, key: "HTFST-A", name: "Aridio" },
        { type: 0, key: "OASIS-A", name: "Oasisse" },
        { type: 0, key: "CER-A", name: "Ceres" },
        { type: 0, key: "CERS-A", name: "Blasted Ceres" },
        { type: 0, key: "PRE-A", name: "Relica" },
        { type: 0, key: "PRES-A", name: "RelicAAAAGH" },
        { type: 1, key: "V-SNDST-C", name: "Terra Cluster" },
        { type: 1, key: "V-OCAN-C", name: "Oceania Cluster" },
        { type: 1, key: "V-SWMP-C", name: "Squelchy Cluster" },
        { type: 1, key: "V-SFRZ-C", name: "Rime Cluster" },
        { type: 1, key: "V-LUSH-C", name: "Verdante Cluster" },
        { type: 1, key: "V-FRST-C", name: "Arboria Cluster" },
        { type: 1, key: "V-VOLCA-C", name: "Volcanea Cluster" },
        { type: 1, key: "V-BAD-C", name: "The Badlands Cluster" },
        { type: 1, key: "V-HTFST-C", name: "Aridio Cluster" },
        { type: 1, key: "V-OASIS-C", name: "Oasisse Cluster" },
        { type: 1, key: "V-CER-C", name: "Ceres Cluster" },
        { type: 1, key: "V-CERS-C", name: "Blasted Ceres Cluster" },
        { type: 1, key: "V-PRE-C", name: "Relica Cluster" },
        { type: 1, key: "V-PRES-C", name: "RelicAAAAGH Cluster" },
        { type: 2, key: "SNDST-C", name: "Terrania Cluster" },
        { type: 2, key: "PRE-C", name: "Relica Minor Cluster" },
        { type: 2, key: "CER-C", name: "Ceres Minor Cluster" },
        { type: 2, key: "FRST-C", name: "Folia Cluster" },
        { type: 2, key: "SWMP-C", name: "Quagmiris Cluster" },
        { type: 2, key: "M-SWMP-C", name: "Moonlet Cluster - Metallic Swampy" },
        { type: 2, key: "M-BAD-C", name: "Moonlet Cluster - The Desolands" },
        { type: 2, key: "M-FRZ-C", name: "Moonlet Cluster - Frozen Forest" },
        { type: 2, key: "M-FLIP-C", name: "Moonlet Cluster - Flipped" },
        {
            type: 2,
            key: "M-RAD-C",
            name: "Moonlet Cluster - Radioactive Ocean",
        },
        { type: 2, key: "M-CERS-C", name: "Moonlet Cluster - Ceres Mantle" },
    ],
    mixing: [
        { type: 1, key: "mix8", name: "Relica Fragment" },
        { type: 1, key: "mix7", name: "Wetlands Biome" },
        { type: 1, key: "mix6", name: "Feather Biome" },
        { type: 1, key: "mix5", name: "Garden Biome" },
        { type: 0, key: "dlc4", name: "The Prehistoric Planet Pack" },
        { type: 0, key: "dlc3", name: "The Bionic Booster Pack" },
        { type: 1, key: "mix4", name: "Ceres Fragment" },
        { type: 1, key: "mix3", name: "Nectar Biome" },
        { type: 1, key: "mix2", name: "Cool Pool Biome" },
        { type: 1, key: "mix1", name: "Ice Cave Biome" },
        { type: 0, key: "dlc2", name: "The Frosty Planet Pack" },
    ],
    geyser: [
        { type: 1, name: "Cool Steam Vent" },
        { type: 0, name: "Steam Vent" },
        { type: 1, name: "Water Geyser" },
        { type: 1, name: "Cool Slush Geyser" },
        { type: 1, name: "Polluted Water Vent" },
        { type: 1, name: "Cool Salt Slush Geyser" },
        { type: 1, name: "Salt Water Geyser" },
        { type: 0, name: "Minor Volcano" },
        { type: 0, name: "Volcano" },
        { type: 2, name: "Carbon Dioxide Geyser" },
        { type: 2, name: "Carbon Dioxide Vent" },
        { type: 2, name: "Hydrogen Vent" },
        { type: 2, name: "Hot Polluted Oxygen Vent" },
        { type: 2, name: "InFectious Polluted Oxygen Vent" },
        { type: 2, name: "Chlorine Gas Vent" },
        { type: 0, name: "Natural Gas Geyser" },
        { type: 0, name: "Copper Volcano" },
        { type: 0, name: "Iron Volcano" },
        { type: 0, name: "Gold Volcano" },
        { type: 0, name: "Aluminum Volcano" },
        { type: 0, name: "Cobalt Volcano" },
        { type: 0, name: "Leaky Oil Fissure" },
        { type: 0, name: "Liquid Sulfur Geyser" },
        { type: 0, name: "Cool Chlorine Gas Vent" },
        { type: 0, name: "Tungsten Volcano" },
        { type: 0, name: "Niobium Volcano" },
        { type: 0, name: "Printing Pod" },
        { type: 0, name: "Oil Reservoir" },
        { type: 0, name: "Supply Teleporter Output" },
        { type: 0, name: "Supply Teleporter Input" },
        { type: 0, name: "Teleporter Transmitter" },
        { type: 0, name: "Cryotank 3000" },
    ],
    traits: [
        { type: 0, name: "No Traits" },
        { type: 0, name: "Crashed Satellites" },
        { type: 0, name: "Frozen Friend" },
        { type: 0, name: "Irregular Oil" },
        { type: 0, name: "Lush Core" },
        { type: 0, name: "Metallic Caves" },
        { type: 0, name: "Radioactive Crust" },
        { type: 0, name: "Subsurface Ocean" },
        { type: 0, name: "Volcanic Activity" },
        { type: 2, name: "Large Boulders" },
        { type: 0, name: "Medium Boulders" },
        { type: 2, name: "Mixed Boulders" },
        { type: 0, name: "Small Boulders" },
        { type: 0, name: "Trapped Oil" },
        { type: 0, name: "Frozen Core" },
        { type: 1, name: "Geoactive" },
        { type: 1, name: "Geodes" },
        { type: 2, name: "Geodormant" },
        { type: 0, name: "Large Claciers" },
        { type: 0, name: "Irregular Oil" },
        { type: 0, name: "Magma Channels" },
        { type: 2, name: "Metal Poor" },
        { type: 1, name: "Metal Rich" },
        { type: 0, name: "Alternate Pod Location" },
        { type: 0, name: "Slime Molds" },
        { type: 0, name: "Subsurface Ocean" },
        { type: 0, name: "Volcanic Activity" },
    ],
};
