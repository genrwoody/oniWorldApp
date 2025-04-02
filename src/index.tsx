import React from "react";
import { useState, useEffect, useRef } from "react";
import { createRoot } from "react-dom/client";
import Stack from "react-bootstrap/Stack";
import Container from "react-bootstrap/Container";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";
import Card from "react-bootstrap/Card";
import InputGroup from "react-bootstrap/InputGroup";
import Button from "react-bootstrap/Button";
import Dropdown from "react-bootstrap/Dropdown";
import Form from "react-bootstrap/Form";
import Modal from "react-bootstrap/Modal";
import Navbar from "react-bootstrap/Navbar";

import "bootstrap/dist/css/bootstrap.min.css";
import "./index.css";

class Point {
    x: number = 0;
    y: number = 0;
}

class Site {
    zone: number = 0;
    poly: Array<Point> = [];
}

interface GeyserConfig {
    width: number;
    height: number;
    color: number;
    name: string;
}

class Geyser {
    index: number = 0;
    pos: Point = new Point();
    config?: GeyserConfig;
}

class World {
    size: Point = new Point();
    starting: Point = new Point();
    traits: Array<number> = [];
    sites: Array<Site> = [];
    geysers: Array<Geyser> = [];
}

declare const Module: {
    world: World;
    HEAP32: Int32Array;
    updateWorld(type: number, count: number, data: number): void;
    locateFile(path: string, prefix: string): string;
    onRuntimeInitialized(): void;
    _app_init(): void;
    _app_generate(cluster: number, seed: number, mixing: number): boolean;
};

const gameCategories = ["本体", "经典", "眼冒金星"];
const clusterConfigs = [
    { type: 0, key: "SNDST-A", name: "类地星体" },
    { type: 0, key: "OCAN-A", name: "海洋星体" },
    { type: 0, key: "S-FRZ", name: "冰霜星体" },
    { type: 0, key: "LUSH-A", name: "翠绿星体" },
    { type: 0, key: "FRST-A", name: "乔木星体" },
    { type: 0, key: "VOLCA", name: "火山星体" },
    { type: 0, key: "BAD-A", name: "荒芜之地" },
    { type: 0, key: "HTFST-A", name: "干热星体" },
    { type: 0, key: "OASIS-A", name: "绿洲星体" },
    { type: 0, key: "CER-A", name: "谷神星" },
    { type: 0, key: "CERS-A", name: "炸裂的谷神星" },
    { type: 1, key: "V-SNDST-C", name: "类地星群" },
    { type: 1, key: "V-OCAN-C", name: "海洋星群" },
    { type: 1, key: "V-SWMP-C", name: "泥泞星群" },
    { type: 1, key: "V-SFRZ-C", name: "冰霜星群" },
    { type: 1, key: "V-LUSH-C", name: "翠绿星群" },
    { type: 1, key: "V-FRST-C", name: "乔木星群" },
    { type: 1, key: "V-VOLCA-C", name: "火山星群" },
    { type: 1, key: "V-BAD-C", name: "荒芜星群" },
    { type: 1, key: "V-HTFST-C", name: "干热星群" },
    { type: 1, key: "V-OASIS-C", name: "绿洲星群" },
    { type: 1, key: "V-CER-C", name: "谷神星星群" },
    { type: 1, key: "V-CERS-C", name: "炸裂的谷神星星群" },
    { type: 2, key: "SNDST-C", name: "砂土星群" },
    { type: 2, key: "CER-C", name: "小谷神星星群" },
    { type: 2, key: "FRST-C", name: "绿叶星群" },
    { type: 2, key: "SWMP-C", name: "泥潭星群" },
    { type: 2, key: "M-SWMP-C", name: "卫星星群 - 金属沼泽" },
    { type: 2, key: "M-BAD-C", name: "卫星星群 - 荒凉" },
    { type: 2, key: "M-FRZ-C", name: "卫星星群 - 冰冻森林" },
    { type: 2, key: "M-FLIP-C", name: "卫星星群 - 倒置" },
    { type: 2, key: "M-RAD-C", name: "卫星星群 - 放射性海洋" },
    { type: 2, key: "M-CERS-C", name: "卫星星群 - 谷神星地幔" },
];
const mixingConfigs = [
    { key: "dlc3", type: 0, name: "仿生增幅包" },
    { key: "mix4", type: 1, name: "谷神星碎片" },
    { key: "mix3", type: 1, name: "花蜜" },
    { key: "mix2", type: 1, name: "冷池" },
    { key: "mix1", type: 1, name: "冰窟" },
    { key: "dlc2", type: 0, name: "寒霜行星包" },
];
const geyserConfigs = [
    { width: 2, height: 4, color: 1, name: "低温蒸汽喷孔" },
    { width: 2, height: 4, color: 0, name: "蒸汽喷孔" },
    { width: 4, height: 2, color: 1, name: "清水泉" },
    { width: 4, height: 2, color: 1, name: "低温泥浆泉" },
    { width: 4, height: 2, color: 1, name: "污水泉" },
    { width: 4, height: 2, color: 1, name: "低温盐泥泉" },
    { width: 4, height: 2, color: 1, name: "盐水泉" },
    { width: 3, height: 3, color: 0, name: "小型火山" },
    { width: 3, height: 3, color: 0, name: "火山" },
    { width: 4, height: 4, color: 2, name: "二氧化碳泉" },
    { width: 2, height: 4, color: 2, name: "二氧化碳喷孔" },
    { width: 2, height: 4, color: 2, name: "氢气喷孔" },
    { width: 2, height: 4, color: 2, name: "高温污氧喷孔" },
    { width: 2, height: 4, color: 2, name: "含菌污氧喷孔" },
    { width: 2, height: 4, color: 2, name: "氯气喷孔" },
    { width: 2, height: 4, color: 0, name: "天然气喷孔" },
    { width: 3, height: 3, color: 0, name: "铜火山" },
    { width: 3, height: 3, color: 0, name: "铁火山" },
    { width: 3, height: 3, color: 0, name: "金火山" },
    { width: 3, height: 3, color: 0, name: "铝火山" },
    { width: 3, height: 3, color: 0, name: "钴火山" },
    { width: 4, height: 2, color: 0, name: "渗油裂缝" },
    { width: 4, height: 2, color: 0, name: "液硫泉" },
    { width: 3, height: 3, color: 0, name: "钨火山" },
    { width: 3, height: 3, color: 0, name: "铌火山" },
    { width: 4, height: 4, color: 0, name: "打印仓" },
    { width: 4, height: 4, color: 0, name: "储油石" },
    { width: 4, height: 4, color: 0, name: "供给传送器输出端" },
    { width: 4, height: 4, color: 0, name: "供给传送器输入端" },
    { width: 4, height: 4, color: 0, name: "传送发射/接收器" },
    { width: 4, height: 4, color: 0, name: "低温箱3000" },
];
const traitConfigs = [
    { color: 0, name: "没有不寻常的特质" },
    { color: 0, name: "坠毁的卫星群" },
    { color: 0, name: "冰封之友" },
    { color: 0, name: "不规则原油区" },
    { color: 0, name: "繁茂核心" },
    { color: 0, name: "金属洞穴" },
    { color: 0, name: "放射性地壳" },
    { color: 0, name: "地下海洋" },
    { color: 0, name: "火山活跃" },
    { color: 2, name: "大型石块" },
    { color: 0, name: "中型石块" },
    { color: 2, name: "混合型石块" },
    { color: 0, name: "小型石块" },
    { color: 0, name: "被圈闭的原油" },
    { color: 0, name: "冰冻核心" },
    { color: 1, name: "活跃性地质" },
    { color: 1, name: "晶洞" },
    { color: 2, name: "休眠性地质" },
    { color: 0, name: "大型冰川" },
    { color: 0, name: "不规则原油区" },
    { color: 0, name: "岩浆通道" },
    { color: 2, name: "金属贫瘠" },
    { color: 1, name: "金属富足" },
    { color: 0, name: "备选的打印舱位置" },
    { color: 0, name: "粘液菌团" },
    { color: 0, name: "地下海洋" },
    { color: 0, name: "火山活跃" },
];

// prettier-ignore
const zoneColor = [
    "#5A6870", "#000080", "#545B42", "#5A4E3C", "#5B5B52", "#D73518", "#56403C",
    "#000000", "#665A57", "#756763", "#4F593E", "#91888C", "#808000", "#808080",
    "#8080FF", "#80FF00", "#6F5E45", "#80FFFF", "#5D6B76", "#49545C", "#738391",
];

function generate(cluster: number, seed: number, mixing: number) {
    const world = Module.world;
    world.traits.length = 0;
    world.sites.length = 0;
    world.geysers.length = 0;

    Module._app_generate(cluster, seed, mixing);

    const cvs = document.getElementById("world") as HTMLCanvasElement;
    cvs.height = 650;
    const scale = cvs.height / world.size.y;
    cvs.width = Math.floor(scale * world.size.x);
    const ctx = cvs.getContext("2d")!;
    ctx.fillStyle = "white";
    ctx.textAlign = "center";
    ctx.textBaseline = "middle";
    ctx.fillRect(0, 0, cvs.width, cvs.height);
    ctx.strokeRect(0, 0, cvs.width, cvs.height);
    world.sites.forEach((item) => {
        ctx.beginPath();
        item.poly.forEach((point, index) => {
            if (index === 0) {
                ctx.moveTo(point.x * scale, cvs.height - point.y * scale);
            } else {
                ctx.lineTo(point.x * scale, cvs.height - point.y * scale);
            }
        });
        ctx.closePath();
        ctx.fillStyle = zoneColor[item.zone];
        ctx.fill();
    });
    ctx.fillStyle = "black";
    ctx.font = "16px sans-serif";
    let point = world.starting;
    let text = geyserConfigs[25].name;
    ctx.fillText(text, point.x * scale, cvs.height - point.y * scale);
    world.geysers.forEach((item) => {
        item.config = geyserConfigs[item.index];
        const w = item.config.width * scale;
        const h = item.config.height * scale;
        const x = item.pos.x * scale;
        const y = cvs.height - item.pos.y * scale;
        ctx.strokeRect(x, y - h, w, h);
        ctx.fillText(item.config.name, x, y);
    });
    world.geysers.sort((a, b) => a.index - b.index);
}

function updateWorld(type: number, count: number, data: number) {
    const world = Module.world;
    let offset = data >> 2;
    if (type == 0) {
        world.starting.x = Module.HEAP32[offset++];
        world.starting.y = Module.HEAP32[offset++];
    } else if (type == 1) {
        if (count === 0) {
            world.traits.push(0);
        } else {
            for (let i = 0; i < count; i++) {
                world.traits.push(Module.HEAP32[offset++] + 1);
            }
        }
    } else if (type == 2) {
        const end = offset + count;
        while (offset < end) {
            const index = Module.HEAP32[offset++];
            const x = Module.HEAP32[offset++];
            const y = Module.HEAP32[offset++];
            world.geysers.push({ index: index, pos: { x: x, y: y } });
        }
    } else if (type == 3) {
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
    } else if (type == 4) {
        world.size.x = Module.HEAP32[offset++];
        world.size.y = Module.HEAP32[offset++];
    }
}

interface ConfigProps {
    type: number;
    name?: string;
    hidden?: boolean;
    active: number;
    onSetActive: (active: number) => void;
}

const MixingItem = (props: ConfigProps) => {
    const states = [
        [
            { name: "禁用", variant: "secondary" },
            { name: "启用", variant: "primary" },
        ],
        [
            { name: "禁用", variant: "secondary" },
            { name: "可能", variant: "success" },
            { name: "确保", variant: "primary" },
        ],
    ];
    return (
        <Dropdown.ItemText hidden={props.hidden}>
            <Dropdown>
                <Dropdown.Toggle
                    variant={states[props.type][props.active].variant}
                >
                    {props.name}
                </Dropdown.Toggle>
                <Dropdown.Menu
                    style={{
                        top: "auto",
                        left: "var(--bs-dropdown-item-padding-x)",
                    }}
                >
                    {states[props.type].map((item, index) => (
                        <Dropdown.Item
                            key={index}
                            eventKey={index}
                            active={props.active === index}
                            onClick={() => props.onSetActive(index)}
                        >
                            {item.name}
                        </Dropdown.Item>
                    ))}
                </Dropdown.Menu>
            </Dropdown>
        </Dropdown.ItemText>
    );
};

const Mixings = ({ type, onSetActive }: ConfigProps) => {
    const [options, setOptions] = useState([1, 0, 0, 0, 0, 1]);
    const handleActive = (index: number, value: number) => {
        const temp = options.map((n, i) => (index === i ? value : n));
        let active = 0;
        for (let i = temp.length; i > 0; i--) {
            active = active * 5 + temp[i - 1];
        }
        setOptions(temp);
        onSetActive(active);
    };
    return (
        <Dropdown autoClose="outside">
            <Dropdown.Toggle
                variant="primary"
                id="mixings"
                style={{ borderRadius: 0 }}
            >
                生态融入
            </Dropdown.Toggle>
            <Dropdown.Menu>
                {mixingConfigs.map((item, index) => (
                    <MixingItem
                        key={item.key}
                        type={item.type}
                        name={item.name}
                        active={options[index]}
                        onSetActive={(active) => handleActive(index, active)}
                        hidden={type === 0 && index === 1}
                    />
                ))}
            </Dropdown.Menu>
        </Dropdown>
    );
};

const ToolBar = ({ onSetWorld }: { onSetWorld: () => void }) => {
    const [category, setCategory] = useState(0);
    const [cluster, setCluster] = useState(0);
    const [mixings, setMixings] = useState(3126);
    const [seed, setSeed] = useState("");
    const onSetCategory = (value: number) => {
        setCategory(value);
        setCluster(value === 0 ? 0 : value === 1 ? 11 : 23);
    };
    const toBase36 = (num: number) => {
        if (num === 0) return "0";
        const hexChars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        let text = "";
        while (num > 0) {
            text += hexChars[num % 36];
            num = Math.floor(num / 36);
        }
        return text;
    };
    const copyToClipboard = async (text: string): Promise<void> => {
        try {
            await navigator.clipboard.writeText(text);
        } catch (err) {}
    };
    return (
        <Stack direction="horizontal">
            <Dropdown>
                <Dropdown.Toggle
                    variant="primary"
                    id="category"
                    style={{
                        borderTopRightRadius: 0,
                        borderBottomRightRadius: 0,
                    }}
                >
                    {gameCategories[category]}
                </Dropdown.Toggle>
                <Dropdown.Menu>
                    {gameCategories.map((item, index) => (
                        <Dropdown.Item
                            key={index}
                            active={category === index}
                            onClick={() => onSetCategory(index)}
                        >
                            {item}
                        </Dropdown.Item>
                    ))}
                </Dropdown.Menu>
            </Dropdown>
            <Dropdown>
                <Dropdown.Toggle
                    variant="success"
                    id="clusters"
                    style={{ borderRadius: 0 }}
                >
                    {clusterConfigs[cluster].name}
                </Dropdown.Toggle>
                <Dropdown.Menu>
                    {clusterConfigs.map((item, index) => (
                        <Dropdown.Item
                            key={item.key}
                            eventKey={index}
                            active={cluster === index}
                            onClick={() => setCluster(index)}
                            hidden={category !== item.type}
                        >
                            {item.name}
                        </Dropdown.Item>
                    ))}
                </Dropdown.Menu>
            </Dropdown>
            <InputGroup>
                <Mixings
                    type={category === 0 ? 0 : 1}
                    active={mixings}
                    onSetActive={(active) => setMixings(active)}
                />
                <Form.Control
                    type="text"
                    value={seed}
                    placeholder="种子"
                    onChange={(e) => setSeed(e.target.value)}
                    onKeyDown={(e) => {
                        if (e.key === "Enter") {
                            generate(cluster, parseInt(seed), mixings);
                            onSetWorld();
                        }
                    }}
                />
                <Button
                    onClick={() => {
                        let nseed = Math.random() * 32768;
                        nseed = nseed * (Math.random() * 32768);
                        nseed = Math.round(nseed);
                        setSeed(nseed.toString());
                        generate(cluster, nseed, mixings);
                        onSetWorld();
                    }}
                >
                    刷新
                </Button>
                <Button
                    onClick={() => {
                        const name = clusterConfigs[cluster].key;
                        const mix = toBase36(mixings);
                        const sseed = `${name}-${seed}-0-D3-${mix}`;
                        copyToClipboard(sseed);
                    }}
                >
                    复制
                </Button>
            </InputGroup>
        </Stack>
    );
};

const WorldInfo = ({ world }: { world: World }) => {
    const convert = (color?: number) => {
        const list = ["success", "danger", "primary"];
        return color ? list[color - 1] : undefined;
    };
    return (
        <>
            <Row xs={2} md={4}>
                {world.traits.map((item, index) => (
                    <Card key={index} text={convert(traitConfigs[item].color)}>
                        <Card.Body>{traitConfigs[item].name}</Card.Body>
                    </Card>
                ))}
            </Row>
            <Row xs={2} md={4}>
                {world.geysers.map((item, index) => (
                    <Card key={index} text={convert(item.config!.color)}>
                        <Card.Body>{item.config!.name}</Card.Body>
                    </Card>
                ))}
            </Row>
        </>
    );
};

const App: React.FC = () => {
    const [show, setShow] = useState(false);
    const [world, setWorld] = useState(new World());
    const [theme, setTheme] = useState(0);
    const wasmLoaded = useRef(false);
    useEffect(() => {
        if (wasmLoaded.current) return;
        wasmLoaded.current = true;
        Module.world = new World();
        Module.updateWorld = updateWorld;
        Module.locateFile = (path: string, prefix: string) => {
            if (process.env.NODE_ENV === "development") {
                return prefix + path;
            } else {
                if (path.endsWith(".data")) return "data-1.bin";
                else if (path.endsWith(".wasm")) return "wasm-3.bin";
                else return prefix + path;
            }
        };
        Module.onRuntimeInitialized = () => {
            Module._app_init();
            setShow(false);
        };
        setShow(true);
        setTimeout(() => {
            const script = document.createElement("script");
            script.src = "oniWorldApp-1.js";
            script.async = true;
            document.body.appendChild(script);
        }, 100);
    }, []);
    const switchTheme = () => {
        const expect = theme ? "light" : "dark";
        setTheme(theme ? 0 : 1);
        document.documentElement.setAttribute("data-bs-theme", expect);
    };
    return (
        <>
            <Navbar className="bg-body-tertiary justify-content-between">
                <Container>
                    <ToolBar onSetWorld={() => setWorld({ ...Module.world })} />
                    <Stack direction="horizontal" gap={3}>
                        <a
                            href="https://github.com/genrwoody/oniWorldApp"
                            target="_blank"
                            className="header-github-link"
                        />
                        <Button
                            variant={"outline-" + (theme ? "light" : "dark")}
                            onClick={() => switchTheme()}
                        >
                            {theme ? "浅色" : "暗黑"}
                        </Button>
                        <span>v1.0.0</span>
                    </Stack>
                </Container>
            </Navbar>
            <Container>
                <Row>
                    <Col lg={12} xl={6}>
                        <WorldInfo world={world} />
                    </Col>
                    <Col>
                        <canvas id="world" width="100" height="100"></canvas>
                    </Col>
                </Row>
            </Container>
            <Modal
                id="loading"
                show={show}
                backdrop="static"
                keyboard={false}
                centered
            >
                <Modal.Body>正在初始化, 请稍候...</Modal.Body>
            </Modal>
        </>
    );
};

const root = createRoot(document.getElementById("root")!);
if (process.env.NODE_ENV === "development") {
    root.render(
        <React.StrictMode>
            <App />
        </React.StrictMode>
    );
} else {
    root.render(<App />);
}
