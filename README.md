# oniWorldApp
生成缺氧世界的间歇泉信息.

## 构建
需要的工具:
- pc版: msvc, cmake, python
```sh
cmake --preset=x86-release
cmake --build out/build/x86-release
# run
./out/build/x86-release/src/oniWorldApp
```
- web版: emsdk, cmake, python, node.js, yarn, emsdk中带有python和node.js
```sh
cmake --preset=wasm-release
cmake --build out/build/wasm-release
yarn install
yarn run build
```
或者debug
```sh
cmake --preset=wasm-debug
cmake --build out/build/wasm-release
yarn install
yarn run start
```

## license
MIT, 第三方库见[3rdparyt](./3rdparty/README.md), 以及[科雷](https://www.klei.com/), [缺氧](https://www.klei.com/games/oxygen-not-included)

