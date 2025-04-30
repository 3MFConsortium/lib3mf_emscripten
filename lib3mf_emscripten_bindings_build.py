#!/usr/bin/env python3
import os
import sys
import subprocess
import argparse
from pathlib import Path

def find_emcc():
    """Locate the emcc compiler on PATH."""
    from shutil import which
    emcc = which("emcc")
    if not emcc:
        sys.exit("error: could not find 'emcc' on your PATH. Did you run emsdk_env?")
    return emcc

def can_print_checkmark():
    enc = sys.stdout.encoding or ""
    try:
        "✓".encode(enc)
        return True
    except Exception:
        return False

def bootstrap_and_install_vcpkg(vcpkg_root: Path, triplet: str):
    """
    Bootstrap vcpkg if needed and install lib3mf (to pull in its deps)
    """
    is_windows = os.name == "nt"
    # paths to bootstrap script + vcpkg executable
    if is_windows:
        bootstrap = vcpkg_root / "bootstrap-vcpkg.bat"
        vcpkg_exe = vcpkg_root / "vcpkg.exe"
        bootstrap_cmd = [str(bootstrap)]
    else:
        bootstrap = vcpkg_root / "bootstrap-vcpkg.sh"
        vcpkg_exe = vcpkg_root / "vcpkg"
        bootstrap_cmd = ["sh", str(bootstrap)]

    # bootstrap if vcpkg.exe (or vcpkg) doesn't exist
    if not vcpkg_exe.exists():
        print(f"Bootstrapping vcpkg ({bootstrap.name})…")
        try:
            subprocess.check_call(bootstrap_cmd, cwd=vcpkg_root)
        except subprocess.CalledProcessError as e:
            sys.exit(f"vcpkg bootstrap failed ({e.returncode})")

    # install lib3mf (to pull in dependencies)
    install_cmd = [
        str(vcpkg_exe),
        "install", "lib3mf",
        f"--triplet={triplet}",
        "--allow-unsupported"
    ]
    print(f"Installing lib3mf and its deps via vcpkg…")
    try:
        subprocess.check_call(install_cmd, cwd=vcpkg_root)
    except subprocess.CalledProcessError as e:
        sys.exit(f"vcpkg install failed ({e.returncode})")

def main():
    parser = argparse.ArgumentParser(
        description="Custom build script for lib3mf Emscripten wrapper"
    )
    parser.add_argument(
        "--vcpkg-root",
        default=os.environ.get("VCPKG_ROOT", str(Path(__file__).parent / "vcpkg")),
        help="Root of your vcpkg checkout (default: env VCPKG_ROOT or ./vcpkg)",
    )
    parser.add_argument(
        "--build-type",
        choices=("Debug", "Release"),
        default="Release",
        help="Build type (default: Release)",
    )
    parser.add_argument(
        "--out-dir",
        default="build",
        help="Output directory (default: build/)",
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="Enable emcc verbose output (-v)",
    )
    args = parser.parse_args()

    project_root = Path(__file__).parent.resolve()
    vcpkg_root   = Path(args.vcpkg_root).resolve()
    triplet      = "wasm32-emscripten"

    # 1) Bootstrap vcpkg & install lib3mf deps
    bootstrap_and_install_vcpkg(vcpkg_root, triplet)

    # 2) Prepare dirs and paths
    out_dir = project_root / args.out_dir
    out_dir.mkdir(parents=True, exist_ok=True)
    src    = project_root / "lib3mf_bindings.cpp"
    target = out_dir / "lib3mfEmscriptenTest.js"
    inst   = vcpkg_root / "installed" / triplet
    include_dir = inst / "include"
    lib_dir     = inst / "lib"

    static_libs = [
        "lib3mf.a",
        "libzip.a",
        "libz.a",
        "libbz2.a",
        "libssl.a",
        "libcrypto.a",
    ]

    # 3) Compile flags
    compile_flags = [
        "-std=c++17",
        "-O3" if args.build_type == "Release" else "-O0",
        "-I", str(include_dir),
    ]

    # 4) Link flags
    link_flags = [
        # verbose?
        # "-v",
        "-s", "WASM=1",
        "-s", "EXPORT_ALL=1",
        "-s", "ALLOW_MEMORY_GROWTH=1",
        "-s", "MAXIMUM_MEMORY=4294967296",  # 4GB
        "-s", "INITIAL_MEMORY=1073741824",  # 1GB
        "-s", "FORCE_FILESYSTEM=1",
        "-s", "EXPORTED_RUNTIME_METHODS=['ccall','cwrap']",
        "--bind",
        "-s", "MODULARIZE=1",
        "-s", "EXCEPTION_CATCHING_ALLOWED=['*']",
        "-s", "ASSERTIONS=1",
    ]
    if args.verbose:
        link_flags.insert(0, "-v")

    # 5) Build command
    emcc = find_emcc()
    cmd = [emcc] + compile_flags + ["-o", str(target), str(src)]
    cmd += [str(lib_dir / lib) for lib in static_libs]
    cmd += link_flags

    print("Running:")
    print(" ", " \\\n    ".join(cmd))
    try:
        subprocess.check_call(cmd)
    except subprocess.CalledProcessError as e:
        sys.exit(f"Build failed ({e.returncode})")

    # 6) Success message
    prefix = "✓ " if can_print_checkmark() else ""
    print(f"{prefix}Built {target.relative_to(project_root)}")

if __name__ == "__main__":
    main()