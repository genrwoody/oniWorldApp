import hashlib
import shutil
import os


def calculate_md5(filepath):
    md5 = hashlib.md5()
    with open(filepath, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            md5.update(chunk)
    return md5.hexdigest()


def copy_with_hash(filepath):
    basename, ext = os.path.splitext(filepath)
    hash = calculate_md5(filepath)
    copyname = f"{basename}-{hash[:8]}{ext}"
    shutil.copy(filepath, copyname)
    return copyname


def main():
    typescript = """
export const WasmFiles = {{
    launcher: "{}",
    wasm: "{}",
    data: "{}",
}};
""".format(
        copy_with_hash("oniWorldApp.js"),
        copy_with_hash("wasm.bin"),
        copy_with_hash("data.bin"),
    )
    with open("WasmFiles.ts", "w") as stm:
        stm.write(typescript)


if __name__ == "__main__":
    main()
