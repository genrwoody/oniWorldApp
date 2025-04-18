const VERSION = "v1.0.3";
const CACHE_NAME = `OniWorldApp-${VERSION}`;
const APP_STATIC_RESOURCES = [
    "index.html",
    "favicon.png",
    "128x128.png",
    "manifest.json",
    "bundle.js",
    "oniWorldApp.js",
    "data.bin",
    "wasm.bin",
];

self.addEventListener("install", (event) => {
    event.waitUntil(
        (async () => {
            const cache = await caches.open(CACHE_NAME);
            cache.addAll(APP_STATIC_RESOURCES);
        })()
    );
});

self.addEventListener("activate", (event) => {
    event.waitUntil(
        (async () => {
            const names = await caches.keys();
            await Promise.all(
                names.map((name) => {
                    if (name !== CACHE_NAME) {
                        return caches.delete(name);
                    }
                })
            );
            await clients.claim();
        })()
    );
});

self.addEventListener("fetch", (event) => {
    if (event.request.mode === "navigate") {
        event.respondWith(caches.match("index.html"));
        return;
    }
    event.respondWith(
        (async () => {
            const cache = await caches.open(CACHE_NAME);
            const cachedResponse = await cache.match(event.request.url, {
                ignoreSearch: true,
            });
            if (cachedResponse) {
                return cachedResponse;
            } else {
                return new Response(null, { status: 404 });
            }
        })()
    );
});
