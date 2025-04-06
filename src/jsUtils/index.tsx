import { configuration } from "./configuration";

const enum ResultType {
    RT_Starting,
    RT_Trait,
    RT_Geyser,
    RT_Polygon,
    RT_WorldSize,
    RT_Resource,
}

Module.updateWorld = (type: number, count: number, data: number) => {
    const world = Module.world;
    let offset = data >> 2;
    switch (type) {
        case ResultType.RT_Starting:
            world.starting.x = Module.HEAP32[offset++];
            world.starting.y = Module.HEAP32[offset++];
            break;
        case ResultType.RT_Trait:
            world.traits.length = 0;
            if (count === 0) {
                world.traits.push(0);
            } else {
                for (let i = 0; i < count; i++) {
                    world.traits.push(Module.HEAP32[offset++] + 1);
                }
            }
            break;
        case ResultType.RT_Geyser: {
            world.geysers.length = 0;
            const end = offset + count;
            while (offset < end) {
                const index = Module.HEAP32[offset++];
                const x = Module.HEAP32[offset++];
                const y = Module.HEAP32[offset++];
                world.geysers.push({
                    index: index,
                    pos: { x: x, y: y },
                    desc: configuration.geyser[index],
                });
            }
            world.geysers.sort((a, b) => a.index - b.index);
            break;
        }
        case ResultType.RT_Polygon: {
            world.sites.length = 0;
            const end = offset + count;
            while (offset < end) {
                const zone = Module.HEAP32[offset++];
                const size = Module.HEAP32[offset++];
                const poly = new Array<Point>();
                for (let j = 0; j < size; j++) {
                    const x = Module.HEAP32[offset++];
                    const y = Module.HEAP32[offset++];
                    poly.push({ x: x, y: y });
                }
                world.sites.push({ zone: zone, poly: poly });
            }
            break;
        }
        case ResultType.RT_WorldSize:
            world.size.x = Module.HEAP32[offset++];
            world.size.y = Module.HEAP32[offset++];
            break;
        case ResultType.RT_Resource:
            if (Module.data!.length === count) {
                Module.HEAPU8.set(Module.data!, data);
            } else console.log("resource file size was wrong.");
            break;
        default:
            console.log("wrong type: " + type);
    }
};

export { configuration } from "./configuration";
export { LanguageContext, useTranslation } from "./language";
