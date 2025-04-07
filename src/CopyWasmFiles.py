import hashlib
import shutil
import re
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


def remove_old_files():
    regex = re.compile(r"^\w+: \"(.+)\",$")
    with open("WasmFiles.ts", "r") as stm:
        for line in stm:
            matchs = regex.match(line.strip())
            if matchs and os.path.exists(matchs[1]):
                print("remove old file: " + matchs[1])
                os.remove(matchs[1])


def main():
    if os.path.exists("WasmFiles.ts"):
        remove_old_files()
    shutil.copy("oniWorldApp.wasm", "wasm.bin")
    typescript = """export const WasmFiles = {{
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
