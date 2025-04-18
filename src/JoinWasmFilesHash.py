import hashlib
import shutil


def calculate_md5(filepath):
    md5 = hashlib.md5()
    with open(filepath, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            md5.update(chunk)
    return md5.hexdigest()


def main():
    shutil.copy("oniWorldApp.wasm", "wasm.bin")
    typescript = """export const WasmFiles = {{
    launcher: "oniWorldApp.js?{}",
    wasm: "wasm.bin?{}",
    data: "data.bin?{}",
}};
""".format(
        calculate_md5("oniWorldApp.js"),
        calculate_md5("wasm.bin"),
        calculate_md5("data.bin"),
    )
    with open("WasmFiles.ts", "w") as stm:
        stm.write(typescript)


if __name__ == "__main__":
    main()
