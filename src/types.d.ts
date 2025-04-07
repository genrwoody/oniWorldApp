interface Point {
    x: number;
    y: number;
}

interface Site {
    zone: number;
    poly: Array<Point>;
}

interface Description {
    type: number;
    key?: string;
    name: string;
}

interface Geyser {
    index: number;
    pos: Point;
    desc: Description;
}

interface World {
    type: number;
    size: Point;
    starting: Point;
    traits: Array<number>;
    sites: Array<Site>;
    geysers: Array<Geyser>;
}

declare const Module: {
    wasm: Uint8Array | null;
    data: Uint8Array | null;
    worlds: Array<World>;
    HEAP32: Int32Array;
    HEAPU8: Uint8Array;
    updateWorld(type: number, count: number, data: number): void;
    locateFile(path: string, prefix: string): string;
    onRuntimeInitialized(): void;
    app_init(): void;
    app_generate(cluster: number, seed: number, mixing: number): boolean;
};
