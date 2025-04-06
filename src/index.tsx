import React from "react";
import { useState, useEffect, useContext } from "react";
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
import { configuration, LanguageContext, useTranslation } from "./jsUtils";

import "bootstrap/dist/css/bootstrap.min.css";
import "./jsUtils/index.css";

interface WasmFiles {
    launcher: string;
    wasm: string;
    data: string;
}

interface ConfigProps {
    type: number;
    name?: string;
    hidden?: boolean;
    active: number;
    onSetActive: (active: number) => void;
}

const MixingItem = (props: ConfigProps) => {
    const translation = useTranslation();
    const states = [
        [
            { name: "Disable", variant: "secondary" },
            { name: "Enable", variant: "primary" },
        ],
        [
            { name: "Disable", variant: "secondary" },
            { name: "Likely", variant: "success" },
            { name: "Guaranteed", variant: "primary" },
        ],
    ];
    const items = states[props.type];
    return (
        <Dropdown.ItemText hidden={props.hidden}>
            <Dropdown>
                <Dropdown.Toggle variant={items[props.active].variant}>
                    {translation(props.name!)}
                </Dropdown.Toggle>
                <Dropdown.Menu
                    style={{
                        top: "auto",
                        left: "var(--bs-dropdown-item-padding-x)",
                    }}
                >
                    {items.map((item, index) => (
                        <Dropdown.Item
                            key={index}
                            eventKey={index}
                            active={props.active === index}
                            onClick={() => props.onSetActive(index)}
                        >
                            {translation(item.name)}
                        </Dropdown.Item>
                    ))}
                </Dropdown.Menu>
            </Dropdown>
        </Dropdown.ItemText>
    );
};

const Mixings = ({ type, onSetActive }: ConfigProps) => {
    const [options, setOptions] = useState([1, 0, 0, 0, 0, 1]);
    const translation = useTranslation();
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
                {translation("Scramble DLCs")}
            </Dropdown.Toggle>
            <Dropdown.Menu>
                {configuration.mixing.map((item, index) => (
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
    const translation = useTranslation();
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
                    {translation(configuration.game[category].name)}
                </Dropdown.Toggle>
                <Dropdown.Menu>
                    {configuration.game.map((item, index) => (
                        <Dropdown.Item
                            key={index}
                            active={category === index}
                            onClick={() => onSetCategory(index)}
                        >
                            {translation(item.name)}
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
                    {translation(configuration.cluster[cluster].name)}
                </Dropdown.Toggle>
                <Dropdown.Menu>
                    {configuration.cluster.map((item, index) => (
                        <Dropdown.Item
                            key={item.key}
                            eventKey={index}
                            active={cluster === index}
                            onClick={() => setCluster(index)}
                            hidden={category !== item.type}
                        >
                            {translation(item.name)}
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
                    placeholder={translation("Worldgen Seed")}
                    onChange={(e) => setSeed(e.target.value)}
                    onKeyDown={(e) => {
                        if (e.key === "Enter") {
                            let nseed = parseInt(seed);
                            Module.app_generate(cluster, nseed, mixings);
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
                        Module.app_generate(cluster, nseed, mixings);
                        onSetWorld();
                    }}
                >
                    {translation("Reroll")}
                </Button>
                <Button
                    onClick={() => {
                        const name = configuration.cluster[cluster].key;
                        const mix = toBase36(mixings);
                        const sseed = `${name}-${seed}-0-D3-${mix}`;
                        copyToClipboard(sseed);
                    }}
                >
                    {translation("Copy")}
                </Button>
            </InputGroup>
        </Stack>
    );
};

const WorldInfo = ({ world }: { world: World }) => {
    const translation = useTranslation();
    const convert = (color?: number) => {
        const list = ["success", "danger", "primary"];
        return color ? list[color - 1] : undefined;
    };
    const traits = configuration.traits;
    return (
        <Col lg={12} xl={6}>
            <Row xs={2} md={4}>
                {world.traits.map((item, index) => (
                    <Card key={index} text={convert(traits[item].type)}>
                        <Card.Body>{translation(traits[item].name)}</Card.Body>
                    </Card>
                ))}
            </Row>
            <Row xs={2} md={4}>
                {world.geysers.map((item, index) => (
                    <Card key={index} text={convert(item.desc.type)}>
                        <Card.Body>{translation(item.desc.name)}</Card.Body>
                    </Card>
                ))}
            </Row>
        </Col>
    );
};

// prettier-ignore
const zoneColor = [
    "#5A6870", "#000080", "#545B42", "#5A4E3C", "#5B5B52", "#D73518", "#56403C",
    "#000000", "#665A57", "#756763", "#4F593E", "#91888C", "#808000", "#808080",
    "#8080FF", "#8E7A5C", "#6F5E45", "#80FFFF", "#5D6B76", "#49545C", "#738391",
];

const WorldCanvas = ({ world, theme }: { world: World; theme: number }) => {
    const language = useContext(LanguageContext);
    const translation = useTranslation();
    useEffect(() => {
        if (world.sites.length === 0) return;
        const cvs = document.getElementById("world") as HTMLCanvasElement;
        cvs.style.aspectRatio = `${cvs.clientWidth} / ${cvs.clientHeight}`;
        cvs.width = cvs.clientWidth;
        cvs.height = cvs.clientHeight;
        let width = 0;
        let height = 0;
        if (cvs.clientWidth / cvs.clientHeight < world.size.x / world.size.y) {
            width = cvs.clientWidth;
            height = (world.size.y / world.size.x) * width;
        } else {
            height = cvs.clientHeight;
            width = (world.size.x / world.size.y) * height;
        }
        const scale = height / world.size.y;
        const ctx = cvs.getContext("2d")!;
        ctx.fillStyle = theme ? "#212529" : "white";
        ctx.fillRect(0, 0, cvs.width, cvs.height);
        ctx.strokeRect(0, 0, width, height);
        world.sites.forEach((item) => {
            ctx.beginPath();
            item.poly.forEach((point, index) => {
                if (index === 0) {
                    ctx.moveTo(point.x * scale, height - point.y * scale);
                } else {
                    ctx.lineTo(point.x * scale, height - point.y * scale);
                }
            });
            ctx.closePath();
            ctx.fillStyle = zoneColor[item.zone];
            ctx.fill();
        });
        ctx.fillStyle = "black";
        ctx.font = "16px sans-serif";
        ctx.textAlign = "center";
        ctx.textBaseline = "middle";
        let point = world.starting;
        let text = translation("Printing Pod");
        ctx.fillText(text, point.x * scale, height - point.y * scale);
        world.geysers.forEach((item) => {
            const w = 5 * scale;
            const h = 5 * scale;
            const x = item.pos.x * scale;
            const y = height - item.pos.y * scale;
            ctx.strokeRect(x - w / 2, y - h, w, h);
            ctx.fillText(translation(item.desc.name), x, y);
        });
    }, [world, language, theme]);
    return (
        <Col>
            <Card className="world-canvas-container">
                <canvas id="world" className="world-canvas"></canvas>
            </Card>
        </Col>
    );
};

const emptyWorld: World = {
    size: { x: 0, y: 0 },
    starting: { x: 0, y: 0 },
    traits: [],
    sites: [],
    geysers: [],
};

const App = ({ onSetLanguage }: { onSetLanguage: (lang: string) => void }) => {
    const [show, setShow] = useState(false);
    const [world, setWorld] = useState(emptyWorld);
    const [theme, setTheme] = useState(0);
    const language = useContext(LanguageContext);
    const translation = useTranslation();
    useEffect(() => {
        if (Module.wasm !== undefined) return;
        Module.wasm = null;
        Module.world = world;
        Module.onRuntimeInitialized = () => {
            Module.app_init();
            setShow(false);
        };
        setShow(true);
        const load = async (url: string) => {
            const response = await fetch(url, { credentials: "same-origin" });
            return response.arrayBuffer();
        };
        (process.env.NODE_ENV === "development"
            ? import("../out/build/wasm-debug/src/WasmFiles")
            : import("../out/build/wasm-release/src/WasmFiles")
        ).then((module) => {
            Promise.all([
                load(module.WasmFiles.data),
                load(module.WasmFiles.wasm),
            ])
                .then((buffers) => {
                    Module.data = new Uint8Array(buffers[0], 4);
                    Module.wasm = new Uint8Array(buffers[1]);
                    const script = document.createElement("script");
                    script.src = module.WasmFiles.launcher;
                    script.async = true;
                    document.body.appendChild(script);
                })
                .catch((reason) => console.log("fetch error: " + reason));
        });
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
                        <Dropdown>
                            <Dropdown.Toggle>{"En/中"}</Dropdown.Toggle>
                            <Dropdown.Menu>
                                {configuration.languages.map((item) => (
                                    <Dropdown.Item
                                        key={item.key}
                                        active={language === item.key!}
                                        onClick={() => onSetLanguage(item.key!)}
                                    >
                                        {item.name}
                                    </Dropdown.Item>
                                ))}
                            </Dropdown.Menu>
                        </Dropdown>
                        <Button
                            variant={"outline-" + (theme ? "light" : "dark")}
                            onClick={() => switchTheme()}
                        >
                            {translation(theme ? "Light" : "Dark")}
                        </Button>
                        <a
                            href="https://github.com/genrwoody/oniWorldApp"
                            target="_blank"
                            className="header-github-link"
                        />
                        <span>v1.0.0</span>
                    </Stack>
                </Container>
            </Navbar>
            <Container>
                <Row>
                    <WorldInfo world={world} />
                    <WorldCanvas world={world} theme={theme} />
                </Row>
            </Container>
            <Modal
                id="loading"
                show={show}
                backdrop="static"
                keyboard={false}
                centered
            >
                <Modal.Body>
                    {translation("Initializing, please wait a moment.")}
                </Modal.Body>
            </Modal>
        </>
    );
};

const Main: React.FC = () => {
    const [language, setLanguage] = useState(navigator.language);
    return (
        <LanguageContext.Provider value={language}>
            <App onSetLanguage={(lang) => setLanguage(lang)} />
        </LanguageContext.Provider>
    );
};

const root = createRoot(document.getElementById("root")!);
if (process.env.NODE_ENV === "development") {
    root.render(
        <React.StrictMode>
            <Main />
        </React.StrictMode>
    );
} else {
    root.render(<Main />);
}
